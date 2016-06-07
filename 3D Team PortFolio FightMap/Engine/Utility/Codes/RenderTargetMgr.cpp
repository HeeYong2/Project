#include "RenderTargetMgr.h"
#include "RenderTarget.h"

USING(Engine)

IMPLEMENT_SINGLETON(CRenderTargetMgr)

CRenderTargetMgr::CRenderTargetMgr(void)
{

}

CRenderTargetMgr::~CRenderTargetMgr(void)
{
	Release();
}

LPDIRECT3DTEXTURE9		CRenderTargetMgr::Get_Texture(const TCHAR* pTargetTag)
{
	CRenderTarget* pRenderTarget = Find_Target(pTargetTag);

	if(NULL == pRenderTarget)
		return NULL;

	return pRenderTarget->Get_Texture();
}

CRenderTarget* CRenderTargetMgr::Find_Target(const TCHAR* pTargetTag)
{
	MAPTARGET::iterator	iter = find_if(m_mapRenderTarget.begin(), m_mapRenderTarget.end(), Engine::CTag_Finder(pTargetTag));

	if(m_mapRenderTarget.end() != iter)
	{
		return iter->second;
	}
	return NULL;
}

list<CRenderTarget*>* CRenderTargetMgr::Find_MRT(const TCHAR* pMRTTag)
{
	MAPMRT::iterator	iter = find_if(m_mapMRT.begin(), m_mapMRT.end(), Engine::CTag_Finder(pMRTTag));

	if(m_mapMRT.end() != iter)
	{
		return &iter->second;
	}
	return NULL;	
}

HRESULT CRenderTargetMgr::Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pTargetTag, const _uint& iSizeX, const _uint& iSizeY, D3DFORMAT Format)
{
	CRenderTarget* pRenderTarget = Find_Target(pTargetTag);

	if(NULL != pRenderTarget)
		return E_FAIL;

	pRenderTarget = CRenderTarget::Create(pGraphicDev, iSizeX, iSizeY, Format);

	if(NULL == pRenderTarget)
		return E_FAIL;

	m_mapRenderTarget.insert(MAPTARGET::value_type(pTargetTag, pRenderTarget));	

	return S_OK;	
}

HRESULT CRenderTargetMgr::Init_DebugBuffer(const TCHAR* pTargetTag, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY)
{
	CRenderTarget* pRenderTarget = Find_Target(pTargetTag);

	if(NULL == pRenderTarget)
		return E_FAIL;

	if(FAILED(pRenderTarget->Init_DebugBuffer(fX, fY, fSizeX, fSizeY)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderTargetMgr::Ready_MRT(const TCHAR* pMRTTag, const TCHAR* pTargetTag)
{
	CRenderTarget* pRenderTarget = Find_Target(pTargetTag);

	if(NULL == pRenderTarget)
		return E_FAIL;

	list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);

	if(NULL == pMRTList)
	{
		list<CRenderTarget*>		MRTList;
		MRTList.push_back(pRenderTarget);	
		m_mapMRT.insert(MAPMRT::value_type(pMRTTag, MRTList));
		return S_OK;
	}	

	pMRTList->push_back(pRenderTarget);

	return S_OK;
}

void CRenderTargetMgr::Begin(const TCHAR* pMRTTag)
{
	list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);

	if(NULL == pMRTList)
		return;

	list<CRenderTarget*>::iterator	iter = pMRTList->begin();
	list<CRenderTarget*>::iterator	iter_end = pMRTList->end();

	_int			iIndex = 0;

	for (; iter != iter_end; ++iter, ++iIndex)
	{
		(*iter)->Begin(iIndex);
	}	
}


void CRenderTargetMgr::End(const TCHAR* pMRTTag)
{
	list<CRenderTarget*>* pMRTList = Find_MRT(pMRTTag);

	if(NULL == pMRTList)
		return;

	list<CRenderTarget*>::iterator	iter = pMRTList->begin();
	list<CRenderTarget*>::iterator	iter_end = pMRTList->end();

	_int			iIndex = 0;

	for (; iter != iter_end; ++iter, ++iIndex)
	{
		(*iter)->End(iIndex);
	}	

}

void CRenderTargetMgr::Render_Debug(void)
{
	MAPTARGET::iterator	iter = m_mapRenderTarget.begin();
	MAPTARGET::iterator	iter_end = m_mapRenderTarget.end();

	for (; iter != iter_end
		; ++iter)
	{
		iter->second->Render();
	}
}

void Engine::CRenderTargetMgr::Release(void)
{
	for_each(m_mapRenderTarget.begin(), m_mapRenderTarget.end(), Engine::CRelease_Pair());
	m_mapRenderTarget.clear();

	m_mapMRT.clear();
}