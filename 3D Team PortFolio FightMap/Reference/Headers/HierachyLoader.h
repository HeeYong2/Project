#ifndef HierachyLoader_h__
#define HierachyLoader_h__
/********************************************************************
	created:	2015/03/13
	file path:	Yong\Engine\Resource\Code
	file base:	HierachLoader
	file ext:	h
	author:		Yong
	purpose:	HierachLoader
*********************************************************************/
#include "Engine_Defines.h"

namespace Engine
{
	class _declspec(dllexport) CHierachyLoader
		: public ID3DXAllocateHierarchy
	{
	public:
		explicit CHierachyLoader(const TCHAR* pPath);
		~CHierachyLoader(void);
	public:
		STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, 
			LPD3DXFRAME *ppNewFrame) ;
		STDMETHOD(CreateMeshContainer)(THIS_ 
			LPCSTR Name, 
			CONST D3DXMESHDATA *pMeshData, 
			CONST D3DXMATERIAL *pMaterials, 
			CONST D3DXEFFECTINSTANCE *pEffectInstances, 
			DWORD NumMaterials, 
			CONST DWORD *pAdjacency, 
			LPD3DXSKININFO pSkinInfo, 
			LPD3DXMESHCONTAINER *ppNewMeshContainer) ;
		STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree) ;
		STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree) ; 
	public:
		HRESULT AllocateName(char** ppDestName, const char* pFrameName);
	public:
		static CHierachyLoader* Create(const TCHAR* pPath);
	private:
		const TCHAR*				m_szPath;
	};
}

#endif // HierachyLoader_h__
