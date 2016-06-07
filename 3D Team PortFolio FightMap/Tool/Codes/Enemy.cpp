#include "stdafx.h"
#include "Enemy.h"
#include "Export_Function.h"
#include "..\..\Engine\Resource\Codes\StaticMesh.h"

CEnemy::CEnemy(void)
: m_fSpeed(30.f)
, m_dwNaviIdx(0)
, m_pEffect(NULL)
{

}

CEnemy::~CEnemy(void)
{
	Release();
}

HRESULT CEnemy::Initialize(void)
{
	if(FAILED(AddComponent()))
		return E_FAIL;
	//서브터레인위치
	m_pInfo->m_vPosition = _vec3(660.f , 0.f , 460.f);

	m_pInfo->m_fAngle[m_pInfo->ANGLE_Y] = D3DXToRadian(-45.f);
	m_pInfo->m_vDirection = _vec3(0.f , 1.f , 0.f);
	m_pInfo->m_vScale = _vec3(0.05f , 0.05f , 0.05f );

	m_pEffect = Engine::Get_Effect(L"Shader_Default");

	if(NULL == m_pEffect)
		return E_FAIL;

	return S_OK;
}

void CEnemy::Update(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDirection, &g_vLook, &m_pInfo->m_matWorld);
	D3DXVec3Normalize(&m_pInfo->m_vDirection, &m_pInfo->m_vDirection);
	KeyCheck();
	Engine::CGameObject::Update();
}

void CEnemy::Render(void)
{
	SetConstantTable();
	m_pEffect->Begin(NULL , 0);
	m_pEffect->BeginPass(0);

	Render_Mesh();

	m_pEffect->EndPass();
	m_pEffect->End();
}

CEnemy* CEnemy::Create(void)
{
	CEnemy*	pEnemy =new CEnemy;
	if(FAILED(pEnemy->Initialize()))
	{
		::safe_delete(pEnemy);
		return NULL;
	}
	return pEnemy;
}

void CEnemy::Render_Mesh(void)
{
	((Engine::CStaticMesh*)m_pMeshCom)->Render(Engine::GetGraphicDev() , &m_pInfo->m_matWorld);
}

HRESULT CEnemy::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;
	//= For.Transform =======================================================================
	pComponent = m_pInfo = Engine::CTransform::Create(&g_vLook);
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOM::value_type(L"Transform", pComponent));

	pComponent = m_pMeshCom = Engine::Clone_Resource(RESOURCE_STAGE , L"Enemy");
	if(pComponent == NULL )
		return E_FAIL;

	m_mapComponent.insert(MAPCOM::value_type(L"Enemy" , pComponent));

	/////////////////////////////////////////
	return S_OK;
}

void CEnemy::KeyCheck(void)
{
	//m_dwNaviIdx = 0;
 	if(Engine::GetDIKeyState(DIK_F) & 0x80)		//왼 회전 
 	{
 		m_pInfo->m_fAngle[Engine::CTransform::ANGLE_Y] -= D3DXToRadian(90.f * Engine::GetTime());
 	}
 
 	if(Engine::GetDIKeyState(DIK_H) & 0x80)		//오른 회전
 	{
 		m_pInfo->m_fAngle[Engine::CTransform::ANGLE_Y] += D3DXToRadian(90.f * Engine::GetTime());
 	}
 
 	if(Engine::GetDIKeyState(DIK_T) & 0x80)	//전진 하기
 	{
 		m_pInfo->m_vPosition += m_pInfo->m_vDirection * m_fSpeed * Engine::GetTime();
 		//	m_dwNaviIdx = Engine::MoveOnNaviMesh(&m_pInfo->m_vPosition , &(m_pInfo->m_vDirection * m_fSpeed * Engine::GetTime()) , m_dwNaviIdx);
 	}
 
 	if(Engine::GetDIKeyState(DIK_G) & 0x80)	//후진 하기
 	{
 		m_pInfo->m_vPosition -= m_pInfo->m_vDirection * m_fSpeed * Engine::GetTime();
 		//	m_dwNaviIdx = Engine::MoveOnNaviMesh(&m_pInfo->m_vPosition , &(-m_pInfo->m_vDirection * m_fSpeed * Engine::GetTime()) , m_dwNaviIdx);
 	}
 
 	if(Engine::GetDIKeyState(DIK_U) & 0x80)	//후진 하기
 	{
 		m_pInfo->m_vPosition -= _vec3(0.f , 5.f , 0.f) * Engine::GetTime();
 		//	m_dwNaviIdx = Engine::MoveOnNaviMesh(&m_pInfo->m_vPosition , &(-m_pInfo->m_vDirection * m_fSpeed * Engine::GetTime()) , m_dwNaviIdx);
 	}
 
 	if(Engine::GetDIKeyState(DIK_I) & 0x80)	//후진 하기
 	{
 		m_pInfo->m_vPosition += _vec3(0.f , 5.f , 0.f) * Engine::GetTime();
 		//	m_dwNaviIdx = Engine::MoveOnNaviMesh(&m_pInfo->m_vPosition , &(-m_pInfo->m_vDirection * m_fSpeed * Engine::GetTime()) , m_dwNaviIdx);
 	}
}
//------------------------------------------------------------------------------------------------------------------------------
_ulong CEnemy::Release(void)
{
	_ulong dwRefCnt = CGameObject::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}
	return dwRefCnt;

}

void CEnemy::SetConstantTable(void)
{
	_matrix matView, matProj;
	Engine::GetGraphicDev()->GetTransform(D3DTS_VIEW, &matView);
	Engine::GetGraphicDev()->GetTransform(D3DTS_PROJECTION, &matProj);
	m_pEffect->SetMatrix("g_matWorld", &m_pInfo->m_matWorld);
	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &matProj);	
}