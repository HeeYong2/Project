#ifndef VIBuffer_h__
#define VIBuffer_h__
/********************************************************************
	created:	2015/01/31
	file path:	Yong\Engine\Resource\Code
	file base:	VIBuffer
	file ext:	h
	author:		Yong
	purpose:	버텍스, 인덱스 버퍼들의 부모
*********************************************************************/
#include "Resource.h"

namespace Engine
{
	class _declspec(dllexport) CVIBuffer
		: public CResource
	{
	public:	
		enum BUFFERTYPE {TYPE_RCTEX, TYPE_CUBE, TYPE_TRICOL};
	public:
		explicit CVIBuffer(void);
		virtual ~CVIBuffer(void);
	public: // Getter
		void Throw_Vertex_Info(/*Out*/void* pVtxInfo);
	public: // Setter
		void Receive_Index_Info(/*In*/void* pIndex, _ulong* pTriCnt);
		void SetVtxInfo(void* pVtxTex);
	public:	
		void Render(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCnt = 0);
	public:
		virtual HRESULT CreateVertexIndexBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
		virtual CResource* Clone_Resource(void)PURE;

	protected:
		LPDIRECT3DVERTEXBUFFER9			m_pVB;
		_ulong							m_dwVtxSize;
		_ulong							m_dwVtxFVF;
		_ulong							m_dwVtxCnt;
	protected:
		LPDIRECT3DINDEXBUFFER9			m_pIB;
		D3DFORMAT						m_IdxFmt;
		_ulong							m_dwTriCnt;
		_ulong							m_dwIdxSize;

	protected:
		void Release(void);
	};
}

#endif // VIBuffer_h__
