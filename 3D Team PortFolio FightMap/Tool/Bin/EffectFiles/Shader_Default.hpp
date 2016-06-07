#include "Shader_Base.hpp"

struct VS_IN
{
	vector vPosition	:	POSITION;
	float2 vTexUV		:	TEXCOORD0;
};

struct VS_OUT
{
	vector vPosition	:	POSITION;
	float2 vTexUV		:	TEXCOORD0;
	vector vProjPos		:	TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN Input)
{
	VS_OUT		Out = (VS_OUT)0;
	Out.vPosition = TransformToProjection(Input.vPosition.xyz);
	Out.vTexUV = Input.vTexUV;
	Out.vProjPos = Out.vProjPos;

	return Out;
}

struct PS_IN
{
	float2		vTexUV		:	TEXCOORD0;
	vector		vProjPos	:	TEXCOORD1;
};

struct PS_OUT
{
	vector		vAlbedo		:	COLOR0;
};

PS_OUT PS_MAIN(PS_IN Input)
{
	PS_OUT		Out = (PS_OUT)0;
	float4 fColor = tex2D(BaseTexture , Input.vTexUV);
	clip(fColor.a - 0.5f);

	Out.vAlbedo = fColor;
	return Out;
}

technique EnvironmentSky
{
	pass SkyAlbedo
	{
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}