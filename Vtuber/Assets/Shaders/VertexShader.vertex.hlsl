cbuffer CBuff : register(b0)
{
	float4x4 transform;
	float val = 1.0f;
};

float4 main(float3 pos : Position) : SV_Position
{
	return mul(float4(pos.x, pos.y + val, pos.z, 1.0f), transform);
}