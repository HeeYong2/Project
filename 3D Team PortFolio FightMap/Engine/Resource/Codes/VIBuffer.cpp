#include "VIBuffer.h"

USING(Engine)

//---------------------------------------------------------------------------------------------------------------------------
Engine::CVIBuffer::CVIBuffer(void)
: m_pVB(NULL)
, m_dwVtxSize(0)
, m_dwVtxFVF(0)
, m_dwVtxCnt(0)
{

}
//---------------------------------------------------------------------------------------------------------------------------
Engine::CVIBuffer::~CVIBuffer(void)
{

}

//---------------------------------------------------------------------------------------------------------------------------
void CVIBuffer::Throw_Vertex_Info(/*Out*/void* pVtxInfo)
{
	void* pOriVtxInfo = NULL;

	m_pVB->Lock(0, 0, &pOriVtxInfo, 0);

	memcpy(pVtxInfo, pOriVtxInfo, m_dwVtxCnt * m_dwVtxSize);

	m_pVB->Unlock();
}
//---------------------------------------------------------------------------------------------------------------------------
void CVIBuffer::SetVtxInfo(void* pVtxTex)
{
	void* pOriVertex = NULL;
	m_pVB->Lock(0, 0, &pOriVertex, 0);
	memcpy(pOriVertex, pVtxTex, m_dwVtxSize * m_dwVtxCnt);
	m_pVB->Unlock();
}

//---------------------------------------------------------------------------------------------------------------------------
void CVIBuffer::Receive_Index_Info(/*In*/void* pIndex, _ulong* pTriCnt)
{
	void* pOriIdxInfo = NULL;

	m_dwTriCnt = *pTriCnt;

	m_pIB->Lock(0, 0, &pOriIdxInfo, 0);

	memcpy(pOriIdxInfo, pIndex, m_dwTriCnt * m_dwIdxSize);

	m_pIB->Unlock();
}

//---------------------------------------------------------------------------------------------------------------------------
void CVIBuffer::Render(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCnt/* = 0*/)
{
	//gdgd
	pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	
	pGraphicDev->SetFVF(m_dwVtxFVF);

	pGraphicDev->SetIndices(m_pIB);

	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);	
}

//---------------------------------------------------------------------------------------------------------------------------
HRESULT Engine::CVIBuffer::CreateVertexIndexBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
{
	if(FAILED(pGraphicDev->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt, 0, m_dwVtxFVF, D3DPOOL_MANAGED, &m_pVB, NULL)))
	{
		msg_box("CreateVertexIndexBuffer Failed");
		return E_FAIL;
	}

	if(FAILED(pGraphicDev->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt, 0, m_IdxFmt, D3DPOOL_MANAGED, &m_pIB, NULL)))
	{
		msg_box("CreateIndexBuffer Failed");
		return E_FAIL;
	}
	return S_OK;
}

//---------------------------------------------------------------------------------------------------------------
void CVIBuffer::Release(void)
{
	if( (*m_pRefCnt) == 0)
	{
		Engine::safe_release(m_pVB);
		Engine::safe_release(m_pIB);
		CResource::Release();
	}	
}