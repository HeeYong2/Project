#include "stdafx.h"
#include "SkyBox.h"
#include "Export_Function.h"
#include "..\..\Engine\Resource\Codes\StaticMesh.h"

//---------------------------------------------------------------------------------------------------------------------------------------

CSkyBox::CSkyBox(void)
: m_iSkyNum(0)
, m_fAngle(0.f)
{

}

//---------------------------------------------------------------------------------------------------------------------------------------

CSkyBox::~CSkyBox(void)
{

}

//---------------------------------------------------------------------------------------------------------------------------------------


HRESULT CSkyBox::Initialize(CString strSkyName)
{
	if(FAILED(AddComponent(strSkyName)))
		return E_FAIL;

	m_pEffect = Engine::Get_Effect(L"Shader_SkyBox");

	if(NULL == m_pEffect)
		return E_FAIL;
	
	m_pInfo->m_vPosition = _vec3(400.f, -60.f, 250.f);
	m_pInfo->m_vScale = _vec3(4.f , 4.f , 4.f);

	return S_OK;
}

//---------------------------------------------------------------------------------------------------------------------------------------
void CSkyBox::Render(void)
{
	SetConstantTable();

	m_pEffect->Begin(NULL , 0);
	m_pEffect->BeginPass(0);

	((Engine::CMesh*)m_pMeshCom)->Render(Engine::GetGraphicDev() , &m_pInfo->m_matWorld );

	m_pEffect->EndPass();
	m_pEffect->End();
}

//---------------------------------------------------------------------------------------------------------------------------------------

void CSkyBox::Updata(void)
{
	Engine::CGameObject::Update();
	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_EFFECT, this);
}
//---------------------------------------------------------------------------------------------------------------------------------------
CSkyBox * CSkyBox::Create(CString strSkyName)
{
	CSkyBox * pSkyBox = new CSkyBox;

	if(FAILED((pSkyBox->Initialize(strSkyName))))
	{
		::safe_delete(pSkyBox);
		msg_box("pSky Box Create Failed");
		return NULL;
	}
	return pSkyBox;
}

//---------------------------------------------------------------------------------------------------------------------------------------

void CSkyBox::SetConstantTable(void)
{
	if(NULL == m_pEffect)
		return;

	_matrix		matRot , matPos;

	m_fAngle += (1.f * Engine::GetTime());

	D3DXMatrixRotationY(&matRot , D3DXToRadian(m_fAngle));
	D3DXMatrixTranslation(&matPos , m_pInfo->m_vPosition.x , m_pInfo->m_vPosition.y , m_pInfo->m_vPosition.z); 

	m_pInfo->m_matWorld = matRot * matPos;
	m_pEffect->SetMatrix("g_matWorld" , &m_pInfo->m_matWorld);

	_matrix		matView, matProj;
	
	Engine::GetGraphicDev()->GetTransform(D3DTS_VIEW , &matView);
	Engine::GetGraphicDev()->GetTransform(D3DTS_PROJECTION , &matProj);

	m_pEffect->SetMatrix("g_matView" , &matView);
	m_pEffect->SetMatrix("g_matProj" , &matProj);
}

//---------------------------------------------------------------------------------------------------------------------------------------

HRESULT CSkyBox::AddComponent(CString strSkyName)
{
	Engine::CComponent*		pComponent = NULL;
	//= For.Transform =======================================================================
	pComponent = m_pInfo = Engine::CTransform::Create(&g_vLook);
	if(pComponent == NULL )
		return E_FAIL;
	m_mapComponent.insert(MAPCOM::value_type(L"Transform", pComponent));

	//= For.Mesh=======================================================================
	pComponent = m_pMeshCom = Engine::Clone_Resource(RESOURCE_STAGE , strSkyName);
	if(pComponent == NULL )
		return E_FAIL;
	m_mapComponent.insert(MAPCOM::value_type(strSkyName, pComponent));
	return S_OK;
}

void CSkyBox::SetSkyNum(_int iSkyNum)
{
	m_iSkyNum = iSkyNum;
}

_ulong CSkyBox::Release(void)
{
	_ulong dwReCnt = Engine::CGameObject::Release();

	if(0 == dwReCnt)
	{
		delete this;
	}
	return dwReCnt;
}