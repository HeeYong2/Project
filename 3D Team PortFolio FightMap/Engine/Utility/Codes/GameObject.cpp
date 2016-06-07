
#include "GameObject.h"

using namespace Engine;
//---------------------------------------------------------------------------------------------------------------------
Engine::CGameObject::CGameObject(void)
{
	ZeroMemory(&m_MtrlInfo, sizeof(m_MtrlInfo));
}
//---------------------------------------------------------------------------------------------------------------------
Engine::CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
{
	ZeroMemory(&m_MtrlInfo, sizeof(m_MtrlInfo));
}

//---------------------------------------------------------------------------------------------------------------------
Engine::CGameObject::~CGameObject(void)
{
	Release();
}
//---------------------------------------------------------------------------------------------------------------------

void Engine::CGameObject::Update(void)
{
	MAPCOM::iterator iter = m_mapComponent.begin();
	MAPCOM::iterator iter_end = m_mapComponent.end();

	for( ; iter != iter_end; ++iter)
	{
		iter->second->Update();
	}
}
//---------------------------------------------------------------------------------------------------------------------

Engine::CComponent* Engine::CGameObject::GetObjectComponent(const TCHAR* pComponentTag)
{
	MAPCOM::iterator 	 iter = find_if(m_mapComponent.begin() , m_mapComponent.end() , CTag_Finder(pComponentTag));

	if(iter == m_mapComponent.end())
		return NULL;

	return iter->second;
}

//---------------------------------------------------------------------------------------------------------------
_ulong CGameObject::Release(void)
{
	_ulong dwRefCnt = Engine::CBase::Release();

	if(0 == dwRefCnt)
	{
		for_each(m_mapComponent.begin(), m_mapComponent.end(), Engine::CDelete_Pair());
		m_mapComponent.clear();
	}

	return dwRefCnt;
}