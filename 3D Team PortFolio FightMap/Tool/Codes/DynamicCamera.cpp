#include "stdafx.h"
#include "DynamicCamera.h"
#include "Export_Function.h"

CDynamicCamera::CDynamicCamera(void)
{

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

CDynamicCamera::~CDynamicCamera(void)
{

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

HRESULT CDynamicCamera::Initialize(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3* pEye , const _vec3* pAt , const _vec3* pUp , _float fFov , _float fAspect , float fNear , float fFar)
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
	m_fCamSpeed = 10.f;
	if(FAILED(CCamera::SetProjMatrix()))	//Project함수
		return E_FAIL;

	return S_OK;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CDynamicCamera::Update(void)
{		//Veiw함수
	KeyCheck();
	CCamera::Update();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CDynamicCamera::KeyCheck(void)
{
	if(Engine::GetDIKeyState(DIK_A) & 0x80)
	{
		_vec3	vLook = m_vAt - m_vEye;	

		_vec3	vLeft;
		D3DXVec3Cross(&vLeft, &vLook, &m_vUp);
		D3DXVec3Normalize(&vLeft, &vLeft);
		m_vAt += vLeft * m_fCamSpeed * Engine::GetTime();
		m_vEye += vLeft * m_fCamSpeed * Engine::GetTime();
	}	

	if(Engine::GetDIKeyState(DIK_D) & 0x80)
	{
		_vec3 vLook = m_vAt - m_vEye;	

		_vec3	vRight;
		D3DXVec3Cross(&vRight, &m_vUp, &vLook);
		D3DXVec3Normalize(&vRight, &vRight);
		m_vAt += vRight * m_fCamSpeed * Engine::GetTime();
		m_vEye += vRight * m_fCamSpeed * Engine::GetTime();
	}		

	if(Engine::GetDIKeyState(DIK_W) & 0x80)
	{
		_vec3			vLook;
		vLook = m_vAt - m_vEye;
		D3DXVec3Normalize(&vLook, &vLook);
		m_vEye += vLook * m_fCamSpeed * Engine::GetTime();
		m_vAt += vLook * m_fCamSpeed * Engine::GetTime();
	}	

	if(Engine::GetDIKeyState(DIK_S) & 0x80)
	{
		_vec3			vLook;
		vLook = m_vAt - m_vEye;
		D3DXVec3Normalize(&vLook, &vLook);
		m_vEye -= vLook * m_fCamSpeed * Engine::GetTime();
		m_vAt -= vLook * m_fCamSpeed * Engine::GetTime();
	}	

	if(Engine::GetDIMouseState(Engine::CDXInput::DIM_RBUTTON) & 0x80)
	{
		_long	dwMouseMove = 0;

		if(dwMouseMove = Engine::GetDIMouseMove(Engine::CDXInput::DIM_XMOVE))
		{
			_vec3		vLook = m_vAt - m_vEye;

			_matrix			matRot;
			D3DXMatrixRotationAxis(&matRot, &_vec3(0.f, 1.f, 0.f), D3DXToRadian(dwMouseMove) / 5);
			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

			m_vAt = m_vEye + vLook;
		}

		if(dwMouseMove = Engine::GetDIMouseMove(Engine::CDXInput::DIM_YMOVE))
		{
			_vec3		vLook = m_vAt - m_vEye;	
			_vec3		vRight;
			D3DXVec3Cross(&vRight, &m_vUp, &vLook);
			D3DXVec3Normalize(&vRight, &vRight);

			_matrix			matRot;
			D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove) / 5);
			D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

			m_vAt = m_vEye + vLook;
		}
	}
	if(Engine::GetDIMouseMove(Engine::CDXInput::DIM_ZMOVE) < 0)
	{
		_vec3		vLook = m_vAt - m_vEye;	

		D3DXVec3Normalize(&vLook , &vLook );

		m_vEye -= vLook * m_fCamSpeed;
		m_vAt -= vLook * m_fCamSpeed;
	}

	if(Engine::GetDIMouseMove(Engine::CDXInput::DIM_ZMOVE) > 0)
	{
		_vec3		vLook = m_vAt - m_vEye;	

		D3DXVec3Normalize(&vLook , &vLook );

		m_vEye += vLook * m_fCamSpeed;
		m_vAt += vLook * m_fCamSpeed;
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

CDynamicCamera * CDynamicCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3* pEye , const _vec3* pAt , const _vec3* pUp , _float fFov /* = D3DXToRadian(45.0f)  */, _float fAspect /* = float(WINSIZEX) / float(WINSIZEY)  */, _float fNear /* = 0.1f  */, _float fFar /* = 1000.f */)
{
	CDynamicCamera* pDynamicCamera = new CDynamicCamera;

	if(FAILED(pDynamicCamera->Initialize(pGraphicDev , pEye , pAt , pUp , fFov , fAspect , fNear , fFar)))
	{
		::safe_delete(pDynamicCamera);
		return NULL;
	}

	return pDynamicCamera;
}