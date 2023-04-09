cbuffer CameraBuffer : register(b0)
{
	matrix CameraMatrix;
}

cbuffer TransformBuffer : register(b1)
{
    matrix TransformMatrix;
}

cbuffer LightBuffer : register(b2)
{
    float4 LightDir;
    float4 LightColor;
}

struct VS_IN
{
	float4 Pos : POSITION;
	float4 Color : COLOR;
    float3 Norm : NORMAL;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    float3 Norm : TEXCOORD0;
};


VS_OUTPUT VS(VS_IN input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = mul(input.Pos, TransformMatrix);
    output.Pos = mul(output.Pos, CameraMatrix);
    output.Color = input.Color;
    output.Norm = mul(input.Norm, TransformMatrix);
    return output;
}


float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 color = 0;

    color += saturate(dot((float3)LightDir, input.Norm) * LightColor);
    color.a = 1;

    return color;
}