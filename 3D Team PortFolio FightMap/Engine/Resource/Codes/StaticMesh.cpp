#include "StaticMesh.h"

using namespace Engine;

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Engine::CStaticMesh::CStaticMesh(void)
: m_dwNumMtrl(0)
, m_pSubset(NULL)
, m_pMesh(NULL)
, m_pMaterial(NULL)
, m_ppTextures(NULL)
, m_ppNormalTex(NULL)
, m_ppSpecularTex(NULL)
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Engine::CStaticMesh::~CStaticMesh(void)
{
	Release();
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

HRESULT Engine::CStaticMesh::Ready_Mesh(LPDIRECT3DDEVICE9 pGraphicDev , const TCHAR* pPath , const TCHAR* pFileName)
{
	Engine::CMeshTextureCtrl::GetInstance()->InitMeshTextureCtrl(pGraphicDev);

	TCHAR	szFullPath[128] = L"";

	lstrcpy(szFullPath , pPath);
	lstrcat(szFullPath , pFileName);

	LPD3DXBUFFER pAdjacency = NULL;

	if(FAILED(D3DXLoadMeshFromX(szFullPath , 0 , pGraphicDev , NULL ,&m_pSubset , NULL , &m_dwNumMtrl , &m_pMesh)))
		return E_FAIL;

	m_pMaterial = new D3DMATERIAL9[m_dwNumMtrl];
	m_ppTextures = new LPDIRECT3DTEXTURE9[m_dwNumMtrl];
	m_ppSpecularTex = new LPDIRECT3DTEXTURE9[m_dwNumMtrl];
	m_ppNormalTex = new LPDIRECT3DTEXTURE9[m_dwNumMtrl];


	LPD3DXMATERIAL pSubSet = ((D3DXMATERIAL*)m_pSubset->GetBufferPointer());

	for(_ulong i = 0 ; i < m_dwNumMtrl ; ++i)
	{
		TCHAR		szFileName[128] = L"";
		TCHAR		szFullPath[128] = L"";

		m_pMaterial[i] = pSubSet[i].MatD3D;

		MultiByteToWideChar(CP_ACP , 0 , pSubSet[i].pTextureFilename , strlen(pSubSet[i].pTextureFilename)
			, szFileName , 128);

		lstrcpy(szFullPath , pPath);
		lstrcat(szFullPath , szFileName);

		Engine::CMeshTextureCtrl::GetInstance()->AddMeshTexture(CMeshTextureCtrl::TEX_DIF, pPath , szFileName, &m_ppTextures[i]);
		if(NULL == m_ppTextures[i])
			return E_FAIL;

// 		TCHAR			szNormalName[128] = L"";
// 		wsprintf(szNormalName , L"%d" , 1);
// 		lstrcat(szNormalName , szFileName);
// 		Engine::CMeshTextureCtrl::GetInstance()->AddMeshTexture(CMeshTextureCtrl::TEX_NOR , pPath , szNormalName , &m_ppNormalTex[i]);
// 
// 		TCHAR			szSpecularName[128] = L"";
// 		wsprintf(szSpecularName , L"%d" , 0);
// 		lstrcat(szSpecularName , szFileName);
// 		Engine::CMeshTextureCtrl::GetInstance()->AddMeshTexture(CMeshTextureCtrl::TEX_SPEC , pPath , szNormalName , &m_ppSpecularTex[i]);
// 
// 
// 		D3DVERTEXELEMENT9	VertexPNTBTElements[] = 
// 		{
// 			{0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
// 			{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
// 			{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
// 			{0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},
// 			{0, 44, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0},
// 			D3DDECL_END()
// 		};
// 
// 		LPD3DXMESH ppOutMesh = NULL;
// 		m_pMesh->CloneMesh(m_pMesh->GetOptions() , VertexPNTBTElements , pGraphicDev , &ppOutMesh);
// 
// 		if(SUCCEEDED(D3DXComputeTangentFrameEx(ppOutMesh,
// 			D3DDECLUSAGE_TEXCOORD, 0,
// 			D3DDECLUSAGE_BINORMAL, 0,
// 			D3DDECLUSAGE_TANGENT, 0,
// 			D3DDECLUSAGE_NORMAL, 0,
// 			NULL,
// 			(DWORD*)pAdjacency->GetBufferPointer(), 0.01f, 0.25f, 0.01f, &ppOutMesh, NULL)))
// 		{
// 			m_pMesh->Release();
// 			m_pMesh = ppOutMesh;
// 		}
	}
	return S_OK;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void  Engine::CStaticMesh::Render(LPDIRECT3DDEVICE9 pGraphicDev , const _matrix* pWorldMatrix)
{
	pGraphicDev->SetTransform(D3DTS_WORLD, pWorldMatrix);

	for(_ulong i = 0; i < m_dwNumMtrl ; ++i)
	{
		pGraphicDev->SetMaterial(&m_pMaterial[i]);
		pGraphicDev->SetTexture(0 , m_ppTextures[i]);

		m_pMesh->DrawSubset(i);
	}
}

void Engine::CStaticMesh::Render(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DXEFFECT pEffect)
{
	for(_ulong i = 0; i < m_dwNumMtrl ; ++i)
	{
		//pGraphicDev->SetMaterial(&m_pMaterial[i]);
		//pGraphicDev->SetTexture(0 , m_ppTextures[i]);
		pEffect->SetTexture("g_BaseTexture", m_ppTextures[i]);
		pEffect->CommitChanges();
		m_pMesh->DrawSubset(i);
	}
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CResource* Engine::CStaticMesh::Clone_Resource(void)
{
	// this : 현재 멤버함수를 호출한 객체의 주소다.
	CResource*			pStatic = new CStaticMesh(*this);

	++(*m_pRefCnt);

	return pStatic;	
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Engine::CStaticMesh::Release(void)
{
	if((*m_pRefCnt) == 0)
	{
		Engine::safe_delete_array(m_pMaterial);
		Engine::safe_delete_array(m_ppTextures);

		Engine::safe_release(m_pSubset);
		Engine::safe_release(m_pMesh);

		CMesh::Release();
	}
	else
		--(*m_pRefCnt);
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CStaticMesh* Engine::CStaticMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev , const TCHAR* pPath , const TCHAR* pFileName)
{
	CStaticMesh*		pStaticMesh = new CStaticMesh;

	if(FAILED(pStaticMesh->Ready_Mesh(pGraphicDev , pPath , pFileName)))
	{
		Engine::safe_delete(pStaticMesh);
		return NULL;
	}
	return pStaticMesh;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

LPD3DXMESH Engine::CStaticMesh::GetMesh(void)
{
	return m_pMesh;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

DWORD Engine::CStaticMesh::GetVtxNum(void)
{
	return m_pMesh->GetNumVertices();
}

DWORD Engine::CStaticMesh::GetVtxByte(void)
{
	return D3DXGetFVFVertexSize(m_pMesh->GetFVF());
}