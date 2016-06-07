#ifndef Resource_h__
#define Resource_h__
/********************************************************************
	created:	2015/01/31
	file path:	Yong\Engine\Resource\Code
	file base:	Resource
	file ext:	h
	author:		Yong
	purpose:	¸®¼Ò½º
*********************************************************************/
#include "Component.h"

namespace Engine
{
	class _declspec(dllexport) CResource
		: public CComponent
	{
	public:
		virtual CResource* Clone_Resource(void) PURE;
	public:
		explicit CResource(void);
		virtual ~CResource(void);

	public:
		LPD3DXMESH GetMesh(void);

	protected:
		_ulong*		m_pRefCnt;

	protected:
		void Release(void);
	};
}

#endif // Resource_h__
