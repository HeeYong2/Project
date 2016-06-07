#ifndef RcCol_h__
#define RcCol_h__
/********************************************************************
	created:	2015/04/29
	file path:	Yong\Engine\Resource\Code
	file base:	RcCol
	file ext:	h
	author:		Yong	
	purpose:	네비메시 전용 버퍼
*********************************************************************/
#include "VIBuffer.h"

namespace Engine
{
	class _declspec(dllexport) CRcCol
		: public CVIBuffer
	{
	public:
		CRcCol(void);
		virtual ~CRcCol(void);

	public:
		virtual CResource* Clone_Resource(void) ;

		virtual HRESULT CreateVertexIndexBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
		static CRcCol* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	private:
		void Release(void);
	};

}
#endif // RcCol_h__
