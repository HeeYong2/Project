#include "DynamicMesh.h"
#include "HierachyLoader.h"
#include "AnimationController.h"

USING(Engine)

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Engine::CDynamicMesh::CDynamicMesh(void)
: m_pLoader(NULL)
{

}
Engine::CDynamicMesh::CDynamicMesh(const CDynamicMesh& Mesh)
: m_pLoader(Mesh.m_pLoader)
, m_pAniCtrler(new CAnimationController(Mesh.m_pAniCtrler))
, m_pRootBone(Mesh.m_pRootBone)
, CMesh(Mesh)
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

_bool Engine::CDynamicMesh::GetEndPeriod(void)
{
	return m_pAniCtrler->GetEndPeriod();
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const _matrix* CDynamicMesh::GetFrameMatrix(const char* pFrameName)
{
	DERIVED_D3DXFRAME*			pFrame = ((DERIVED_D3DXFRAME*)D3DXFrameFind(m_pRootBone ,pFrameName));

	return &(pFrame->CombinedTransformMatrix);
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
_uint Engine::CDynamicMesh::GetAniMaxCnt(void)
{
	return m_pAniCtrler->m_iMaxAniSet;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CDynamicMesh::SetAnimationSet(_uint iAniIdx)
{
	m_pAniCtrler->SetAnimationSet(iAniIdx);
}
//---------------------------------------------------------------------------------------------------------------------------------------

void CDynamicMesh::FrameMove(_float fTime)
{
	m_pAniCtrler->FrameMove(fTime);

	_matrix				matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	UpdateFrameMatrices((DERIVED_D3DXFRAME*)m_pRootBone, &matIdentity);
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Engine::CDynamicMesh::~CDynamicMesh(void)
{
	Release();
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT Engine::CDynamicMesh::Ready_Mesh(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pPath, const TCHAR* pFileName)
{
	TCHAR			szFullPath[128] = L"";
	lstrcpy(szFullPath, pPath);
	lstrcat(szFullPath, pFileName);

	m_pAniCtrler = new CAnimationController;
	m_pLoader = CHierachyLoader::Create(pPath);

	// 뼈대와 메시들을 읽기위한 기능을 제공해 주는 클래스이다.
	if(FAILED(D3DXLoadMeshHierarchyFromX(szFullPath, D3DXMESH_MANAGED, pGraphicDev, m_pLoader
		, NULL, &m_pRootBone, m_pAniCtrler->Get_AniCtrl())))
		return E_FAIL;

	_matrix			matrix;
	D3DXMatrixIdentity(&matrix);

	UpdateFrameMatrices((DERIVED_D3DXFRAME*)m_pRootBone, &matrix);
	SetUpBoneMatrixPointer((DERIVED_D3DXFRAME*)m_pRootBone);

	return S_OK;
}
//---------------------------------------------------------------------------------------------------------------------------------------

void CDynamicMesh::UpdateFrameMatrices(DERIVED_D3DXFRAME* pFrame, const _matrix* pmatParent)
{
	if(NULL == pFrame)
		return;

	pFrame->CombinedTransformMatrix = pFrame->TransformationMatrix * *pmatParent;

	if(NULL != pFrame->pFrameSibling)
		UpdateFrameMatrices(((DERIVED_D3DXFRAME*)pFrame->pFrameSibling), pmatParent);
	if(NULL != pFrame->pFrameFirstChild)
		UpdateFrameMatrices(((DERIVED_D3DXFRAME*)pFrame->pFrameFirstChild), &pFrame->CombinedTransformMatrix);
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Engine::CDynamicMesh::Render(LPDIRECT3DDEVICE9 pGraphicDev, const _matrix* pWorldMatrix)
{
	pGraphicDev->SetTransform(D3DTS_WORLD, pWorldMatrix);
	Render(pGraphicDev, (DERIVED_D3DXFRAME*)m_pRootBone);	
}
//---------------------------------------------------------------------------------------------------------------------------------------

void Engine::CDynamicMesh::Render(LPDIRECT3DDEVICE9 pGraphicDev, DERIVED_D3DXFRAME* pBone)
{
	if(NULL == pBone)
		return;

	if(NULL != pBone->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED*		pBoneMesh = (D3DXMESHCONTAINER_DERIVED*)pBone->pMeshContainer;

		if(NULL != pBoneMesh->pSkinInfo)
		{
			int			iNumBone = pBoneMesh->pSkinInfo->GetNumBones();

			for (int i = 0; i < iNumBone; ++i)
			{
				pBoneMesh->pResultMatrix[i] = pBoneMesh->pBoneoffsetMatrix[i] * *pBoneMesh->ppBoneMatrix[i];
			}
		}

		BYTE*				pSour;
		BYTE*				pDest;

		pBoneMesh->pOriMesh->LockVertexBuffer(0, (void**)&pSour);
		pBoneMesh->MeshData.pMesh->LockVertexBuffer(0, (void**)&pDest);

		pBoneMesh->pSkinInfo->UpdateSkinnedMesh(pBoneMesh->pResultMatrix, NULL, pSour, pDest);

		pBoneMesh->pOriMesh->UnlockVertexBuffer();
		pBoneMesh->MeshData.pMesh->UnlockVertexBuffer();

		for (DWORD i = 0; i < pBoneMesh->NumMaterials; ++i)
		{
			int	iMtrl = pBoneMesh->pAttributeTable[i].AttribId;

			pGraphicDev->SetMaterial(&pBoneMesh->vecMtrls[iMtrl]);
			
			if(!pBoneMesh->vecTextures.empty())
				pGraphicDev->SetTexture(0, pBoneMesh->vecTextures[iMtrl]);

			pBoneMesh->MeshData.pMesh->DrawSubset(iMtrl);
		}
	}

	if(NULL != pBone->pFrameSibling)
		Render(pGraphicDev, (DERIVED_D3DXFRAME*)pBone->pFrameSibling);
	if(NULL != pBone->pFrameFirstChild)
		Render(pGraphicDev, (DERIVED_D3DXFRAME*)pBone->pFrameFirstChild);
}
//---------------------------------------------------------------------------------------------------------------------------------------

void Engine::CDynamicMesh::SetUpBoneMatrixPointer(DERIVED_D3DXFRAME* pBone)
{
	if(NULL != pBone->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED*		pBoneMesh = ((D3DXMESHCONTAINER_DERIVED*)pBone->pMeshContainer);

		if(NULL != pBoneMesh->pSkinInfo)
		{
			int			iNumBone = pBoneMesh->pSkinInfo->GetNumBones();

			pBoneMesh->ppBoneMatrix = new D3DXMATRIX*[iNumBone];

			for( int i = 0; i < iNumBone; ++i)
			{				
				DERIVED_D3DXFRAME*		pLinkedBone = (DERIVED_D3DXFRAME*)D3DXFrameFind(m_pRootBone, pBoneMesh->pSkinInfo->GetBoneName(i));

				if(NULL != pLinkedBone)
					pBoneMesh->ppBoneMatrix[i] = &pLinkedBone->CombinedTransformMatrix;
				else
					pBoneMesh->ppBoneMatrix[i] = NULL;				
			}
		}
	}

	if(NULL != pBone->pFrameSibling)
		SetUpBoneMatrixPointer((DERIVED_D3DXFRAME*)pBone->pFrameSibling);

	if(NULL != pBone->pFrameFirstChild)
		SetUpBoneMatrixPointer((DERIVED_D3DXFRAME*)pBone->pFrameFirstChild);
}


//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CResource* Engine::CDynamicMesh::Clone_Resource(void)
{
	CResource*	 pDynamic = new CDynamicMesh(*this);

	++(*m_pRefCnt);

	return pDynamic;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CDynamicMesh* Engine::CDynamicMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pPath, const TCHAR* pFileName)
{
	CDynamicMesh*		pDynamicMesh = new CDynamicMesh;

	if(FAILED(pDynamicMesh->Ready_Mesh(pGraphicDev, pPath, pFileName)))
	{
		Engine::safe_delete(pDynamicMesh);
		return NULL;
	}
	return pDynamicMesh;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Engine::CDynamicMesh::Release(void)
{
	Engine::safe_delete(m_pAniCtrler);

	if((*m_pRefCnt) == 0)		
	{
		m_pLoader->DestroyFrame(m_pRootBone);

		Engine::safe_delete(m_pLoader);		

		CMesh::Release();
	}
	else
		--(*m_pRefCnt);	
}
