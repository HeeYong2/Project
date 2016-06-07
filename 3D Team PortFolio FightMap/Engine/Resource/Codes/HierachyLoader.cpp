#include "HierachyLoader.h"

USING(Engine)

Engine::CHierachyLoader::CHierachyLoader(const TCHAR* pPath)
: m_szPath(pPath)
{

}

Engine::CHierachyLoader::~CHierachyLoader(void)
{

}

HRESULT Engine::CHierachyLoader::AllocateName(char** ppDestName, const char* pFrameName)
{
	*ppDestName = new CHAR[strlen(pFrameName) + 1];

	strcpy(*ppDestName, pFrameName);

	return S_OK;		
}

// 안에서 유효한 뼈대의 정보를 생성 한 다음에 ppNewFrame에 담아주세요
STDMETHODIMP Engine::CHierachyLoader::CreateFrame(THIS_ LPCSTR Name /*input*/, LPD3DXFRAME *ppNewFrame /*output*/)
{
	DERIVED_D3DXFRAME*		pNewFrame = new DERIVED_D3DXFRAME;
	ZeroMemory(pNewFrame, sizeof(DERIVED_D3DXFRAME));

	if(NULL != Name)
		AllocateName(&pNewFrame->Name, Name);	

	_matrix				matIdentity;

	D3DXMatrixIdentity(&matIdentity);

	D3DXMatrixIdentity(&pNewFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pNewFrame->CombinedTransformMatrix);

	*ppNewFrame = pNewFrame;	

	return S_OK;
}

STDMETHODIMP Engine::CHierachyLoader::CreateMeshContainer(THIS_ LPCSTR Name
														  , CONST D3DXMESHDATA *pMeshData
														  , CONST D3DXMATERIAL *pMaterials
														  , CONST D3DXEFFECTINSTANCE *pEffectInstances
														  , DWORD NumMaterials
														  , CONST DWORD *pAdjacency
														  , LPD3DXSKININFO pSkinInfo
														  , LPD3DXMESHCONTAINER *ppNewMeshContainer)
{
	D3DXMESHCONTAINER_DERIVED*			pMeshContainer = new D3DXMESHCONTAINER_DERIVED;

	if(NULL != Name)
		AllocateName(&pMeshContainer->Name, Name); //인자로 들어온 이름을 복사한다.

	pMeshContainer->vecMtrls.reserve(NumMaterials);
	pMeshContainer->vecTextures.reserve(NumMaterials);

	// 인자로 들어온 메시데이터를 =새롭게 만든 본메시에 저장한다.
	pMeshContainer->MeshData.pMesh = pMeshData->pMesh; // D3DXMesh
	pMeshContainer->MeshData.Type = pMeshData->Type;
	pMeshData->pMesh->AddRef();

	LPDIRECT3DDEVICE9		pGraphicDev = NULL;
	pMeshContainer->MeshData.pMesh->GetDevice(&pGraphicDev);

	for (DWORD i = 0; i < NumMaterials; ++i)
	{
		D3DXMATERIAL			Mtrl;
		memcpy(&Mtrl, &pMaterials[i], sizeof(D3DXMATERIAL));

		pMeshContainer->vecMtrls.push_back(Mtrl.MatD3D);

		LPDIRECT3DTEXTURE9			pTexture = NULL;
		TCHAR						szFullPath[128] = L"";

		if(NULL != Mtrl.pTextureFilename)
		{
			lstrcpy(szFullPath, m_szPath);

			TCHAR			szFileName[128] = L"";

			MultiByteToWideChar(CP_ACP, 0, Mtrl.pTextureFilename
				, strlen(Mtrl.pTextureFilename), szFileName, 128);

			lstrcat(szFullPath, szFileName);

			if(FAILED(D3DXCreateTextureFromFile(pGraphicDev, szFullPath, &pTexture)))
				return E_FAIL;

			pMeshContainer->vecTextures.push_back(pTexture);
		}
	}

	if(NULL != pSkinInfo)
	{
		pMeshContainer->pSkinInfo = pSkinInfo;
		pMeshContainer->pOriMesh = pMeshData->pMesh;
		pSkinInfo->AddRef();
		pMeshData->pMesh->AddRef();

		pMeshData->pMesh->CloneMeshFVF(D3DXMESH_MANAGED, pMeshData->pMesh->GetFVF()
			, pGraphicDev, &pMeshContainer->MeshData.pMesh);

		pMeshContainer->MeshData.pMesh->GetAttributeTable(NULL, &pMeshContainer->NumMaterials);

		pMeshContainer->pAttributeTable = new D3DXATTRIBUTERANGE[pMeshContainer->NumMaterials];

		pMeshContainer->MeshData.pMesh->GetAttributeTable(pMeshContainer->pAttributeTable, NULL);

		int			iNumBone = pSkinInfo->GetNumBones();

		pMeshContainer->pBoneoffsetMatrix = new D3DXMATRIX[iNumBone];
		pMeshContainer->pResultMatrix = new D3DXMATRIX[iNumBone];

		for (int i = 0; i < iNumBone; ++i)
		{
			pMeshContainer->pBoneoffsetMatrix[i] = *(pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i));
		}
	}

	*ppNewMeshContainer = pMeshContainer;

	return S_OK;
}

