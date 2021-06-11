cbuffer World
{
	float4x4 ViewProjection;
};

float4 main(float4 pos : Position) : SV_Position
{
	return mul(pos, ViewProjection);
}