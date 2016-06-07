#include "Transform.h"

using namespace Engine;

CTransform::CTransform(const _vec3* pGlobalLook)
{
	memcpy(&m_vDirection ,  pGlobalLook , sizeof(_vec3));
}
//---------------------------------------------------------------------------------------------------------------------------------------

CTransform::~CTransform(void)
{

}
//---------------------------------------------------------------------------------------------------------------------------------------

HRESULT CTransform::Initialize(void)
{
	m_vScale = _vec3(1.f, 1.f, 1.f);
	ZeroMemory(&m_vPosition, sizeof(_vec3));
	ZeroMemory(m_fAngle, sizeof(_float ) * ANGLE_END);

	D3DXMatrixIdentity(&m_matWorld);
	return S_OK;
}
//---------------------------------------------------------------------------------------------------------------------------------------

void CTransform::Update(void)
{
	_matrix			matScale, matRotX, matRotY, matRotZ, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationX(&matRotX, m_fAngle[ANGLE_X]);
	D3DXMatrixRotationY(&matRotY, m_fAngle[ANGLE_Y]);
	D3DXMatrixRotationZ(&matRotZ, m_fAngle[ANGLE_Z]);

	D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	m_matWorld = matScale * matRotX * matRotY * matRotZ * matTrans;

}
//---------------------------------------------------------------------------------------------------------------------------------------

CTransform* CTransform::Create(const _vec3* pGlobalLook)
{
	CTransform * pTransform = new CTransform(pGlobalLook);

	if(FAILED(pTransform->Initialize()))
	{
		Engine::safe_delete(pTransform);
		return NULL;
	}
	return pTransform;
}