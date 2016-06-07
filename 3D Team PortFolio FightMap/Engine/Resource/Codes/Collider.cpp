#include "Collider.h"

using namespace Engine;

//---------------------------------------------------------------------------------------------------------------------------
Engine::CCollider::CCollider(void)
: m_pMesh(NULL)
, m_fRadius(1.f)
{

}

//---------------------------------------------------------------------------------------------------------------------------

Engine::CCollider::~CCollider(void)
{
	Release();
}
//---------------------------------------------------------------------------------------------------------------------------
HRESULT Engine::CCollider::Ready_Mesh(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pPath , const TCHAR* pFileName)
{
	D3DXMatrixIdentity(&m_Collider.m_matWorld);

 	if(FAILED(D3DXCreateSphere(pGraphicDev , m_fRadius , 20 , 20 ,&m_pMesh , NULL)))
 		return E_FAIL;

// 	if(FAILED(D3DXCreateBox(pGraphicDev , m_Collider.m_vMax.x - m_Collider.m_vMin.x
// 		, m_Collider.m_vMax.y - m_Collider.m_vMin.y
// 		, m_Collider.m_vMax.z - m_Collider.m_vMin.z , &m_pMesh , NULL)))
// 		return E_FAIL;


	return S_OK;
}
//---------------------------------------------------------------------------------------------------------------------------

void Engine::CCollider::Render(LPDIRECT3DDEVICE9 pGraphicDev , const _matrix* pWorldMatrix)
{
	m_Collider.m_matWorld = *pWorldMatrix;
	pGraphicDev->SetTransform(D3DTS_WORLD , pWorldMatrix);
	m_pMesh->DrawSubset(0);
}

//---------------------------------------------------------------------------------------------------------------------------

CResource* Engine::CCollider::Clone_Resource(void)
{
	CResource*		pCollider = new CCollider(*this);

	++(*m_pRefCnt);

	return pCollider;
}

//---------------------------------------------------------------------------------------------------------------------------
CCollider* Engine::CCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev , const TCHAR* pPath , const TCHAR* pFileName)
{
	CCollider*	pCollider = new CCollider;

	if(FAILED(pCollider->Ready_Mesh(pGraphicDev , pPath , pFileName)))
	{
		Engine::safe_delete(pCollider);
		return NULL;
	}
	return pCollider;
}


//---------------------------------------------------------------------------------------------------------------------------
void Engine::CCollider::Release(void)
{
	if((*m_pRefCnt) == 0)		
	{
		Engine::safe_release(m_pMesh);

		// 해제하는 작업
		CMesh::Release();
	}
	else
		--(*m_pRefCnt);	
}

void Engine::CCollider::SetRadius(float fRadius)
{
	m_fRadius = fRadius;
}