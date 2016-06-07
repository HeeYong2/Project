#ifndef TriCol_h__
#define TriCol_h__
/********************************************************************
	created:	2015/04/29
	file path:	Yong\Engine\Resource\Code
	file base:	TriCol
	file ext:	h
	author:		Yong	
	purpose:	»ï°¢ÇüÀÇ Tricol
*********************************************************************/
#include "VIBuffer.h"

namespace Engine
{
	class _declspec(dllexport) CTriCol
		: public CVIBuffer
	{
	public:
		CTriCol(void);
		virtual ~CTriCol(void);

	public:
		virtual CResource* Clone_Resource(void) ;

		virtual HRESULT CreateVertexIndexBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
		static CTriCol* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	private:
		void Release(void);
	};

}
#endif // TriCol_h__
