#include "RcTex.h"

USING(Engine)

//---------------------------------------------------------------------------------------------------------------------------
CRcTex::CRcTex(void)
{

}

//---------------------------------------------------------------------------------------------------------------------------
CRcTex::~CRcTex(void)
{
	Release();
}
//---------------------------------------------------------------------------------------------------------------------------
CResource * CRcTex::Clone_Resource(void)
{
	CResource * pRcTex = new CRcTex(*this);

	++(*m_pRefCnt);

	return pRcTex;
}

//---------------------------------------------------------------------------------------------------------------------------
HRESULT Engine::CRcTex::CreateVertexIndexBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxFVF = VTXTEX_FVF;
	m_dwVtxCnt = 4;

	m_IdxFmt = D3DFMT_INDEX16;
	m_dwTriCnt = 2;
	m_dwIdxSize = sizeof(INDEX16);

	if(FAILED(CVIBuffer::CreateVertexIndexBuffer(pGraphicDev)))
		return E_FAIL;

	// 1. 공간에 대한 독점권을 가지겠다.
	// 2. 할당되어있는 공간에 가장 앞 주소를 어덩오겠다.
	VTXTEX*			pVtxtex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxtex, 0);

	pVtxtex[0].vPosition = _vec3(-0.5f, 0.5f, 0.f);
	pVtxtex[0].vNormal = _vec3(0.f, 0.f, 0.f);
	pVtxtex[0].vTexUV = _vec2(0.0f, 0.f);

	pVtxtex[1].vPosition = _vec3(0.5f, 0.5f, 0.f);
	pVtxtex[1].vNormal = _vec3(0.f, 0.f, 0.f);
	pVtxtex[1].vTexUV = _vec2(1.0f, 0.f);

	pVtxtex[2].vPosition = _vec3(0.5f, -0.5f, 0.f);
	pVtxtex[2].vNormal = _vec3(0.f, 0.f, 0.f);
	pVtxtex[2].vTexUV = _vec2(1.0f, 1.f);

	pVtxtex[3].vPosition = _vec3(-0.5f, -0.5f, 0.f);
	pVtxtex[3].vNormal = _vec3(0.f, 0.f, 0.f);
	pVtxtex[3].vTexUV = _vec2(0.0f, 1.f);
	
	INDEX16*			pIndex = NULL;
	_vec3				vDest, vSour, vFaceNormal;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._1 = 0;
	pIndex[0]._2 = 1;
	pIndex[0]._3 = 2;
	vDest = pVtxtex[pIndex[0]._2].vPosition - pVtxtex[pIndex[0]._1].vPosition;
	vSour = pVtxtex[pIndex[0]._3].vPosition - pVtxtex[pIndex[0]._2].vPosition;
	D3DXVec3Cross(&vFaceNormal, &vDest, &vSour);
	pVtxtex[pIndex[0]._1].vNormal += vFaceNormal;
	pVtxtex[pIndex[0]._2].vNormal += vFaceNormal;
	pVtxtex[pIndex[0]._3].vNormal += vFaceNormal;

	pIndex[1]._1 = 0;
	pIndex[1]._2 = 2;
	pIndex[1]._3 = 3;
	vDest = pVtxtex[pIndex[1]._2].vPosition - pVtxtex[pIndex[1]._1].vPosition;
	vSour = pVtxtex[pIndex[1]._3].vPosition - pVtxtex[pIndex[1]._2].vPosition;
	D3DXVec3Cross(&vFaceNormal, &vDest, &vSour);
	pVtxtex[pIndex[1]._1].vNormal += vFaceNormal;
	pVtxtex[pIndex[1]._2].vNormal += vFaceNormal;
	pVtxtex[pIndex[1]._3].vNormal += vFaceNormal;

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)	
		D3DXVec3Normalize(&pVtxtex[i].vNormal, &pVtxtex[i].vNormal);

	m_pIB->Unlock();

	m_pVB->Unlock();

	return S_OK;
}

//---------------------------------------------------------------------------------------------------------------------------
CRcTex* CRcTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CRcTex*			pRcTex = new CRcTex;

	if(FAILED(pRcTex->CreateVertexIndexBuffer(pGraphicDev)))
	{
		Engine::safe_delete(pRcTex);
		return NULL;
	}	
	return pRcTex;
}
//---------------------------------------------------------------------------------------------------------------------------------------

void CRcTex::Release(void) // 복제본이 해제될때 or 원본이 해제될때
{
	if((*m_pRefCnt) == 0)		
	{
		// 해제하는 작업
		CVIBuffer::Release();
	}
	else
		--(*m_pRefCnt);
}