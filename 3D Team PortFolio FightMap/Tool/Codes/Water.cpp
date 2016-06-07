#include "stdafx.h"
#include "Water.h"
#include "Export_Function.h"

CWater::CWater(void)
{

}

CWater::~CWater(void)
{

}

HRESULT CWater::Initialize(void)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pEffect = Engine::Get_Effect(L"Shader_Water");
	return S_OK;
}

void CWater::Update(void)
{
	Engine::CGameObject::Update();
	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_EFFECT , this);
}

void CWater::Render(void)
{
	Set_ConstantTable();

	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);

	m_pBuffer->Render(Engine::GetGraphicDev());

	m_pEffect->EndPass();
	m_pEffect->End();
}

HRESULT CWater::Add_Component(void)
{
	Engine::CComponent*		pComponent = NULL;
	//= For.Transform =======================================================================
	pComponent = m_pInfo = Engine::CTransform::Create(&g_vLook);
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOM::value_type(L"Transform", pComponent));

	//= For.Texture =======================================================================
	pComponent = m_pTexture = Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Water");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOM::value_type(L"Texture_Water", pComponent));

	//= For.Buffer =======================================================================
	pComponent = m_pBuffer = Engine::Clone_Resource(RESOURCE_STAGE, L"RcTex");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOM::value_type(L"Water", pComponent));

	return S_OK;
}

_ulong CWater::Release(void)
{

	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
// 		::safe_delete_array(m_pVtxInfo);
// 		::safe_delete_array(m_pIndex);
		delete this;
	}
	return dwRefCnt;	
}

void CWater::Set_ConstantTable(void)
{
	if(NULL == m_pEffect)
		return;

	m_pEffect->SetMatrix("g_matWorld", &m_pInfo->m_matWorld);

	_matrix			matView, matProj;
	Engine::GetGraphicDev()->GetTransform(D3DTS_VIEW, &matView);
	Engine::GetGraphicDev()->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &matProj);

	IDirect3DBaseTexture9*				pTexture = NULL;

	pTexture = ((Engine::CTexture*)m_pTexture)->Get_Texture();
	m_pEffect->SetTexture("g_BaseSplatting", pTexture);
}
