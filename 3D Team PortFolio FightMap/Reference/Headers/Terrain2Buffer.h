#ifndef TerrainBuffer2_h__
#define TerrainBuffer2_h__
/********************************************************************
	created:	2015/04/17
	file path:	Yong\Engine\Resource\Code
	file base:	RcTex
	file ext:	h
	author:		Yong	
	purpose:	Create Terrain Buffer
*********************************************************************/

#include "VIBuffer.h"

namespace Engine
{
	class _declspec(dllexport) CTerrain2Buffer
		: public CVIBuffer
	{
	public:
		explicit CTerrain2Buffer(void);
		virtual ~CTerrain2Buffer(void);

	public:
		virtual CResource* Clone_Resource(void);
		
	public:
		virtual HRESULT CreateVertexIndexBuffer(LPDIRECT3DDEVICE9 pGraphicDev
			, const _ulong& wCntX, const _ulong& wCntZ, const float& fItv);
		
	public:
		static CTerrain2Buffer* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& wCntX, const _ulong& wCntZ, const float& fItv);

	public:
		void Release(void);

	private:
		_ulong		m_wVtxSizeX;
		_ulong		m_wVtxSizeZ;
		WORD		m_wVtxItv;

	};
}
#endif // TerrainBuffer2_h__
