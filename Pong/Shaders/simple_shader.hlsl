cbuffer CameraBuffer : register(b0)
{
	matrix CameraMatrix;
}

cbuffer TransformBuffer : register(b1)
{
	matrix TransformMatrix;
}


struct VS_IN
{
	float4 Pos : POSITION0;
	float4 Color : COLOR0;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
};


VS_OUTPUT VS(VS_IN input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = mul(input.Pos, TransformMatrix);
    output.Pos = mul(output.Pos, CameraMatrix);
    output.Color = input.Color;
    return output;
}



float4 PS(VS_OUTPUT input) : SV_Target
{
    return input.Color;
}