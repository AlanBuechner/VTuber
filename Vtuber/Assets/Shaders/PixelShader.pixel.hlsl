Texture2D tex;
SamplerState splr;

struct PS_INPUT
{
	float2 uv : TEXTCOORD;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	float4 color = tex.Sample(splr, input.uv);
	color = float4(color.b, color.g, color.r, color.a);

	return color;
}