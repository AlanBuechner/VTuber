Texture2D tex;
SamplerState splr;

struct Light
{
	float3 pos;
	float3 color;
	float diffuseIntensity;
	float attConst;
	float attLin;
	float attQuad;
};

cbuffer Lights
{
	Light light;
};

struct PS_INPUT
{
	float4 color : COLOR;
	float2 uv : TEXTCOORD;
	float3 n : NORMAL;
	float3 worldPos : WORLDPOSITION;
};

static const float3 ambient = { 0.15f, 0.15f, 0.15f };

float4 main(PS_INPUT input) : SV_TARGET
{
	const float3 vToL = light.pos - input.worldPos;
	const float distToL = length(vToL);
	const float3 dirToL = vToL / distToL;
	
	const float att = 1 / (light.attConst + (light.attLin * distToL) + (light.attQuad * distToL * distToL));
	
	float4 color = tex.Sample(splr, input.uv);
	color = float4(color.b, color.g, color.r, color.a) * input.color;

	const float3 diffuse = light.color * light.diffuseIntensity * att * max(0.0f, dot(dirToL, input.n));
	
	return float4(saturate((diffuse + ambient) * color.rgb), color.a);
}