
#include "Light.h"

USING(Engine)

CLight::CLight(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
{

}

CLight::~CLight(void)
{

}

HRESULT	CLight::Init_Light(const D3DLIGHT9* pLightInfo, const _int& iIdx)
{
	m_iIndex = iIdx;

	memcpy(&m_Light, pLightInfo, sizeof(D3DLIGHT9));

	if(FAILED(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXSCREEN) * 4, 0, VTXSCREEN_FVF, D3DPOOL_MANAGED, &m_pVB, NULL)))
		return E_FAIL;

	VTXSCREEN*			pVtxScreen = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxScreen, 0);

	pVtxScreen[0].vPosition = _vec4(0.f, 0.f, 0.0f, 1.f);
	pVtxScreen[0].vTexUV = _vec2(0.0f, 0.0f);

	pVtxScreen[1].vPosition = _vec4(800.f, 0.f, 0.0f, 1.f);
	pVtxScreen[1].vTexUV = _vec2(1.0f, 0.0f);

	pVtxScreen[2].vPosition = _vec4(800.0f, 600.0f, 0.0f, 1.f);
	pVtxScreen[2].vTexUV = _vec2(1.0f, 1.0f);

	pVtxScreen[3].vPosition = _vec4(0.0f, 600.f, 0.0f, 1.f);
	pVtxScreen[3].vTexUV = _vec2(0.0f, 1.0f);

	m_pVB->Unlock();

	if(FAILED(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 2, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIB, NULL)))
		return E_FAIL;

	INDEX16*			pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._1 = 0;
	pIndex[0]._2 = 1;
	pIndex[0]._3 = 2;

	pIndex[1]._1 = 0;
	pIndex[1]._2 = 2;
	pIndex[1]._3 = 3;

	m_pIB->Unlock();

	return S_OK;
}

void CLight::Render(LPD3DXEFFECT pEffect)
{	
	pEffect->SetVector("g_LightDir", &_vec4(m_Light.Direction.x, m_Light.Direction.y, m_Light.Direction.z, 0.f));
	pEffect->SetVector("g_LightDiffuse", &_vec4(m_Light.Diffuse.r, m_Light.Diffuse.g, m_Light.Diffuse.b, m_Light.Diffuse.a));
	
	pEffect->CommitChanges();

	pEffect->BeginPass(0);
	
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	m_pGraphicDev->SetFVF(VTXSCREEN_FVF);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);	

	pEffect->EndPass();
}

CLight* CLight::Create(LPDIRECT3DDEVICE9	pGraphicDev, const D3DLIGHT9* pLightInfo, const _int& iIdx )
{
	CLight*		pLight = new CLight(pGraphicDev);

	if(FAILED(pLight->Init_Light(pLightInfo, iIdx)))
	{
		Engine::safe_delete(pLight);
		return NULL;
	}
	return pLight;
}