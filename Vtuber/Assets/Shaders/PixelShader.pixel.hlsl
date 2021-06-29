#define MAX_LIGHTS 10


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
	uint numLights;
	Light light[MAX_LIGHTS];
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
    float4 texColor = tex.Sample(splr, input.uv) * input.color;
	float3 totalDiffuse = float3(0.0f, 0.0f, 0.0f);
	
	for (uint i = 0; i < numLights; i++)
	{
		const float3 vToL = light[i].pos - input.worldPos;
		const float distToL = length(vToL);
		const float3 dirToL = vToL / distToL;
	
		const float att = 1 / (light[i].attConst + (light[i].attLin * distToL) + (light[i].attQuad * distToL * distToL));
	
		const float3 diffuse = light[i].color * light[i].diffuseIntensity * att * max(0.0f, dot(dirToL, input.n));
		
		totalDiffuse += diffuse;
	}
	
	return float4(saturate((totalDiffuse + ambient) * texColor.rgb), texColor.a);
}