//=For.Resource.Getter==================================================================

inline Engine::CResource* Engine::Clone_Resource(_ulong ContainerIdx , const TCHAR* pResourceTag)
{
	return CResourceMgr::GetInstance()->Clone_Resource(ContainerIdx , pResourceTag);
}

inline void Engine::Throw_Vertex_Info(_ulong ContainerIdx, const TCHAR* pResourceTag, /*Out*/void* pVtxInfo)
{
	CResourceMgr::GetInstance()->Throw_Vertex_Info(ContainerIdx, pResourceTag, pVtxInfo);
}

inline map<const TCHAR*, Engine::CResource*>* Engine::GetMapResource(const _ulong& CounterIndex)
{
	return Engine::CResourceMgr::GetInstance()->GetMapResource(CounterIndex);
}
//=For.Resource.Setter==================================================================

 inline void Engine::Receive_Index_Info(_ulong ContainerIdx, const TCHAR* pResourceTag, /*In*/void* pIndex, _ulong* pTriCnt)
 {
 	CResourceMgr::GetInstance()->Receive_Index_Info(ContainerIdx, pResourceTag, pIndex, pTriCnt);
 }

//=For.Resource.General==================================================================
inline void Engine::Reserve_Container(_ulong dwSize)
{
	CResourceMgr::GetInstance()->Reserve_Container(dwSize);
}

inline void Engine::Resource_Reset(_int iResourceIdx)
{
	CResourceMgr::GetInstance()->Resource_Reset(iResourceIdx);
}

inline HRESULT Engine::Ready_Buffers(LPDIRECT3DDEVICE9 pGraphicDev, _ulong ContainerIdx, CVIBuffer::BUFFERTYPE BufferType, const TCHAR* pBufferTag)
{
	return CResourceMgr::GetInstance()->Ready_Buffers(pGraphicDev, ContainerIdx, BufferType, pBufferTag);
}

inline HRESULT Engine::Ready_Terrain_Buffers(LPDIRECT3DDEVICE9 pGraphicDev, _ulong ContainerIdx, const TCHAR* pBufferTag, const _ulong& dwCntX, const _ulong& dwCntZ, const _float& fItv , _int iNum)
{
	return CResourceMgr::GetInstance()->Ready_Terrain_Buffers(pGraphicDev, ContainerIdx, pBufferTag, dwCntX, dwCntZ, fItv , iNum);
}

inline HRESULT Engine::Ready_Textures(LPDIRECT3DDEVICE9 pGraphicDev, _ulong ContainerIdx, CTexture::TEXTURETYPE TextureType , const TCHAR* pTextureTag, const TCHAR* pPath, const TCHAR* pFileName, const _ulong& dwCnt /* = 1 */)
{
	return CResourceMgr::GetInstance()->Ready_Textures(pGraphicDev ,ContainerIdx , TextureType , pTextureTag , pPath , pFileName , dwCnt);
}

inline HRESULT Engine::Ready_TexturesPath(LPDIRECT3DDEVICE9 pGraphicDev, _ulong ContainerIdx, CTexture::TEXTURETYPE TextureType , const TCHAR* pTextureTag, const TCHAR* pFullPath)
{
	return CResourceMgr::GetInstance()->Ready_TexturesPath(pGraphicDev ,ContainerIdx , TextureType , pTextureTag , pFullPath);
}

inline HRESULT Engine::Ready_Mesh(LPDIRECT3DDEVICE9 pGraphicDev , _ulong ContainerIdx , CMesh::MESHTYPE MeshType , const TCHAR* pMeshTag , const TCHAR* pPath , const TCHAR* pFileName)
{
	return CResourceMgr::GetInstance()->Ready_Mesh(pGraphicDev, ContainerIdx, MeshType, pMeshTag, pPath, pFileName);
}

inline void Engine::Resource_Release(void)
{
	CResourceMgr::GetInstance()->DestroyInstance();
}
//------------------------------------------------------------------------------------------------------------
