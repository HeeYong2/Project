matrix			g_matWorld;
matrix			g_matView;
matrix			g_matProj;

texture			g_BaseTexture;

sampler BaseTexture = sampler_state
{
	Texture = g_BaseTexture;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
};

float4	TransformToProjection(float3 vPosition)
{
	matrix			matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	float4	vPos = mul(vector(vPosition, 1.f), matWVP);

	return vPos;
}