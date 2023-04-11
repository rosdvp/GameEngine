cbuffer CameraBuffer : register(b0) // 76 -> 80
{
	matrix CameraMatrix;    // 64   [0 left]
    float3 CameraPos;       // 12   [4 left]
}

cbuffer TransformBuffer : register(b1)
{
    matrix TransformMatrix; // 64   [0 left]
}

cbuffer MaterialBuffer : register(b2) // 16
{
    float MatDiffuse;       // 4    [12 left]
    float MatAmbient;       // 4    [8 left]
    float MatSpecular;      // 4    [4 left]
    float MatSpecularSize;  // 4    [0 left]
}

cbuffer LightBuffer : register(b3) // 20 -> 32
{
    float AmbientIntensity; // 4    [12 left] 
    float3 LightDir;        // 12   [0 left]
    float4 LightColor;      // 16    [0 left]
}


struct VS_IN
{
	float4 Pos : POSITION;
	float4 Color : COLOR;
    float3 Normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 WorldPos : POSITION;
    float4 Color : COLOR;
    float3 Normal : TEXCOORD0;
};


VS_OUTPUT VS(VS_IN input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.WorldPos = mul(input.Pos, TransformMatrix);
    output.Pos = mul(output.WorldPos, CameraMatrix);
    output.Color = input.Color;
    output.Normal = normalize(mul(input.Normal, TransformMatrix));
    return output;
}


float4 PS(VS_OUTPUT input) : SV_Target
{
    //------------LIGHT-----------
    float3 dirToCamera = normalize(CameraPos - (float3)input.WorldPos);

    input.Normal = normalize(input.Normal);
    float3 reflectionDir = reflect(LightDir, input.Normal);

    float4 ambient = MatAmbient * AmbientIntensity;
    float4 diffuse = MatDiffuse * saturate(dot(input.Normal, LightDir));
    float4 specular = MatSpecular * pow(saturate(dot(reflectionDir, -dirToCamera)), MatSpecularSize);
    //----------------------------

    float4 color = ambient + (diffuse + specular) * LightColor;
    
    return color * input.Color;
}