cbuffer CameraBuffer : register(b0)
{
	matrix CameraMatrix;
}

cbuffer TransformBuffer : register(b1)
{
	matrix WorldMatrix;
}

cbuffer LightBuffer : register(b2)
{
    float4 LightAmbient;

    float4 LightDir;
    float4 LightColor;
}

SamplerState Sampler : register(s0);
Texture2D Tx : register(t0);


struct VS_IN
{
	float4 Pos : POSITION;
	float4 Color : COLOR;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    float3 Normal : TEXCOORD0;
    float2 TexCoord : TEXCOORD1;
};


VS_OUTPUT VS(VS_IN input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = mul(input.Pos, WorldMatrix);
    output.Pos = mul(output.Pos, CameraMatrix);
    output.Color = input.Color;
    output.TexCoord = input.TexCoord;
    output.Normal = normalize(mul(input.Normal, WorldMatrix));
    return output;
}



float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 color = 0;
    
    color += saturate(dot((float3)LightDir, input.Normal) * LightColor);
    color *= Tx.Sample(Sampler, input.TexCoord);
    color.a = 1;

    return color;
}