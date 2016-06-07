#include "Layer.h"
#include "GameObject.h"

USING(Engine)

//---------------------------------------------------------------------------------------
Engine::CLayer::CLayer(void)
{

}
//---------------------------------------------------------------------------------------
Engine::CLayer::~CLayer(void)
{
	Release();
}
//---------------------------------------------------------------------------------------
HRESULT Engine::CLayer::Insert_GameObject(const TCHAR* pObjectTag, CGameObject* pGameObejct)
{
	if(NULL == pGameObejct)
		return E_FAIL;

	MAPLIST::iterator	iter = find_if(m_mapObject.begin(), m_mapObject.end(), CGameObject_Finder(pObjectTag));

	if(iter == m_mapObject.end())
	{
		LISTOBJECT			newList;
		newList.push_back(pGameObejct);
		m_mapObject.insert(MAPLIST::value_type(pObjectTag, newList));
	}
	else
		iter->second.push_back(pGameObejct);

	return S_OK;
}
//---------------------------------------------------------------------------------------

Engine::CComponent* Engine::CLayer::GetObjectComponent(const TCHAR* pObjectTag , const TCHAR* pComponentTag , const _ushort& wCnt)
{
	MAPLIST::iterator iter = find_if(m_mapObject.begin() , m_mapObject.end() , CGameObject_Finder(pObjectTag));

	if(iter == m_mapObject.end())
		return NULL;

	if(wCnt >= iter->second.size())
		return NULL;

	LISTOBJECT::iterator iterList = iter->second.begin();

	for(_ushort i = 0 ; i < wCnt ; ++i)
		++iterList;

	return (*iterList)->GetObjectComponent(pComponentTag);
	
}
//---------------------------------------------------------------------------------------

list<CGameObject*>*	Engine::CLayer::GetCurrentList(const TCHAR* pObjectTag)
{
	MAPLIST::iterator iter = find_if(m_mapObject.begin() , m_mapObject.end() , CGameObject_Finder(pObjectTag));

	if(iter == m_mapObject.end())
		return NULL;

	return &iter->second;
}
//---------------------------------------------------------------------------------------

list<CGameObject*>*	Engine::CLayer::GetList(const TCHAR* pObjectTag)
{
	MAPLIST::iterator iter = find_if(m_mapObject.begin() , m_mapObject.end() , CGameObject_Finder(pObjectTag));

	if(iter == m_mapObject.end())
		return NULL;

	return &iter->second;
}

//---------------------------------------------------------------------------------------
void CLayer::Update(void)
{
	MAPLIST::iterator	iter_map = m_mapObject.begin();
	MAPLIST::iterator	iter_map_end = m_mapObject.end();

	for (; iter_map != iter_map_end; ++iter_map)
	{
		LISTOBJECT::iterator	iter = iter_map->second.begin();
		LISTOBJECT::iterator	iter_end = iter_map->second.end();

		for (; iter != iter_end; ++iter)
		{
			(*iter)->Update();
		}
	}
}
//---------------------------------------------------------------------------------------
void CLayer::Render(void)
{
	MAPLIST::iterator	iter_map = m_mapObject.begin();
	MAPLIST::iterator	iter_map_end = m_mapObject.end();

	for (; iter_map != iter_map_end; ++iter_map)
	{
		LISTOBJECT::iterator	iter = iter_map->second.begin();
		LISTOBJECT::iterator	iter_end = iter_map->second.end();

		for (; iter != iter_end; ++iter)
		{
			(*iter)->Render();
		}
	}
}
//---------------------------------------------------------------------------------------
CLayer* CLayer::Create(void)
{
	CLayer*		pLayer = new CLayer;
	return pLayer;
}
//---------------------------------------------------------------------------------------
void CLayer::Release(void)
{
	MAPLIST::iterator iter_map = m_mapObject.begin();
	MAPLIST::iterator iter_map_end = m_mapObject.end();

	for( ; iter_map != iter_map_end ; ++iter_map)
	{
		LISTOBJECT::iterator iter_list = iter_map->second.begin();
		LISTOBJECT::iterator iter_list_end = iter_map->second.end();

		for( ; iter_list != iter_list_end ; ++iter_list)
		{
			Engine::safe_delete((*iter_list));
		}
		iter_map->second.clear();
	}
	m_mapObject.clear();
}