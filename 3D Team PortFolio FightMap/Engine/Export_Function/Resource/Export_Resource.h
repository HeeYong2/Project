#ifndef Export_Resource_h__
#define Export_Resource_h__
/********************************************************************
	created:	2015/01/31
	file path:	Yong\Client\Code
	file base:	Export_Function
	file ext:	h
	author:		Yong	
	purpose:	클라에서 사용해야할 자원 텍스쳐, 버퍼, 메시
*********************************************************************/
#include "../../Reference/Headers/ResourceMgr.h"

namespace Engine
{
	//=For.Resource.Getter===================================================================
	inline Engine::CResource* Clone_Resource(_ulong ContainerIdx , const TCHAR* pResourceTag);
	inline void Throw_Vertex_Info(_ulong ContainerIdx, const TCHAR* pResourceTag, /*Out*/void* pVtxInfo);
	inline map<const TCHAR*, Engine::CResource*>* GetMapResource(const _ulong& CounterIndex = 0);

	//=For.Resource.Setter===================================================================
	inline void Receive_Index_Info(_ulong ContainerIdx, const TCHAR* pResourceTag, /*In*/void* pIndex, _ulong* pTriCnt);
	
	//=For.Resource.General==================================================================
	inline void Reserve_Container(_ulong dwSize);
	inline void Resource_Reset(_int iResourceIdx);
	inline HRESULT Ready_Buffers(LPDIRECT3DDEVICE9 pGraphicDev, _ulong ContainerIdx, CVIBuffer::BUFFERTYPE BufferType, const TCHAR* pBufferTag);
	inline HRESULT Ready_Terrain_Buffers(LPDIRECT3DDEVICE9 pGraphicDev, _ulong ContainerIdx, const TCHAR* pBufferTag, const _ulong& dwCntX, const _ulong& dwCntZ, const _float& fItv , _int iNum);
	inline HRESULT Ready_Textures(LPDIRECT3DDEVICE9 pGraphicDev, _ulong ContainerIdx, CTexture::TEXTURETYPE TextureType
		, const TCHAR* pTextureTag, const TCHAR* pPath, const TCHAR* pFileName, const _ulong& dwCnt = 1);
	inline HRESULT Ready_TexturesPath(LPDIRECT3DDEVICE9 pGraphicDev, _ulong ContainerIdx, CTexture::TEXTURETYPE TextureType , const TCHAR* pTextureTag, const TCHAR* pFullPath);
	inline HRESULT Ready_Mesh(LPDIRECT3DDEVICE9 pGraphicDev , _ulong ContainerIdx , CMesh::MESHTYPE MeshType
		, const TCHAR* pMeshTag , const TCHAR* pPath , const TCHAR* pFileName);
	inline void Resource_Release(void);
}

#include "Export_Resource.inl"

#endif // Export_Resource_h__