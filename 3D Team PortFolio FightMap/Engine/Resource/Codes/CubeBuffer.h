#ifndef CubeBuffer_h__
#define CubeBuffer_h__
/********************************************************************
	created:	2015/05/01
	file path:	Yong\Engine\Resource\Code
	file base:	CubeBuffer
	file ext:	h
	author:		Yong	
	purpose:	Create CubeBuffer
*********************************************************************/
#include "VIBuffer.h"

namespace Engine
{
	class _declspec(dllexport) CCubeBuffer
		: public CVIBuffer
	{
	public:
		explicit CCubeBuffer(void);
		~CCubeBuffer(void);

	public:
		virtual CResource*	Clone_Resource(void);
		virtual HRESULT CreateVertexIndexBuffer(LPDIRECT3DDEVICE9 pGraphicDev);

	public:
		static CVIBuffer*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

	private:
		void Release(void);
	};
}

#endif // CubeBuffer_h__
