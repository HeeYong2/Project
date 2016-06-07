#include "ResourceMgr.h"
#include "TerrainBuffer.h"
#include "RcTex.h"
#include "DynamicMesh.h"
#include "StaticMesh.h"
#include "Collider.h"
#include "Terrain2Buffer.h"
#include "TriCol.h"
#include "CubeBuffer.h"

USING(Engine)

IMPLEMENT_SINGLETON(CResourceMgr)

//------------------------------------------------------------------------------------------------------------------
CResourceMgr::CResourceMgr(void)
: m_dwSize(0)
{

}

//------------------------------------------------------------------------------------------------------------------
CResourceMgr::~CResourceMgr(void)
{
	Release();
}

//------------------------------------------------------------------------------------------------------------------
void CResourceMgr::Throw_Vertex_Info(_ulong ContainerIdx, const TCHAR* pResourceTag, /*Out*/void* pVtxInfo)
{
	MAPRESOURCE::iterator	iter = find_if(m_pMapResource[ContainerIdx].begin(), m_pMapResource[ContainerIdx].end(), CTag_Finder(pResourceTag));

	if(iter == m_pMapResource[ContainerIdx].end())
		return;

	((Engine::CVIBuffer*)iter->second)->Throw_Vertex_Info(pVtxInfo);
}
//------------------------------------------------------------------------------------------------------------------------------
map<const TCHAR*, CResource*>* Engine::CResourceMgr::GetMapResource(const _ulong& CounterIndex)
{
	return m_pMapResource;
}

