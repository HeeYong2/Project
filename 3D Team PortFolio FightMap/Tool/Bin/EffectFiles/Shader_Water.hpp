#include "Shader_Base.hpp"

float4			g_WorldLightPosition;
float4			g_WorldCameraPosition;

float			g_Time;
float			g_LightColor;
texture			g_DiffuseTexture;
texture			g_SpecularTexture;

sampler DiffuseTexture = sampler_state
{
	Texture = g_DiffuseTexture;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

sampler SpecularTexture = sampler_state
{
	Texture = g_SpecularTexture;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

struct VS_IN
{
	vector vPosition : POSITION;
	float3 vNormal : NORMAL;
	float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
	vector	vPosition : POSITION;
	float2  vTexUV	: TEXCOORD0;
	float3  vDiffuse : TEXCOORD1;
	float3	vViewDir : TEXCOORD2;
	float3  vReflection : TEXCOORD3;
};

VS_OUT VS_MAIN(VS_IN Input)
{
	VS_OUT Out = VS_OUT(0);

	Out.vPosition = mul(Input.vPosition , g_matWorld);

	float3 fLightDir = Out.vPosition.xyz - g_WorldLightPosition.xyz;
	fLightDir = normalize(fLightDir);

	float3 ViewDir = normalize(Out.vPosition->xyz - g_WorldCameraPosition.xyz);
	Out.vViewDir = ViewDir;

	Out.vPosition = mul(Out.vPosition , g_matView);
	Out.vPosition = mul(Out.vPosition , g_matProj);

	float3 WorldNormal = mul(Input.vNormal , float3(g_matWorld));
	WorldNormal = normalize(WorldNormal);

	Out.vDiffuse = dot(-fLightDir , WorldNormal);
	Out.vReflection = reflect(fLightDir , WorldNormal);

	Out.vTexUV = Input.vTexUV + float2(g_Time * 0.25f , 0);

	return Out;
}

struct PS_IN
{
	float2 vTexUV : TEXCOORD0;
	float3 vDiffuse : TEXCOORD1;
	float3 vViewDir : TEXCOORD2;
	float3 vReflection : TEXCOORD3;
};

struct PS_OUT // 픽셀셰이딩을 마치고 난 이후의 결과물.
{	
	vector			vAlbedo : COLOR0;
	vector			vNormal : COLOR1;	
	vector			vDepth : COLOR2;
};

PS_OUT PS_MAIN(PS_IN Input)
{
	PS_OUT Out = PS_OUT(0);

	Out.vAlbedo = tex2D(g_DiffuseTexture , Input.vTexUV);
	float3 fDiffuse = g_LightColor * Out.vAlbedo.rgb * saturate(Input.vDiffuse);

	float3 vReflection	= normalize(Input.vReflection);
	float3 vViewDir = normalize(Input.vViewDir);
	float3 vSpecular = 0;

	if(fDiffuse.x > 0)
	{
		specular = saturate(dot(vReflection , -vViewDir));
		specular = pow(specular , 20.f);

		vector vSpecularIntensity = tex2D(SpecularTexture , Input.vTexUV);
		specular *= specularIntensity.rgb * g_LightColor;
	}

	float3 vAmbient = float3(0.1f , 0.1f, 0.1f) * Out.vAlbedo;
	
	vector vResult = vector(vAmbient + fDiffuse + specular , 1);

	Out.vAlbedo = vResult;

	return Out.vAlbedo;
	
}