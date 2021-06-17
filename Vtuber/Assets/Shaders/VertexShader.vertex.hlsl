cbuffer World
{
	float4x4 ViewProjection;
};

cbuffer Model
{
	float4x4 Transform;
};

struct VS_INPUT
{
	float4 pos : POSITION;
	float2 uv : TEXTCOORD;
};

struct VS_OUTPUT 
{
	float2 uv : TEXTCOORD;
	float4 pos : SV_POSITION;
};

VS_OUTPUT main(VS_INPUT vert)
{
	VS_OUTPUT output;

	output.pos = mul(mul(ViewProjection, Transform), vert.pos);
	output.uv = vert.uv;

	return output;
}