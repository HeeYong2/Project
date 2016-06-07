#include "Shader_Base.hpp"

texture			g_BaseSplatting;
texture			g_Mix1Texture;
texture			g_Mix2Texture;
texture			g_Mix3Texture;
texture			g_Mix4Texture;


texture			g_Mix1AlphaTexture;
texture			g_Mix2AlphaTexture;
texture			g_Mix3AlphaTexture;
texture			g_Mix4AlphaTexture;

float			g_fDetail;
float			g_Mix1Ratio;
float			g_Mix2Ratio;
float			g_Mix3Ratio;
float			g_Mix4Ratio;


sampler BaseSplatting = sampler_state
{
	Texture = g_BaseSplatting;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

sampler Mix1Texture = sampler_state
{
	Texture = g_Mix1Texture;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};


sampler Mix2Texture = sampler_state
{
	Texture = g_Mix2Texture;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};


sampler Mix3Texture = sampler_state
{
	Texture = g_Mix3Texture;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

sampler Mix4Texture = sampler_state
{
	Texture = g_Mix4Texture;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};


sampler Mix1AlphaTexture = sampler_state
{
	Texture = g_Mix1AlphaTexture;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};


sampler Mix2AlphaTexture = sampler_state
{
	Texture = g_Mix2AlphaTexture;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

sampler Mix3AlphaTexture = sampler_state
{
	Texture = g_Mix3AlphaTexture;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

sampler Mix4AlphaTexture = sampler_state
{
	Texture = g_Mix4AlphaTexture;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};


struct VS_IN
{
	vector	vPosition : POSITION;
	float3	vNormal : NORMAL;
	float2	vTexUV : TEXCOORD0;
	float2	vBaseSplatting : TEXCOORD1;
	float2	vMix1Texture : TEXCOORD2;
	float2	vMix2Texture : TEXCOORD3;
	float2	vMix3Texture : TEXCOORD4;
	float2	vMix1AlphaTexture : TEXCOORD5;
	float2	vMix2AlphaTexture : TEXCOORD6;
	float2	vMix3AlphaTexture : TEXCOORD7;

};

struct VS_OUT
{
	vector	vPosition : POSITION;	
	float2	vTexUV : TEXCOORD0;	
	float3	vNormal : TEXCOORD1;	
	vector	vProjPos : TEXCOORD2;
	vector	vWorldPos : TEXCOORD3;
};

VS_OUT VS_MAIN(VS_IN Input)	// 정점의 변환
{
	VS_OUT			Out = (VS_OUT)0;

	float3		vPos = Input.vPosition.xyz;

	Out.vPosition = TransformToProjection(Input.vPosition.xyz);

	Out.vTexUV = Input.vTexUV;

	matrix			matWV;
	matWV = mul(g_matWorld, g_matView);

	Out.vNormal = normalize(mul(vector(Input.vNormal, 0.f), matWV).xyz);

	Out.vProjPos = Out.vPosition;	

	Out.vWorldPos = mul(float4(vPos, 1.f), g_matWorld);	
	return Out;	
}

struct PS_IN
{	
	float2			vTexUV : TEXCOORD0;
	float3			vNormal : TEXCOORD1;
	vector			vPojPos : TEXCOORD2;
	vector			vWorldPos : TEXCOORD3;

};

struct PS_OUT // 픽셀셰이딩을 마치고 난 이후의 결과물.
{	
	vector			vAlbedo : COLOR0;
	vector			vNormal : COLOR1;	
	vector			vDepth : COLOR2;
};

PS_OUT PS_MAIN(PS_IN Input)
{
	PS_OUT			Out = (PS_OUT)0;	


	float2	vDetailUV = Input.vTexUV * g_fDetail;
	float2	vMix1UV = Input.vTexUV * g_Mix1Ratio;
	float2	vMix2UV = Input.vTexUV * g_Mix2Ratio;
	float2	vMix3UV = Input.vTexUV * g_Mix3Ratio;
	float2	vMix4UV = Input.vTexUV * g_Mix4Ratio;

	vector vColor[9];

	vColor[0] = tex2D(BaseSplatting    , vDetailUV);			//베이스 텍스처
	vColor[1] = tex2D(Mix1Texture 	   , vMix1UV);				//재료 1		tex2D(전역변수는 const형태다 값을 수정 할 수 없다 )
	vColor[2] = tex2D(Mix2Texture 	   , vMix2UV);				//재료 2
	vColor[3] = tex2D(Mix3Texture 	   , vMix3UV);				//재료 3
	vColor[4] = tex2D(Mix4Texture 	   , vMix4UV);				//재료 4
	vColor[5] = tex2D(Mix1AlphaTexture , Input.vTexUV);			//알파 1
	vColor[6] = tex2D(Mix2AlphaTexture , Input.vTexUV);			//알파 2
	vColor[7] = tex2D(Mix3AlphaTexture , Input.vTexUV);			//알파 3
	vColor[8] = tex2D(Mix4AlphaTexture , Input.vTexUV);			//알파 4



	vector	vOne	= vColor[1] * vColor[5] + (vColor[0] * (1.f - vColor[5])) ;
	vector	vTwo	= vColor[2] * vColor[6] + (vOne * (1.f - vColor[6])) ;
	vector	vThree	= vColor[3] * vColor[7] + (vTwo * (1.f - vColor[7]));
	vector	vFour	= vColor[4] * vColor[8] + (vThree * (1.f - vColor[8]));
	Out.vAlbedo = (vFour ) ;	//알파값을 비교해서 가장큰놈을 넣어주자

	Out.vNormal = float4(Input.vNormal * 0.5f + 0.5f, 0.f);

	return Out;
}

technique TerrainEffect
{
	pass Default
	{			
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};