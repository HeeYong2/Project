#include "TriCol.h"

using namespace Engine;

CTriCol::CTriCol(void)
{
	
}

CTriCol::~CTriCol(void)
{
	Release();
}

CResource* Engine::CTriCol::Clone_Resource(void)
{
	CResource * pRcTex = new CTriCol(*this);

	++(*m_pRefCnt);

	return pRcTex;
}

HRESULT Engine::CTriCol::CreateVertexIndexBuffer(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt = 3;
	m_dwVtxFVF = VTXFVF_COL;
	m_dwTriCnt = 1;
	m_dwIdxSize = sizeof(INDEX16);
	m_IdxFmt = D3DFMT_INDEX16;

	if(FAILED(CVIBuffer::CreateVertexIndexBuffer(pGraphicDev)))
		return E_FAIL;

	VTXCOL*		pVtxCol = NULL;

	m_pVB->Lock(0 , 0 , (void**)&pVtxCol , 0 );

	pVtxCol[0].vPosition = _vec3(0.f , 1.f , 0.f);
	pVtxCol[0].dwColor = D3DCOLOR_ARGB(255 , 255 , 0, 0);

	pVtxCol[1].vPosition = _vec3(1.f , -1.f , 0.f);
	pVtxCol[1].dwColor = D3DCOLOR_ARGB(255 , 255 , 0, 0);

	pVtxCol[2].vPosition = _vec3(-1.f , -1.f , 0.f);
	pVtxCol[2].dwColor = D3DCOLOR_ARGB(255 , 255 , 0, 0);

	m_pVB->Unlock();

	return S_OK;
}

CTriCol* Engine::CTriCol::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTriCol*	pTriCol = new CTriCol();

	if(FAILED(pTriCol->CreateVertexIndexBuffer(pGraphicDev)))
	{
		::safe_delete(pTriCol);
		return NULL;
	}

	return pTriCol;

}

void Engine::CTriCol::Release(void)
{
	if( (*m_pRefCnt) == 0 )
	{
		CVIBuffer::Release();
	}
	else
		--(*m_pRefCnt);
}