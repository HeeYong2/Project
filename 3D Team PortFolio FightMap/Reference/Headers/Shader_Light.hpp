// 법선의 정보가 저장된 텍스쳐를 가지고 오고
// 텍스쳐와 전달받은 빛의 정보를 연산한다.
texture				g_NormalTexture;
vector				g_LightDir;
vector				g_LightDiffuse;

sampler NormalTexture = sampler_state
{
	Texture = g_NormalTexture;
	MipFilter = POINT;
	MinFilter = POINT;
	MagFilter = POINT;
};

struct PS_OUT
{
	vector			vColor : COLOR0; // Light Debug버퍼에 그릴 색들
};

PS_OUT PS_MAIN(float2 vTexUV : TEXCOORD0)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vOriNormal = tex2D(NormalTexture, vTexUV);

	// 현재 얻어온 노말은 0 ~ 1사이로 표현되어있다 이걸 원래 노말로 바꾸자.
	float3		vNormal;

	vNormal.x = vOriNormal.x * 2.f - 1.f;
	vNormal.y = vOriNormal.y * 2.f - 1.f;
	vNormal.z = vOriNormal.z * 2.f - 1.f;

	float3 vLightDir = normalize(-g_LightDir).xyz;	

	Out.vColor = max(dot(vNormal, vLightDir), 0.f);
	//Out.vColor = 1.f;

	return Out;
}

technique LightAcc
{
	pass Directional
	{
		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN();
	}
};
