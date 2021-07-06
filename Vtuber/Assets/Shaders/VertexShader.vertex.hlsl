cbuffer Camera
{
	float4x4 View;
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
	float3 worldPos : WORLDPOSITION;
	float3 wn : NORMAL;
	float3 cameraPos : CAMERAPOS;
	float3 cn : CAMERANORMAL;
	float4 pos : SV_POSITION;
};

VS_OUTPUT main(VS_INPUT vert)
{
	VS_OUTPUT output;

	float4x4 mvp = mul(ViewProjection, Transform);
	float4x4 mv = mul(View, Transform);

	output.color = vert.color;
	output.uv = vert.uv;
	output.worldPos = (float3)mul(Transform, vert.pos);
	output.wn = normalize(mul((float3x3) Transform, vert.n));
	output.cameraPos = (float3)mul(mv, vert.pos);
	output.cn = normalize(mul((float3x3) mv, vert.n));
	output.pos = mul(mvp, vert.pos);

	return output;
}