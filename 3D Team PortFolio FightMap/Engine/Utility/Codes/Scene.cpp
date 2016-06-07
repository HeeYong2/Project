#include "Scene.h"

using namespace Engine;

//------------------------------------------------------------------------------------------------------------------------
Engine::CScene::CScene(void)
: m_pGraphicDev(NULL)
{

}
//------------------------------------------------------------------------------------------------------------------------
CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
{

}

//------------------------------------------------------------------------------------------------------------------------
Engine::CScene::~CScene(void)
{
	Release();
}
//------------------------------------------------------------------------------------------------------------------------
Engine::CComponent* Engine::CScene::GetObjectComponent(const _int& iLayerID , const TCHAR* pObjectTag , const TCHAR* pComponentTag , const _ushort& wCnt)
{
	MAPLAYER::iterator iter = m_mapLayer.find(iLayerID);

	if(iter == m_mapLayer.end())
		return NULL;

	return iter->second->GetObjectComponent(pObjectTag , pComponentTag , wCnt);
}
//------------------------------------------------------------------------------------------------------------------------

list<CGameObject*>* Engine::CScene::GetCurrentList(const _int& iLayer, const TCHAR* pObjectTag)
{
	MAPLAYER::iterator iter = m_mapLayer.find(iLayer);

	if(iter == m_mapLayer.end())
		return NULL;

	return iter->second->GetCurrentList(pObjectTag);

}

//------------------------------------------------------------------------------------------------------------------------

list<CGameObject*>* Engine::CScene::GetList(const _int& iLayer, const TCHAR* pObjectTag)
{
	MAPLAYER::iterator iter = m_mapLayer.find(iLayer);

	if(iter == m_mapLayer.end())
		return NULL;

	return iter->second->GetList(pObjectTag);

}

//------------------------------------------------------------------------------------------------------------------------
void Engine::CScene::Render(void)
{
	MAPLAYER::iterator iter = m_mapLayer.begin();
	MAPLAYER::iterator iter_end = m_mapLayer.end();

	for(; iter != iter_end; ++iter)
	{
		(iter->second)->Render();
	}
}
//------------------------------------------------------------------------------------------------------------------------
void Engine::CScene::Update(void)
{
	MAPLAYER::iterator iter = m_mapLayer.begin();
	MAPLAYER::iterator iter_end = m_mapLayer.end();

	for(; iter != iter_end; ++iter)
	{
		(iter->second)->Update();
	}
}

//------------------------------------------------------------------------------------------------------------------------
void CScene::Release(void)
{
	for_each(m_mapLayer.begin(), m_mapLayer.end(), CDelete_Pair());
	m_mapLayer.clear();
}