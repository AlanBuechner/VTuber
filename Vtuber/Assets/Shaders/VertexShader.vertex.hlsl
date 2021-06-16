cbuffer World
{
	float4x4 ViewProjection;
};

cbuffer Model
{
	float4x4 Transform;
};

struct Vertex
{
	float4 pos : POSITION;
	//float2 uv : TEXTCORD;
};

float4 main(Vertex vert) : SV_Position
{
	vert.pos = mul(mul(ViewProjection, Transform), vert.pos);
	return vert.pos;
}