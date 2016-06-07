#include "LightMgr.h"
#include "Light.h"

USING(Engine)

IMPLEMENT_SINGLETON(CLightMgr)

CLightMgr::CLightMgr(void)
{

}

CLightMgr::~CLightMgr(void)
{
	Release();
}

HRESULT CLightMgr::Ready_Light(LPDIRECT3DDEVICE9	pGraphicDev, const D3DLIGHT9* pLightInfo)
{
	CLight*			pLight = CLight::Create(pGraphicDev, pLightInfo, m_LightList.size());

	if(NULL == pLight)
		return E_FAIL;

	m_LightList.push_back(pLight);

	return S_OK;
}

void CLightMgr::Render(LPD3DXEFFECT pEffect)
{
	LIGHTLIST::iterator	iter = m_LightList.begin();
	LIGHTLIST::iterator	iter_end = m_LightList.end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render(pEffect);
	}
}

void CLightMgr::Release(void)
{
	for_each(m_LightList.begin(), m_LightList.end(), Engine::CDelete_Single());
	m_LightList.clear();
}