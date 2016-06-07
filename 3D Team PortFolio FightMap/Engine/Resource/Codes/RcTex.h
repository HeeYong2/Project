#ifndef RcTex_h__
#define RcTex_h__
/********************************************************************
	created:	2015/01/31
	file path:	Yong\Engine\Resource\Code
	file base:	RcTex
	file ext:	h
	author:		Yong	
	purpose:	1x1 사각형 텍스쳐를 맵핑하기위한 버퍼
*********************************************************************/
#include "VIBuffer.h"

namespace Engine
{
	class _declspec(dllexport) CRcTex
		: public CVIBuffer
	{
	public:
		virtual CResource* Clone_Resource(void) ;
	public:
		explicit CRcTex(void);
		virtual ~CRcTex(void);
	public:
		virtual HRESULT CreateVertexIndexBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	public:
		static CRcTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	private:
		void Release(void);

	};
}
#endif // RcTex_h__
