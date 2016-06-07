#include "CubeBuffer.h"

using namespace Engine;

//------------------------------------------------------------------------------------------------------------------------------
Engine::CCubeBuffer::CCubeBuffer(void)
{

}

//------------------------------------------------------------------------------------------------------------------------------

Engine::CCubeBuffer::~CCubeBuffer(void)
{
	Release();
}

//------------------------------------------------------------------------------------------------------------------------------

CResource* Engine::CCubeBuffer::Clone_Resource(void)
{
	CResource*		pCube = new CCubeBuffer(*this);
	++(*m_pRefCnt);
	return pCube;
}

//------------------------------------------------------------------------------------------------------------------------------

HRESULT Engine::CCubeBuffer::CreateVertexIndexBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_dwVtxSize = sizeof(VTXCUBE);
	m_dwVtxCnt = 8;
	m_dwVtxFVF = VTXFVF_CUBE;
	m_dwTriCnt = 12;
	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	if(FAILED(CVIBuffer::CreateVertexIndexBuffer(pGraphicDev)))
		return E_FAIL;

	VTXCUBE*	pVtxCube = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxCube, 0);

	pVtxCube[0].vPosition = D3DXVECTOR3(-1.f, 1.f, -1.f);
	pVtxCube[0].vTexUV = pVtxCube[0].vPosition;

	pVtxCube[1].vPosition = D3DXVECTOR3(1.f, 1.f, -1.f);
	pVtxCube[1].vTexUV = pVtxCube[1].vPosition;

	pVtxCube[2].vPosition = D3DXVECTOR3(1.f, -1.f, -1.f);
	pVtxCube[2].vTexUV = pVtxCube[2].vPosition;

	pVtxCube[3].vPosition = D3DXVECTOR3(-1.f, -1.f, -1.f);
	pVtxCube[3].vTexUV = pVtxCube[3].vPosition;

	pVtxCube[4].vPosition = D3DXVECTOR3(-1.f, 1.f, 1.f);
	pVtxCube[4].vTexUV = pVtxCube[4].vPosition;

	pVtxCube[5].vPosition = D3DXVECTOR3(1.f, 1.f, 1.f);
	pVtxCube[5].vTexUV = pVtxCube[5].vPosition;

	pVtxCube[6].vPosition = D3DXVECTOR3(1.f, -1.f, 1.f);
	pVtxCube[6].vTexUV = pVtxCube[6].vPosition;

	pVtxCube[7].vPosition = D3DXVECTOR3(-1.f, -1.f, 1.f);
	pVtxCube[7].vTexUV = pVtxCube[7].vPosition;


	INDEX16*		pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// +x
	pIndex[0]._1 = 1; pIndex[0]._2 = 5;	pIndex[0]._3 = 6;
	pIndex[1]._1 = 1; pIndex[1]._2 = 6;	pIndex[1]._3 = 2;

	// -x
	pIndex[2]._1 = 4; pIndex[2]._2 = 0;	pIndex[2]._3 = 3;
	pIndex[3]._1 = 4; pIndex[3]._2 = 3;	pIndex[3]._3 = 7;

	// +y
	pIndex[4]._1 = 4; pIndex[4]._2 = 5;	pIndex[4]._3 = 1;
	pIndex[5]._1 = 4; pIndex[5]._2 = 1;	pIndex[5]._3 = 0;

	// -y
	pIndex[6]._1 = 3; pIndex[6]._2 = 2;	pIndex[6]._3 = 6;
	pIndex[7]._1 = 3; pIndex[7]._2 = 6;	pIndex[7]._3 = 7;

	// +z
	pIndex[8]._1 = 7; pIndex[8]._2 = 6;	pIndex[8]._3 = 5;
	pIndex[9]._1 = 7; pIndex[9]._2 = 5;	pIndex[9]._3 = 4;

	// -z
	pIndex[10]._1 = 0; pIndex[10]._2 = 1;	pIndex[10]._3 = 2;
	pIndex[11]._1 = 0; pIndex[11]._2 = 2;	pIndex[11]._3 = 3;

	m_pVB->Unlock();
	m_pIB->Unlock();

	return S_OK;
}

//------------------------------------------------------------------------------------------------------------------------------
CVIBuffer* CCubeBuffer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CVIBuffer* pCubeBuffer = new CCubeBuffer;

	if(FAILED(pCubeBuffer->CreateVertexIndexBuffer(pGraphicDev)))
	{
		::safe_delete(pCubeBuffer);
		return NULL;
	}

	return pCubeBuffer;
}

//------------------------------------------------------------------------------------------------------------------------------

void Engine::CCubeBuffer::Release(void)
{
	if((*m_pRefCnt) == 0)
		CVIBuffer::Release();
	else
		--(*m_pRefCnt);
}