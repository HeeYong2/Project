// ������ ������ ����� �ؽ��ĸ� ������ ����
// �ؽ��Ŀ� ���޹��� ���� ������ �����Ѵ�.
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
	vector			vColor : COLOR0; // Light Debug���ۿ� �׸� ����
};

PS_OUT PS_MAIN(float2 vTexUV : TEXCOORD0)
{
	PS_OUT		Out = (PS_OUT)0;

	vector		vOriNormal = tex2D(NormalTexture, vTexUV);

	// ���� ���� �븻�� 0 ~ 1���̷� ǥ���Ǿ��ִ� �̰� ���� �븻�� �ٲ���.
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
