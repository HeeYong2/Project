#include "stdafx.h"
#include "NaviFlag.h"
#include "Export_Function.h"

CNaviFlag::CNaviFlag(void)
: m_isRender(false)
, m_iTexture(0)
{

}

CNaviFlag::~CNaviFlag(void)
{

}

HRESULT CNaviFlag::Initialize(void)
{
	if(FAILED(AddComponent()))
		return E_FAIL;
	m_pInfo->m_vScale = _vec3(0.5f , 0.5f , 0.5f);

	return S_OK;
}

void CNaviFlag::Render(void)
{
	if(m_isRender == true)
	{
		Render_Texture();
		Render_Buffer();
	}
}

void CNaviFlag::Update(void)
{
	Engine::CGameObject::Update();
}

CNaviFlag* CNaviFlag::Create(void)
{
	CNaviFlag*	pNaviFlag = new CNaviFlag;

	if(FAILED(pNaviFlag->Initialize()))
	{
		::Engine::safe_delete(pNaviFlag);
		return NULL;
	}
	return pNaviFlag;
}

HRESULT CNaviFlag::AddComponent(void)
{
	Engine::CComponent*	pComponent = NULL;

	//For. Transform//////////////////////////////////////////
	pComponent = m_pInfo = Engine::CTransform::Create(&g_vLook);
	if(pComponent == NULL)
		return E_FAIL;

	m_mapComponent.insert(MAPCOM::value_type(L"Transform" , pComponent));


	//For. Texture//////////////////////////////////////////
	pComponent = m_pTexterCom = Engine::Clone_Resource(RESOURCE_STAGE , L"Texture_Flag");
	if(pComponent == NULL)
		return E_FAIL;

	m_mapComponent.insert(MAPCOM::value_type(L"Texture_Flag" , pComponent));

	//For. Buffer//////////////////////////////////////////
	pComponent = m_pBufferCom = Engine::Clone_Resource(RESOURCE_STATIC, L"CubeTex");		//RcTex로 하면  그냥 사각형임
	if(NULL == pComponent)
		return E_FAIL;

	m_mapComponent.insert(MAPCOM::value_type(L"CubeTex", pComponent));
	
	return S_OK;
}

void CNaviFlag::Render_Texture(void)
{
	_matrix matRotate , matTrans ,matScale;

	m_pInfo->m_fAngle[m_pInfo->ANGLE_X] += 5.f * Engine::GetTime();
	D3DXMatrixScaling(&matScale , m_pInfo->m_vScale.x , m_pInfo->m_vScale.y , m_pInfo->m_vScale.z);
	D3DXMatrixRotationY(&matRotate , m_pInfo->m_fAngle[m_pInfo->ANGLE_X] );
	D3DXMatrixTranslation(&matTrans , m_pInfo->m_vPosition.x , m_pInfo->m_vPosition.y , m_pInfo->m_vPosition.z);
	m_pInfo->m_matWorld = matScale * matRotate * matTrans;
	Engine::GetGraphicDev()->SetTransform(D3DTS_WORLD , &m_pInfo->m_matWorld);
	((Engine::CTexture*)m_pTexterCom)->Render(Engine::GetGraphicDev(), m_iTexture);
}

void CNaviFlag::Render_Buffer(void)
{
	((Engine::CVIBuffer*)m_pBufferCom)->Render(Engine::GetGraphicDev());
}

void CNaviFlag::SetPos(_vec3* vPos)
{
	m_pInfo->m_vPosition = *vPos;
	m_pInfo->m_vPosition.y += 1.f;
}

void CNaviFlag::SetRender(_bool isRender)
{
	m_isRender = isRender;
}

void CNaviFlag::SetTexture(_int iTexture)
{
	m_iTexture = iTexture;
}
