#define MAX_LIGHTS 10


Texture2D texDef : register(t0);
SamplerState splrDef;

Texture2D texSpec : register(t1);
SamplerState splrSpec;

struct Light
{
	float3 pos; // position is relitive to the camera
	float3 color;
	float diffuseIntensity;
	float attConst;
	float attLin;
	float attQuad;
};

cbuffer Lights
{
	const float3 ambient;
	uint numLights;
	Light light[MAX_LIGHTS];
};

struct PS_INPUT
{
	float4 color : COLOR;
	float2 uv : TEXTCOORD;
	float3 worldPos : WORLDPOSITION; // position in the world
	float3 wn : NORMAL; // wrold normal 
	float3 cameraPos : CAMERAPOS; // position relitave to the camera
	float3 cn : CAMERANORMAL; // normal relitave to the camera
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float4 color = texDef.Sample(splrDef, input.uv) * input.color;
	float4 spec = texSpec.Sample(splrSpec, input.uv);

	float3 totalDiffuse = float3(0.0f, 0.0f, 0.0f);
	float3 totalSpec = float3(0.0f, 0.0f, 0.0f);
	
	for (uint i = 0; i < numLights; i++)
	{
		const float3 vToL = light[i].pos - input.cameraPos;
		const float distToL = length(vToL);
		const float3 dirToL = vToL / distToL;
	
		const float att = 1 / (light[i].attConst + (light[i].attLin * distToL) + (light[i].attQuad * distToL * distToL));
	
		const float3 diffuse = light[i].color * light[i].diffuseIntensity * att * max(0.0f, dot(dirToL, input.cn));

		const float3 w = input.cn * dot(vToL, input.cn);
		const float3 r = (w * 2.0f) - vToL;

		const float3 specular = att * (light[i].color * light[i].diffuseIntensity) * spec * pow(max(0.0f, dot(normalize(-r), normalize(input.cameraPos))), 30.0f);
		
		totalDiffuse += diffuse;
		totalSpec += specular;
	}
	
	return float4(saturate((totalDiffuse + ambient + totalSpec) * color.rgb), color.a);
}