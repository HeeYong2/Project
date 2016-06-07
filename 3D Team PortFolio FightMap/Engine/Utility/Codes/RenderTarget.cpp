#include "RenderTarget.h"

USING(Engine)

Engine::CRenderTarget::CRenderTarget( LPDIRECT3DDEVICE9 pGraphicDev )
: m_pGraphicDev(pGraphicDev)
, m_pTargetTexture(NULL)
, m_pTargetSurface(NULL)
, m_pOldSurface(NULL)
, m_pVB(NULL)
, m_pIB(NULL)
{

}

Engine::CRenderTarget::~CRenderTarget( void )
{

}

HRESULT Engine::CRenderTarget::Init_RenderTarget(const _uint& iSizeX, const _uint& iSizeY, D3DFORMAT Format)
{	
	if(FAILED(D3DXCreateTexture(m_pGraphicDev, iSizeX, iSizeY, 1, D3DUSAGE_RENDERTARGET, Format, D3DPOOL_DEFAULT, &m_pTargetTexture)))
	{
		msg_box("D3DXCreateTexture For RenderTarget Failed");
		return E_FAIL;
	}

	m_pTargetTexture->GetSurfaceLevel(0, &m_pTargetSurface);
	m_pTargetSurface->Release();

	if(NULL == m_pTargetSurface)
		return E_FAIL;

	return S_OK;
}

HRESULT Engine::CRenderTarget::Init_DebugBuffer(const _float& fX, const _float& fY
												, const _float& fSizeX, const _float& fSizeY)
{
	if(FAILED(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXSCREEN) * 4, 0, VTXSCREEN_FVF, D3DPOOL_MANAGED, &m_pVB, NULL)))
		return E_FAIL;

	VTXSCREEN*			pVtxScreen = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxScreen, 0);

	pVtxScreen[0].vPosition = _vec4(fX, fY, 0.0f, 1.f);
	pVtxScreen[0].vTexUV = _vec2(0.0f, 0.0f);

	pVtxScreen[1].vPosition = _vec4(fX + fSizeX, fY, 0.0f, 1.f);
	pVtxScreen[1].vTexUV = _vec2(1.0f, 0.0f);

	pVtxScreen[2].vPosition = _vec4(fX + fSizeX, fY + fSizeY, 0.0f, 1.f);
	pVtxScreen[2].vTexUV = _vec2(1.0f, 1.0f);

	pVtxScreen[3].vPosition = _vec4(fX, fY + fSizeY, 0.0f, 1.f);
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

void Engine::CRenderTarget::Begin(const _ulong& dwIndex)
{
	m_pGraphicDev->GetRenderTarget(dwIndex, &m_pOldSurface);	

	m_pGraphicDev->SetRenderTarget(dwIndex, m_pTargetSurface);	

	m_pGraphicDev->Clear(0, NULL, D3DCLEAR_TARGET, D3DXCOLOR(0.f, 0.f, 0.f, 0.f), 1.f, 0);
}

void Engine::CRenderTarget::End(const _ulong& dwIndex)
{
	m_pGraphicDev->SetRenderTarget(dwIndex, m_pOldSurface);

	Engine::safe_release(m_pOldSurface);
}

CRenderTarget* Engine::CRenderTarget::Create( LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iSizeX, const _uint& iSizeY, D3DFORMAT Format )
{
	CRenderTarget* pRenderTarget = new CRenderTarget(pGraphicDev);

	if(FAILED(pRenderTarget->Init_RenderTarget(iSizeX, iSizeY, Format)))
		Engine::safe_release(pRenderTarget);
	
	return pRenderTarget;
}

void CRenderTarget::Render(void)
{
	if(NULL == m_pVB
		|| NULL == m_pIB)
		return;
	m_pGraphicDev->SetTexture(0, m_pTargetTexture);
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	m_pGraphicDev->SetFVF(VTXSCREEN_FVF);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

_ulong CRenderTarget::Release(void)
{
	_ulong RefCnt = CBase::Release();

	if(0 == RefCnt)
	{
		if(Engine::safe_release(m_pTargetSurface))		//Texture를 사용한 다음에 NULL로 다시 채워주자
			msg_box("m_pTargetSurface safe_release Failed");
		if(Engine::safe_release(m_pOldSurface))
			msg_box("m_pOldSurface safe_release Failed");
		if(Engine::safe_release(m_pVB))
			msg_box("m_pVB safe_release Failed");
		if(Engine::safe_release(m_pIB))
			msg_box("m_pIB safe_release Failed");

		delete this;
	}

	return RefCnt;	
}