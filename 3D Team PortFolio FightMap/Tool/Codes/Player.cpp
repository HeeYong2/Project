#include "stdafx.h"
#include "Player.h"
#include "Export_Function.h"
#include "..\..\Engine\Resource\Codes\StaticMesh.h"

CPlayer::CPlayer(void)
: m_fSpeed(10.f)
, m_dwNaviIdx(0)
, m_pEffect(NULL)
{

}

CPlayer::~CPlayer(void)
{
	Release();
}

HRESULT CPlayer::Initialize(void)
{
	if(FAILED(AddComponent()))
		return E_FAIL;
//서브터레인위치
	m_pInfo->m_vPosition = _vec3(146.f , 4.8f , 261.f);
//	m_pInfo->m_vPosition = _vec3(375.f , 1.f , 395.f);

	m_pInfo->m_fAngle[m_pInfo->ANGLE_Y] = D3DXToRadian(-45.f);
	m_pInfo->m_vDirection = _vec3(0.f , 1.f , 0.f);
	m_pInfo->m_vScale = _vec3(0.05f , 0.05f , 0.05f );

	m_pEffect = Engine::Get_Effect(L"Shader_Default");

	if(NULL == m_pEffect)
		return E_FAIL;

	SetNaviIndex();
	return S_OK;
}

void CPlayer::Update(void)
{
	D3DXVec3TransformNormal(&m_pInfo->m_vDirection, &g_vLook, &m_pInfo->m_matWorld);
	D3DXVec3Normalize(&m_pInfo->m_vDirection, &m_pInfo->m_vDirection);
	KeyCheck();
	Engine::CGameObject::Update();
}

void CPlayer::Render(void)
{
	SetConstantTable();
	m_pEffect->Begin(NULL , 0);
	m_pEffect->BeginPass(0);

	Render_Mesh();
	
	m_pEffect->EndPass();
	m_pEffect->End();
}

CPlayer* CPlayer::Create(void)
{
	CPlayer*	pPlayer =new CPlayer;
	if(FAILED(pPlayer->Initialize()))
	{
		::safe_delete(pPlayer);
		return NULL;
	}
	return pPlayer;
}

void CPlayer::Render_Mesh(void)
{
	((Engine::CStaticMesh*)m_pMeshCom)->Render(Engine::GetGraphicDev() , &m_pInfo->m_matWorld);
}

HRESULT CPlayer::AddComponent(void)
{
	Engine::CComponent*		pComponent = NULL;
	//= For.Transform =======================================================================
	pComponent = m_pInfo = Engine::CTransform::Create(&g_vLook);
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOM::value_type(L"Transform", pComponent));

	pComponent = m_pMeshCom = Engine::Clone_Resource(RESOURCE_STAGE , L"Player");
	if(pComponent == NULL )
		return E_FAIL;

	m_mapComponent.insert(MAPCOM::value_type(L"Player" , pComponent));

	/////////////////////////////////////////
	return S_OK;
}

void CPlayer::KeyCheck(void)
{
	//m_dwNaviIdx = 0;
	if(Engine::GetDIKeyState(DIK_LEFT) & 0x80)		//왼 회전 
	{
		m_pInfo->m_fAngle[Engine::CTransform::ANGLE_Y] -= D3DXToRadian(90.f * Engine::GetTime());
	}

	if(Engine::GetDIKeyState(DIK_RIGHT) & 0x80)		//오른 회전
	{
		m_pInfo->m_fAngle[Engine::CTransform::ANGLE_Y] += D3DXToRadian(90.f * Engine::GetTime());
	}

	if(Engine::GetDIKeyState(DIK_UP) & 0x80)	//전진 하기
	{
	//	m_pInfo->m_vPosition += m_pInfo->m_vDirection * m_fSpeed * Engine::GetTime();
		m_dwNaviIdx = Engine::MoveOnNaviMesh(&m_pInfo->m_vPosition , &(m_pInfo->m_vDirection * m_fSpeed * Engine::GetTime()) , m_dwNaviIdx);
	}

	if(Engine::GetDIKeyState(DIK_DOWN) & 0x80)	//후진 하기
	{
	//	m_pInfo->m_vPosition -= m_pInfo->m_vDirection * m_fSpeed * Engine::GetTime();
		m_dwNaviIdx = Engine::MoveOnNaviMesh(&m_pInfo->m_vPosition , &(-m_pInfo->m_vDirection * m_fSpeed * Engine::GetTime()) , m_dwNaviIdx);
	}

	if(Engine::GetDIKeyState(DIK_N) & 0x80)	//후진 하기
	{
		m_pInfo->m_vPosition -= _vec3(0.f , 5.f , 0.f) * Engine::GetTime();
	}

	if(Engine::GetDIKeyState(DIK_M) & 0x80)	//후진 하기
	{
		m_pInfo->m_vPosition += _vec3(0.f , 5.f , 0.f) * Engine::GetTime();
	}

	if(Engine::GetDIKeyState(DIK_P) & 0x80)	//후진 하기
	{
		m_pInfo->m_vPosition = _vec3(460.f , 6.7f , 370.f);
	}

	if(Engine::GetDIKeyState(DIK_RETURN) & 0x80)	//후진 하기
	{
		SetNaviIndex();
	}
}
//------------------------------------------------------------------------------------------------------------------------------
_ulong CPlayer::Release(void)
{
	_ulong dwRefCnt = CGameObject::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}
	return dwRefCnt;

}

void CPlayer::SetConstantTable(void)
{
	_matrix matView, matProj;
	Engine::GetGraphicDev()->GetTransform(D3DTS_VIEW, &matView);
	Engine::GetGraphicDev()->GetTransform(D3DTS_PROJECTION, &matProj);
	m_pEffect->SetMatrix("g_matWorld", &m_pInfo->m_matWorld);
	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &matProj);	
}

//------------------------------------------------------------------------------------------------------------------------------

Engine::CTransform* CPlayer::GetPlayerInfo(void)
{
	return m_pInfo;
}

//------------------------------------------------------------------------------------------------------------------------------

_vec3 CPlayer::GetPlayerPos(void)
{
	return m_pInfo->m_vPosition;
}

void CPlayer::SetNaviIndex(void)
{
	vector<Engine::CNaviMesh*>*	pVecNaviMesh = Engine::GetVecNaviMesh();

	vector<Engine::CNaviMesh*>::iterator iter = pVecNaviMesh->begin();
	vector<Engine::CNaviMesh*>::iterator iter_end= pVecNaviMesh->end();

	for(; iter != iter_end; ++iter)
	{
		if((*iter)->isInNavi(&m_pInfo->m_vPosition))
		{
			m_dwNaviIdx = (*iter)->GetInfo().dwIdx;
		}
	}
}