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
	float3 n : NORMAL;
	float4 color: COLOR;
	float2 uv : TEXTCOORD;
};

struct VS_OUTPUT 
{
	float4 color: COLOR;
	float2 uv : TEXTCOORD;
	float3 n : NORMAL;
	float3 worldPos : WORLDPOSITION;
	float4 pos : SV_POSITION;
};

VS_OUTPUT main(VS_INPUT vert)
{
	VS_OUTPUT output;

	output.pos = mul(mul(ViewProjection, Transform), vert.pos);
	output.uv = vert.uv;
	output.color = vert.color;
	output.worldPos = (float3)mul(Transform, vert.pos);
	output.n = normalize(mul((float3x3) Transform, vert.n));

	return output;
}