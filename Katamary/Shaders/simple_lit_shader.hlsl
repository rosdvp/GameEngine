cbuffer CameraBuffer : register(b0) // 80
{
	matrix CameraViewProjMatrix;    // 64   [0 left]
    float3 CameraPos;               // 12   [4 left, goes to padding]
}

cbuffer TransformBuffer : register(b1)
{
    matrix ObjWorldMatrix; // 64   [0 left]
}

cbuffer MaterialBuffer : register(b2) // 52 -> 64
{
    float4 MatDiffuse;      // 16
    float4 MatAmbient;      // 16
    float4 MatSpecular;     // 16
    float MatSpecularSize;  // 4    [0 left]
}

cbuffer LightBuffer : register(b3) // 112
{
    float AmbientIntensity;     // 4    [12 left]
    float3 LightPos;            // 12   [0 left]
    float3 LightDir;            // 12   [4 left, goes to padding]
    float4 LightColor;          // 16   [0 left]
    matrix LightViewProjMatrix; // 64
}

Texture2D MainTex : register(t0);
SamplerState MainTexSampler : register(s0);

Texture2D ShadowMap : register(t1);
SamplerState ShadowMapSampler : register(s1);

struct VS_IN
{
	float4 Pos : POSITION;
	float4 Color : COLOR;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
};

struct PS_IN
{
    float4 Pos : SV_POSITION;
    float4 WorldPos : POSITION;
    float4 Color : COLOR;
    float3 Normal : TEXCOORD0;
    float2 TexCoord : TEXCOORD1;
    float4 ShadowMapPos : TEXCOORD2;
};


PS_IN VS(VS_IN input)
{
    PS_IN output = (PS_IN)0;
    output.WorldPos = mul(input.Pos, ObjWorldMatrix);
    output.Pos = mul(output.WorldPos, CameraViewProjMatrix);
    output.Color = input.Color;
    output.Normal = normalize(mul(input.Normal, ObjWorldMatrix));

    output.ShadowMapPos = mul(output.WorldPos, LightViewProjMatrix);

    return output;
}



float4 GetPhongLightColor(float3 pixelWorldPos, float3 pixelNormal, float shadowK)
{
    pixelNormal = normalize(pixelNormal);

    float3 dirToCamera = normalize(CameraPos - pixelWorldPos);

    float3 reflectionDir = reflect(LightDir, pixelNormal);

    float4 diffuse = MatDiffuse * saturate(dot(pixelNormal, LightDir));
    float4 specular = MatSpecular * pow(saturate(dot(reflectionDir, -dirToCamera)), MatSpecularSize);
    float4 ambient = MatAmbient * AmbientIntensity;

    return ambient + (diffuse + specular) * LightColor * shadowK;
}

float GetShadowK(float4 shadowMapPos)
{
    shadowMapPos.xyz /= shadowMapPos.w;

    if (shadowMapPos.x < -1.0f || shadowMapPos.x > 1.0f ||
        shadowMapPos.y < -1.0f || shadowMapPos.y > 1.0f ||
        shadowMapPos.z < 0.0f  || shadowMapPos.z > 1.0f) 
        return 0;

    shadowMapPos.x = shadowMapPos.x / 2 + 0.5;
    shadowMapPos.y = shadowMapPos.y / -2 + 0.5;

    float shadowMapDepth = ShadowMap.Sample(ShadowMapSampler, shadowMapPos.xy).r;
    if (shadowMapDepth < shadowMapPos.z)
        return 0;

    return 1;
}

float4 PS(PS_IN input) : SV_Target
{
    float shadowK = GetShadowK(input.ShadowMapPos);

    float4 color = GetPhongLightColor(input.WorldPos, (float3)input.Normal, shadowK);

    return color * input.Color;
}