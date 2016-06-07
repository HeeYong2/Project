#include "TerrainBuffer.h"

using namespace Engine;
//---------------------------------------------------------------------------------------------------------------------------
CTerrainBuffer::CTerrainBuffer(void)
{

}
//---------------------------------------------------------------------------------------------------------------------------
CTerrainBuffer::~CTerrainBuffer(void)
{
	Release(); 
}
//---------------------------------------------------------------------------------------------------------------------------------------

CResource* CTerrainBuffer::Clone_Resource(void)
{
	CResource* pTerrainBuffer = new CTerrainBuffer(*this);
	
	++(*m_pRefCnt);
	return pTerrainBuffer;

}
//---------------------------------------------------------------------------------------------------------------------------------------

HRESULT	Engine::CTerrainBuffer::CreateVertexIndexBuffer(LPDIRECT3DDEVICE9 pGraphicDev
																			   , const _ulong& wCntX, const _ulong& wCntZ, const float& fItv)
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt	= wCntX * wCntZ;
	m_dwVtxFVF	= VTXTEX_FVF;
	m_dwTriCnt	= (wCntX - 1) * (wCntZ - 1) * 2;
	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if(FAILED(CVIBuffer::CreateVertexIndexBuffer(pGraphicDev)))
		return E_FAIL;

	VTXTEX*		pVtxTex = NULL;

	m_pVB->Lock(0 , 0 , (void**)&pVtxTex , 0 );

	int iIndex = 0;

	for(_ulong z = 0 ; z < wCntZ ; ++z)
	{
		for(_ulong x = 0 ; x < wCntX ; ++x)
		{
			iIndex = z * wCntZ + x;
			pVtxTex[iIndex].vPosition = _vec3(float(x) * fItv , /*float(rand () % 2)*/0
				, float(z) * fItv);
			pVtxTex[iIndex].vTexUV = _vec2(x / (wCntX - 1.f), z / (wCntZ - 1.f));
		}
	}

	INDEX32 * pIndex = NULL;

	m_pIB->Lock( 0 , 0 , (void**)&pIndex , 0 );

	DWORD				dwTriCnt = 0; 
	_vec3				vDest, vSour, vFaceNormal;


	for (_uint CntZ_i = 0; CntZ_i < wCntZ - 1; ++CntZ_i)
	{
		for (_uint CntX_j = 0; CntX_j < wCntX - 1; ++CntX_j)
		{
			iIndex = CntZ_i * wCntX + CntX_j;

			pIndex[dwTriCnt]._1 = iIndex + wCntX;
			pIndex[dwTriCnt]._2 = iIndex + wCntX + 1;
			pIndex[dwTriCnt]._3 = iIndex + 1;

			vDest = pVtxTex[pIndex[dwTriCnt]._2].vPosition - pVtxTex[pIndex[dwTriCnt]._1].vPosition;
			vSour = pVtxTex[pIndex[dwTriCnt]._3].vPosition - pVtxTex[pIndex[dwTriCnt]._2].vPosition;
			D3DXVec3Cross(&vFaceNormal, &vDest, &vSour);
			pVtxTex[pIndex[dwTriCnt]._1].vNormal += vFaceNormal;
			pVtxTex[pIndex[dwTriCnt]._2].vNormal += vFaceNormal;
			pVtxTex[pIndex[dwTriCnt]._3].vNormal += vFaceNormal;
			++dwTriCnt;

			pIndex[dwTriCnt]._1 = iIndex + wCntX;
			pIndex[dwTriCnt]._2 = iIndex + 1;
			pIndex[dwTriCnt]._3 = iIndex;

			vDest = pVtxTex[pIndex[dwTriCnt]._2].vPosition - pVtxTex[pIndex[dwTriCnt]._1].vPosition;
			vSour = pVtxTex[pIndex[dwTriCnt]._3].vPosition - pVtxTex[pIndex[dwTriCnt]._2].vPosition;
			D3DXVec3Cross(&vFaceNormal, &vDest, &vSour);
			pVtxTex[pIndex[dwTriCnt]._1].vNormal += vFaceNormal;
			pVtxTex[pIndex[dwTriCnt]._2].vNormal += vFaceNormal;
			pVtxTex[pIndex[dwTriCnt]._3].vNormal += vFaceNormal;
			++dwTriCnt;
		}
	}

	for (_ulong i = 0; i < wCntX * wCntZ; ++i)	
		D3DXVec3Normalize(&pVtxTex[i].vNormal, &pVtxTex[i].vNormal);	


	m_pVB->Unlock();
	m_pIB->Unlock();

	return S_OK;
}
//---------------------------------------------------------------------------------------------------------------------------------------

CTerrainBuffer* CTerrainBuffer::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& wCntX, const _ulong& wCntZ, const float& fItv)
{
	CTerrainBuffer* pTerrainBuffer = new CTerrainBuffer;

	if(FAILED(((CTerrainBuffer*)pTerrainBuffer)->CreateVertexIndexBuffer(pGraphicDev, wCntX, wCntZ, fItv)))
	{
		Engine::safe_delete(pTerrainBuffer);
		return NULL;
	}
	return pTerrainBuffer;
}
//---------------------------------------------------------------------------------------------------------------------------------------

void CTerrainBuffer::Release(void)
{
	if((*m_pRefCnt) == 0)		
	{
		// �����ϴ� �۾�
		CVIBuffer::Release();
	}
	else
		--(*m_pRefCnt);
}