//------------------------------------------------------------------------------------------------------------------
CResource* CResourceMgr::Clone_Resource(_ulong ContainerIdx, const TCHAR* pResourceTag)
{
	MAPRESOURCE::iterator	iter = find_if(m_pMapResource[ContainerIdx].begin(), m_pMapResource[ContainerIdx].end(), CResource_Finder(pResourceTag));

	if(iter == m_pMapResource[ContainerIdx].end())
		return NULL;

	return iter->second->Clone_Resource();
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
void CResourceMgr::Receive_Index_Info(_ulong ContainerIdx, const TCHAR* pResourceTag, /*In*/void* pIndex, _ulong* pTriCnt)
{
	MAPRESOURCE::iterator	iter = find_if(m_pMapResource[ContainerIdx].begin(), m_pMapResource[ContainerIdx].end(), CTag_Finder(pResourceTag));

	if(iter == m_pMapResource[ContainerIdx].end())
		return;

	((Engine::CVIBuffer*)iter->second)->Receive_Index_Info(pIndex, pTriCnt);
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
void Engine::CResourceMgr::Resource_Reset(_int iResourceIdx)
{
	for_each(m_pMapResource[iResourceIdx].begin(), m_pMapResource[iResourceIdx].end(), CDelete_Pair());
	m_pMapResource[iResourceIdx].clear();
}

//------------------------------------------------------------------------------------------------------------------
void CResourceMgr::Reserve_Container(_ulong dwSize)
{
	m_pMapResource = new MAPRESOURCE[dwSize];
	m_dwSize = dwSize;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CResourceMgr::Ready_Buffers(LPDIRECT3DDEVICE9 pGraphicDev, _ulong ContainerIdx, CVIBuffer::BUFFERTYPE BufferType, const TCHAR* pBufferTag )
{
	if(0 == m_dwSize )
	{
		msg_box("Container was not Reserved");
		return E_FAIL;
	}

	MAPRESOURCE::iterator	iter = find_if(m_pMapResource[ContainerIdx].begin(), m_pMapResource[ContainerIdx].end(), CResource_Finder(pBufferTag));

	if(iter != m_pMapResource[ContainerIdx].end())
	{
		return S_OK;
	}

	CVIBuffer*			pBuffer = NULL;

	switch(BufferType)
	{
	case CVIBuffer::TYPE_RCTEX:
		pBuffer = CRcTex::Create(pGraphicDev);
		break;

	case CVIBuffer::TYPE_TRICOL:
		pBuffer = CTriCol::Create(pGraphicDev);
		break;


	case CVIBuffer::TYPE_CUBE:
		pBuffer = CCubeBuffer::Create(pGraphicDev);
		break;

	}

	if(NULL == pBuffer)
		return E_FAIL;

	m_pMapResource[ContainerIdx].insert(MAPRESOURCE::value_type(pBufferTag, pBuffer));

	return S_OK;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CResourceMgr::Ready_Terrain_Buffers(LPDIRECT3DDEVICE9 pGraphicDev, _ulong ContainerIdx, const TCHAR* pBufferTag, const _ulong& dwCntX, const _ulong& dwCntZ, const _float& fItv, _int iNum)
{
	if(m_dwSize == 0)
	{
		msg_box("Container was not Reserved");
		return E_FAIL;
	}

	MAPRESOURCE::iterator	iter = find_if(m_pMapResource[ContainerIdx].begin(), m_pMapResource[ContainerIdx].end(), CResource_Finder(pBufferTag));

	if(iter != m_pMapResource[ContainerIdx].end())
	{
		return S_OK;
	}

	CVIBuffer*			pBuffer = NULL;
	if(iNum == 0)
		pBuffer = CTerrainBuffer::Create(pGraphicDev, dwCntX, dwCntZ, fItv);
	else
		pBuffer = CTerrain2Buffer::Create(pGraphicDev, dwCntX, dwCntZ, fItv);

	if(NULL == pBuffer)
		return E_FAIL;

	m_pMapResource[ContainerIdx].insert(MAPRESOURCE::value_type(pBufferTag, pBuffer));

	return S_OK;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT Engine::CResourceMgr::Ready_Textures(LPDIRECT3DDEVICE9 pGraphicDev, _ulong ContainerIdx, CTexture::TEXTURETYPE TextureType , const TCHAR* pTextureTag, const TCHAR* pPath, const TCHAR* pFileName, const _ulong& dwCnt)
{
	if(m_dwSize == 0)
	{
		msg_box("Container was not Reserved");
		return E_FAIL;
	}

	MAPRESOURCE::iterator	iter = find_if(m_pMapResource[ContainerIdx].begin(), m_pMapResource[ContainerIdx].end(), CResource_Finder(pTextureTag));

	if(iter != m_pMapResource[ContainerIdx].end())
	{
		return S_OK;
	}

	CTexture*			pTexture = CTexture::Create(pGraphicDev, TextureType, pPath, pFileName, dwCnt);

	if(NULL == pTexture)
		return E_FAIL;

	m_pMapResource[ContainerIdx].insert(MAPRESOURCE::value_type(pTextureTag, pTexture));

	return S_OK;	
}


HRESULT Engine::CResourceMgr::Ready_TexturesPath(LPDIRECT3DDEVICE9 pGraphicDev, _ulong ContainerIdx, CTexture::TEXTURETYPE TextureType , const TCHAR* pTextureTag, const TCHAR* pFullPath)
{
	if(m_dwSize == 0)
	{
		msg_box("Container was not Reserved");
		return E_FAIL;
	}

	MAPRESOURCE::iterator	iter = find_if(m_pMapResource[ContainerIdx].begin(), m_pMapResource[ContainerIdx].end(), CResource_Finder(pTextureTag));

	if(iter != m_pMapResource[ContainerIdx].end())
	{
		return S_OK;
	}

	CTexture*			pTexture = CTexture::CreatePath(pGraphicDev, TextureType, pFullPath);

	if(NULL == pTexture)
		return E_FAIL;

	m_pMapResource[ContainerIdx].insert(MAPRESOURCE::value_type(pTextureTag, pTexture));

	return S_OK;	
}


//---------------------------------------------------------------------------------------------------------------------------------------

HRESULT Engine::CResourceMgr::Ready_Mesh(LPDIRECT3DDEVICE9 pGraphicDev , _ulong ContainerIdx , CMesh::MESHTYPE MeshType , const TCHAR* pMeshTag , const TCHAR* pPath , const TCHAR* pFileName)
{
	if(m_dwSize == 0)
	{
		msg_box("Container was not Reserved");
		return E_FAIL;
	}

	MAPRESOURCE::iterator iter = find_if(m_pMapResource[ContainerIdx].begin() , m_pMapResource[ContainerIdx].end() , CResource_Finder(pMeshTag));

	if(iter != m_pMapResource[ContainerIdx].end())
	{
		return S_OK;
	}

	CMesh*		pMesh = NULL;

	switch(MeshType)
	{
	case CMesh::TYPE_STATIC:
		pMesh = CStaticMesh::Create(pGraphicDev , pPath , pFileName);
		break;

	case CMesh::TYPE_DYNAMIC:
		pMesh = CDynamicMesh::Create(pGraphicDev , pPath , pFileName);
		break;

	case CMesh::TYPE_COLLIDER:
		pMesh = CCollider::Create(pGraphicDev , pPath , pFileName);
		break;
	}

	if(pMesh == NULL)
		return E_FAIL;

	m_pMapResource[ContainerIdx].insert(MAPRESOURCE::value_type(pMeshTag , pMesh));

	return S_OK;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------
LPD3DXMESH CResourceMgr::GetMesh(_ulong ContainerIdx , const TCHAR* pMeshTag )
{
//  	MAPRESOURCE::iterator iter = m_pMapResource->begin();
//  	MAPRESOURCE::iterator iter_end = m_pMapResource->end();
//  
	MAPRESOURCE::iterator	iter = find_if(m_pMapResource[ContainerIdx].begin() , m_pMapResource[ContainerIdx].end() , CResource_Finder(pMeshTag));
	
	return	((CStaticMesh*)(*iter).second)->GetMesh();
}

//------------------------------------------------------------------------------------------------------------------------------------------------------
void CResourceMgr::Release(void)
{
	for (_ulong i = 0; i < m_dwSize; ++i)
	{
		for_each(m_pMapResource[i].begin(), m_pMapResource[i].end(), CDelete_Pair());
		m_pMapResource[i].clear();
	}	
	Engine::safe_delete_array(m_pMapResource);
}
