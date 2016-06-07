#include "stdafx.h"
#include "StaticCamera.h"

CStaticCamera::CStaticCamera(void)
: m_fCamDistance(20.f)
, m_fCamAngle(D3DXToRadian(40.f))
{

}
//---------------------------------------------------------------------------------------------------------------------------------------

CStaticCamera::~CStaticCamera(void)
{

}
//---------------------------------------------------------------------------------------------------------------------------------------

HRESULT CStaticCamera::Initialize(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3* pEye , const _vec3* pAt , const _vec3* pUp , _float fFov /* = D3DXToRadian(45.0f)  */, _float fAspect /* = float(WINSIZEX) / float(WINSIZEY)  */, float fNear /* = 0.1f  */, float fFar /* = 1000.f */)
{
	m_pGraphicDev = pGraphicDev;

	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = *pUp;


	if(FAILED(CCamera::SetViewMatrix()))	//Veiw함수
		return E_FAIL;

	m_fAspect = fAspect;
	m_fFov = fFov;
	m_fNear = fNear;
	m_fFar = fFar;

	if(FAILED(CCamera::SetProjMatrix()))	//Project함수
		return E_FAIL;

	return S_OK;
}
//---------------------------------------------------------------------------------------------------------------------------------------

void CStaticCamera::Update(void)
{		
	SetCamera();
	CCamera::Update();
}
//---------------------------------------------------------------------------------------------------------------------------------------

void CStaticCamera::SetCamera(void)
{
	_vec3		vCamPos = m_pInfo->m_vDirection * - 1.f;
	D3DXVec3Normalize(&vCamPos, &vCamPos);												//마우스
	vCamPos *= m_fCamDistance;

	_vec3		vRight;
	memcpy(&vRight, &m_pInfo->m_matWorld.m[0][0], sizeof(float) * 3);
	D3DXVec3Normalize(&vRight, &vRight);

	_matrix		matRotAxis;
	D3DXMatrixRotationAxis(&matRotAxis, &vRight, m_fCamAngle);

	D3DXVec3TransformNormal(&vCamPos, &vCamPos, &matRotAxis);
	_vec3 vUp(m_pInfo->m_matWorld.m[1][0], m_pInfo->m_matWorld.m[1][1], m_pInfo->m_matWorld.m[1][2]);
	vCamPos = m_pInfo->m_vPosition;
	m_vEye = vCamPos - m_pInfo->m_vDirection * 3.f ;
	m_vEye +=  0.5f * _vec3(m_pInfo->m_matWorld.m[0][0], m_pInfo->m_matWorld.m[0][1], m_pInfo->m_matWorld.m[0][2]) + vUp * 20.f;
	m_vAt = (m_vEye + m_pInfo->m_vDirection );
}
//---------------------------------------------------------------------------------------------------------------------------------------

CStaticCamera * CStaticCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3* pEye , const _vec3* pAt , const _vec3* pUp , _float fFov /* = D3DXToRadian(45.0f)  */, _float fAspect /* = float(WINSIZEX) / float(WINSIZEY)  */, _float fNear /* = 0.1f  */, _float fFar /* = 1000.f */)
{
	CStaticCamera* pStaticCamera = new CStaticCamera;

	if(FAILED(pStaticCamera->Initialize(pGraphicDev , pEye , pAt , pUp , fFov , fAspect , fNear , fFar)))
	{
		::safe_delete(pStaticCamera);
		return NULL;
	}

	return pStaticCamera;
}
//---------------------------------------------------------------------------------------------------------------------------------------

void CStaticCamera::SetTransCamera(Engine::CTransform* pInfo)
{
	m_pInfo = pInfo;
}