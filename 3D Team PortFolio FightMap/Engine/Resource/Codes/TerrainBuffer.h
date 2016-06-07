#ifndef TerrainBuffer_h__
#define TerrainBuffer_h__
/********************************************************************
	created:	2015/02/04
	file path:	Yong\Engine\Resource\Code
	file base:	RcTex
	file ext:	h
	author:		Yong	
	purpose:	Create Terrain Buffer
*********************************************************************/

#include "VIBuffer.h"

namespace Engine
{
	class _declspec(dllexport) CTerrainBuffer
		: public CVIBuffer
	{
	public:
		explicit CTerrainBuffer(void);
		virtual ~CTerrainBuffer(void);

	public:
		virtual CResource* Clone_Resource(void);
		
	public:
		virtual HRESULT CreateVertexIndexBuffer(LPDIRECT3DDEVICE9 pGraphicDev
			, const _ulong& wCntX, const _ulong& wCntZ, const float& fItv);
		
	public:
		static CTerrainBuffer* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& wCntX, const _ulong& wCntZ, const float& fItv);

	public:
		void Release(void);

	private:
		_ulong		m_wVtxSizeX;
		_ulong		m_wVtxSizeZ;
		WORD		m_wVtxItv;

	};
}
#endif // TerrainBuffer_h__
