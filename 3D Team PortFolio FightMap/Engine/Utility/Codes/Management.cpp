#include "Management.h"
#include "Scene.h"

USING(Engine)

IMPLEMENT_SINGLETON(CManagement)

//------------------------------------------------------------------------------------------------------------------------------
CManagement::CManagement(void)
: m_pCurrentScene(NULL)
, m_pRenderer(NULL)
{

}
//------------------------------------------------------------------------------------------------------------------------------
CManagement::~CManagement(void)
{
	Release();
}
//------------------------------------------------------------------------------------------------------------------------------
Engine::CComponent* CManagement::GetObjectComponent(const _int& iLayerID , const TCHAR* pObjectTag , const TCHAR* pComponentTag , const _ushort& wCnt)
{
	if(m_pCurrentScene == NULL
		|| pObjectTag == NULL)
		return NULL;

	return m_pCurrentScene->GetObjectComponent(iLayerID , pObjectTag , pComponentTag , wCnt);
}
void CManagement::Add_RenderGroup(Engine::CRenderer::RENDER_GROUP GroupID, Engine::CGameObject* pGameObject)
{
	if(NULL != m_pRenderer)
		m_pRenderer->Add_RenderGroup(GroupID, pGameObject);
}
//------------------------------------------------------------------------------------------------------------------------------

list<CGameObject*>* CManagement::GetCurrentList(const _int& iLayer, const TCHAR* pObjectTag)
{
	if(m_pCurrentScene == NULL)
		return NULL;

	return m_pCurrentScene->GetCurrentList(iLayer , pObjectTag);
}

//------------------------------------------------------------------------------------------------------------------------------

list<CGameObject*>* CManagement::GetList(const _int& iLayer, const TCHAR* pObjectTag)
{
	return m_pCurrentScene->GetList(iLayer , pObjectTag);
}

//------------------------------------------------------------------------------------------------------------------------------
HRESULT CManagement::Init_Management(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iSizeX, const _uint& iSizeY)
{
	if(m_pRenderer == NULL)
		m_pRenderer = CRenderer::Create(pGraphicDev, iSizeX, iSizeY);

	return S_OK;
}

//------------------------------------------------------------------------------------------------------------------------------
void CManagement::Update(void)
{
	if(NULL != m_pCurrentScene)
		m_pCurrentScene->Update();	
}
//------------------------------------------------------------------------------------------------------------------------------
void CManagement::Render(void)
{
	if(NULL != m_pRenderer)
		m_pRenderer->Render();
}

//------------------------------------------------------------------------------------------------------------------------------
void CManagement::Release(void)
{
	Engine::safe_delete(m_pCurrentScene);
	Engine::safe_delete(m_pRenderer);
}