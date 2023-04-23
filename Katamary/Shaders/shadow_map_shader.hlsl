cbuffer ShadowBuffer : register(b0)
{
	matrix LightViewProjMatrix;
}

cbuffer TransformBuffer : register(b1)
{
	matrix TransformMatrix;
}

struct VS_IN
{
	float4 Pos : POSITION;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 DepthPos : TEXTURE0;
};


VS_OUTPUT VS(VS_IN input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = mul(input.Pos, TransformMatrix);
    output.Pos = mul(output.Pos, LightViewProjMatrix);
    return output;
}



float4 PS(VS_OUTPUT input) : SV_Target 
{
	float depth = input.DepthPos.z / input.DepthPos.w;

	return float4(depth, depth, depth, 1.0f);
}