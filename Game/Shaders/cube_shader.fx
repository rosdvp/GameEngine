cbuffer ConstantBuffer
{
	matrix World;
	matrix View;
	matrix Projection;
}

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
};


VS_OUTPUT VS(float4 pos : POSITION, float4 color : COLOR)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = mul(pos, World );
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    output.Color = color;
    return output;
}



float4 PS(VS_OUTPUT input) : SV_Target
{
    return input.Color;
}