STDMETHODIMP Engine::CHierachyLoader::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	if(NULL != pFrameToFree)
	{
		if(NULL != pFrameToFree->pFrameSibling)
			DestroyFrame(pFrameToFree->pFrameSibling);
		if(NULL != pFrameToFree->pFrameFirstChild)
			DestroyFrame(pFrameToFree->pFrameFirstChild);

		if(NULL != pFrameToFree->Name)
		{
			Engine::safe_delete_array(pFrameToFree->Name);
		}

		if(NULL != pFrameToFree->pMeshContainer)
		{
			DestroyMeshContainer(pFrameToFree->pMeshContainer);
		}

		Engine::safe_delete(pFrameToFree);
	}

	return S_OK;
}

STDMETHODIMP Engine::CHierachyLoader::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	vector<D3DMATERIAL9>			vecMtrl;
	vector<LPDIRECT3DTEXTURE9>		vecTexture;

	D3DXMESHCONTAINER_DERIVED*		pBoneMesh = ((D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree);
	pBoneMesh->vecMtrls.swap(vecMtrl);
	vecMtrl.clear();

	for (unsigned int i = 0; i < ((D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree)->vecTextures.size(); ++i)
	{
		pBoneMesh->vecTextures[i]->Release();
		pBoneMesh->vecTextures[i] = NULL;
	}

	pBoneMesh->vecTextures.swap(vecTexture);
	vecTexture.clear();

	if(NULL != pBoneMesh->Name)
		Engine::safe_delete_array(((D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree)->Name);

	if(NULL != pBoneMesh->pAttributeTable)
		Engine::safe_delete_array(((D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree)->pAttributeTable);

	if(NULL != pBoneMesh->pBoneoffsetMatrix)
		Engine::safe_delete_array(((D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree)->pBoneoffsetMatrix);

	if(NULL != pBoneMesh->pResultMatrix)
		Engine::safe_delete_array(((D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree)->pResultMatrix);

	if(NULL != pBoneMesh->ppBoneMatrix)
		Engine::safe_delete_array(((D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree)->ppBoneMatrix);

	pBoneMesh->MeshData.pMesh->Release();

	pBoneMesh->pOriMesh->Release();


	pBoneMesh->pSkinInfo->Release();



	Engine::safe_delete(pBoneMesh);

	return S_OK;
}

CHierachyLoader* Engine::CHierachyLoader::Create(const TCHAR* pPath)
{
	CHierachyLoader*		pLoader = new CHierachyLoader(pPath);

	return pLoader;
}