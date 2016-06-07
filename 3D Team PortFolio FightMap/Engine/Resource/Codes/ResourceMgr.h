#ifndef ResourceMgr_h__
#define ResourceMgr_h__
/********************************************************************
	created:	2015/01/31
	file path:	Yong\Engine\Resource\Code
	file base:	ResourceMgr
	file ext:	h
	author:		Yong	
	purpose:	리소스들을 관리하는 클래스
*********************************************************************/
#include "Engine_Defines.h"
#include "VIBuffer.h"
#include "Texture.h"
#include "Mesh.h"

namespace Engine
{
	class _declspec(dllexport) CResourceMgr
	{
		DECLARE_SINGLETON(CResourceMgr)
	private:
		explicit CResourceMgr(void);
	public:
		~CResourceMgr(void);

	public:
		CResource * Clone_Resource(_ulong ContainerIdx , const TCHAR* pResourceTag);
		void Throw_Vertex_Info(_ulong ContainerIdx, const TCHAR* pResourceTag, /*Out*/void* pVtxInfo);
		map<const TCHAR*, CResource*>* GetMapResource(const _ulong& CounterIndex = 0);

	public: // Setter
		void Receive_Index_Info(_ulong ContainerIdx, const TCHAR* pResourceTag, /*In*/void* pIndex, _ulong* pTriCnt);

	public:
		void Resource_Reset(_int iResourceIdx);
		void Reserve_Container(_ulong dwSize);
		HRESULT Ready_Buffers(LPDIRECT3DDEVICE9 pGraphicDev, _ulong ContainerIdx, CVIBuffer::BUFFERTYPE BufferType, const TCHAR* pBufferTag);
		HRESULT Ready_Terrain_Buffers(LPDIRECT3DDEVICE9 pGraphicDev, _ulong ContainerIdx, const TCHAR* pBufferTag, const _ulong& dwCntX, const _ulong& dwCntZ, const _float& fItv , _int iNum);
		HRESULT Ready_Textures(LPDIRECT3DDEVICE9 pGraphicDev, _ulong ContainerIdx, CTexture::TEXTURETYPE TextureType , const TCHAR* pTextureTag
			, const TCHAR* pPath, const TCHAR* pFileName, const _ulong& dwCnt = 1); 
		HRESULT Ready_TexturesPath(LPDIRECT3DDEVICE9 pGraphicDev, _ulong ContainerIdx, CTexture::TEXTURETYPE TextureType , const TCHAR* pTextureTag, const TCHAR* pFullPath); 
		HRESULT Ready_Mesh(LPDIRECT3DDEVICE9 pGraphicDev , _ulong ContainerIdx , CMesh::MESHTYPE MeshType , const TCHAR* pMeshTag , const TCHAR* pPath , const TCHAR* pFileName);
		LPD3DXMESH GetMesh(_ulong ContainerIdx , const TCHAR* pMeshTag);

	private:
		map<const TCHAR*, CResource*>*			m_pMapResource;
		typedef map<const TCHAR*, CResource*>	MAPRESOURCE;
	private:
		_ulong									m_dwSize;
	private:
		void Release(void);
	};

	class CResource_Finder
	{
	public:
		explicit CResource_Finder(const TCHAR* pTag) : m_pTag(pTag) {}
		~CResource_Finder(void) {}

	public:
		template <typename T>
		bool operator () (T& Pair)
		{
			_int iResult = lstrcmp(Pair.first , m_pTag);

			if(iResult == 0)
				return true;
			return false;
		}

	private:
		const TCHAR*				m_pTag;
	};
}

#endif // ResourceMgr_h__
