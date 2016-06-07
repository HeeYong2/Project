#include "Renderer.h"
#include "Scene.h"
#include "GameObject.h"
#include "RenderTargetMgr.h"
#include "ShaderMgr.h"
#include "LightMgr.h"

using namespace Engine;

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Engine::CRenderer::CRenderer(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
{

}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Engine::CRenderer::~CRenderer(void)
{
	Release();
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CRenderer::Init_Renderer(const _uint& iSizeX, const _uint& iSizeY)
{
	// 렌더타겟ㅇ들 생성하겠ㅅ브니다.
	if(FAILED(Engine::CShaderMgr::GetInstance()->Ready_Shader(m_pGraphicDev, L"Shader_Light", L"../../Reference/Headers/Shader_Light.hpp")))
		return E_FAIL;

	// 렌더타겟ㅇ들 생성하겠ㅅ브니다.
	if(FAILED(Engine::CRenderTargetMgr::GetInstance()->Ready_RenderTarget(m_pGraphicDev, L"Albedo", iSizeX, iSizeY, D3DFMT_A16B16G16R16F)))
		return E_FAIL;
	if(FAILED(Engine::CRenderTargetMgr::GetInstance()->Init_DebugBuffer(L"Albedo", 0.f, 0.f, 100.f, 100.f)))		
		return E_FAIL;

	if(FAILED(Engine::CRenderTargetMgr::GetInstance()->Ready_RenderTarget(m_pGraphicDev, L"Normal", iSizeX, iSizeY, D3DFMT_A16B16G16R16F)))
		return E_FAIL;
	if(FAILED(Engine::CRenderTargetMgr::GetInstance()->Init_DebugBuffer(L"Normal", 0.f, 100.f, 100.f, 100.f)))		
		return E_FAIL;

	if(FAILED(Engine::CRenderTargetMgr::GetInstance()->Ready_RenderTarget(m_pGraphicDev, L"Depth", iSizeX, iSizeY, D3DFMT_A16B16G16R16F)))
		return E_FAIL;
	if(FAILED(Engine::CRenderTargetMgr::GetInstance()->Init_DebugBuffer(L"Depth", 0.f, 200.f, 100.f, 100.f)))		
		return E_FAIL;

	if(FAILED(Engine::CRenderTargetMgr::GetInstance()->Ready_RenderTarget(m_pGraphicDev, L"Light", iSizeX, iSizeY, D3DFMT_A16B16G16R16F)))
		return E_FAIL;
	if(FAILED(Engine::CRenderTargetMgr::GetInstance()->Init_DebugBuffer(L"Light", 100.f, 0.f, 100.f, 100.f)))		
		return E_FAIL;


	if(FAILED(Engine::CRenderTargetMgr::GetInstance()->Ready_MRT(L"GBuffer", L"Albedo")))
		return E_FAIL;
	if(FAILED(Engine::CRenderTargetMgr::GetInstance()->Ready_MRT(L"GBuffer", L"Normal")))
		return E_FAIL;
	if(FAILED(Engine::CRenderTargetMgr::GetInstance()->Ready_MRT(L"GBuffer", L"Depth")))
		return E_FAIL;
	if(FAILED(Engine::CRenderTargetMgr::GetInstance()->Ready_MRT(L"LightAcc", L"Light")))
		return E_FAIL;

	if(FAILED(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXSCREEN) * 4, 0, VTXSCREEN_FVF, D3DPOOL_MANAGED, &m_pVB, NULL)))
		return E_FAIL;

	VTXSCREEN*			pVtxScreen = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxScreen, 0);

	pVtxScreen[0].vPosition = _vec4(0.0f, 0.0f, 0.0f, 1.f);
	pVtxScreen[0].vTexUV = _vec2(0.0f, 0.0f);

	pVtxScreen[1].vPosition = _vec4((_float)iSizeX, 0.0f, 0.0f, 1.f);
	pVtxScreen[1].vTexUV = _vec2(1.0f, 0.0f);

	pVtxScreen[2].vPosition = _vec4((_float)iSizeX, (_float)iSizeY, 0.0f, 1.f);
	pVtxScreen[2].vTexUV = _vec2(1.0f, 1.0f);

	pVtxScreen[3].vPosition = _vec4(0.0f, (_float)iSizeY, 0.0f, 1.f);
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
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CRenderer::Render(void)
{
// 	m_pGraphicDev->Clear(0 , NULL , D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL ,D3DCOLOR_ARGB(255 , 0 , 0 , 255) , 1.f , 0);
// 	m_pGraphicDev->BeginScene();

	Render_Deferred();
	Render_Light();
// #ifdef _DEBUG
// 	Engine::CRenderTargetMgr::GetInstance()->Render_Debug();
// #endif

// 	m_pGraphicDev->EndScene();
// 	m_pGraphicDev->Present(NULL , NULL , NULL , NULL);
	Clear_RenderGroup();	
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CRenderer::Render_Light(void)
{
	Engine::CRenderTargetMgr::GetInstance()->Begin(L"LightAcc");

	LPD3DXEFFECT pEffect = Engine::CShaderMgr::GetInstance()->Get_Effect(L"Shader_Light");

	if(NULL == pEffect)
		return;

	LPDIRECT3DTEXTURE9 pTexture = Engine::CRenderTargetMgr::GetInstance()->Get_Texture(L"Normal");
	if(NULL == pTexture)
		return;

	pEffect->SetTexture("g_NormalTexture", pTexture);

	pEffect->Begin(NULL, 0);	

	Engine::CLightMgr::GetInstance()->Render(pEffect);

	pEffect->End();

	Engine::CRenderTargetMgr::GetInstance()->End(L"LightAcc");

	pEffect->SetTexture("g_NormalTexture", NULL);			//Texture를 사용한 다음에 NULL로 다시 채워주자

}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void CRenderer::Render_Deferred(void)
{
	CRenderTargetMgr::GetInstance()->Begin(L"GBuffer");
	
	RENDERLIST::iterator	iter = m_RenderGroup[RENDER_DEFERRED].begin();
	RENDERLIST::iterator	iter_end = m_RenderGroup[RENDER_DEFERRED].end();

	for (; iter != iter_end; ++iter)
	{
		(*iter)->Render();
	}

	CRenderTargetMgr::GetInstance()->End(L"GBuffer");
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CRenderer* Engine::CRenderer::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iSizeX, const _uint& iSizeY)
{
	CRenderer*	pRenderer = new CRenderer(pGraphicDev);

	if(FAILED(pRenderer->Init_Renderer(iSizeX, iSizeY)))
		Engine::safe_delete(pRenderer);

	return pRenderer;
}

void Engine::CRenderer::Clear_RenderGroup(void)
{
	for (_int i = 0; i < RENDER_END; ++i)
	{
		m_RenderGroup[i].clear();
	}
}

void CRenderer::Release(void)
{
	Engine::safe_release(m_pVB);
	Engine::safe_release(m_pIB);
}