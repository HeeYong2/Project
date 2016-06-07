#include "Camera.h"

using namespace Engine;

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
CCamera::CCamera(void)
: m_fCamSpeed(2.f)
{

}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
CCamera::~CCamera(void)
{

}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CCamera::Update(void)
{
	SetViewMatrix();
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CCamera::SetViewMatrix(void)
{
	D3DXMatrixLookAtLH(&m_matView , &m_vEye , &m_vAt , &m_vUp);

	if(m_pGraphicDev == NULL)
		return E_FAIL;

	m_pGraphicDev->SetTransform(D3DTS_VIEW , &m_matView);
	return S_OK;
}
//---------------------------------------------------------------------------------------------------------------------------------------

HRESULT CCamera::SetProjMatrix(void)
{
	D3DXMatrixPerspectiveFovLH(&m_matProjection , m_fFov , m_fAspect , m_fNear , m_fFar );

	if(m_pGraphicDev == NULL)
		return E_FAIL;

	m_pGraphicDev->SetTransform(D3DTS_PROJECTION , &m_matProjection);

	return S_OK;
}