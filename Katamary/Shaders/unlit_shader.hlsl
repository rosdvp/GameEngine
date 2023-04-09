cbuffer CameraBuffer : register(b0)
{
	matrix CameraMatrix;
}

cbuffer TransformBuffer : register(b1)
{
	matrix TransformMatrix;
}

SamplerState Sampler : register(s0);
Texture2D Tx : register(t0);


struct VS_IN
{
	float4 Pos : POSITION;
	float4 Color : COLOR;
    float2 TexCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    float2 TexCoord : TEXCOORD;
};


VS_OUTPUT VS(VS_IN input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = mul(input.Pos, TransformMatrix);
    output.Pos = mul(output.Pos, CameraMatrix);
    output.Color = input.Color;
    output.TexCoord = input.TexCoord;
    return output;
}



float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 color = 0;
    color = Tx.Sample(Sampler, input.TexCoord);

    return color;
}