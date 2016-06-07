#ifndef Component_h__
#define Component_h__
/********************************************************************
	created:	2015/01/31
	file path:	Yong\Engine\Utility\Code
	file base:	Component
	file ext:	h
	author:		Yong
	purpose:	객체를 이루는 부품들의 부모이다(ex. 트랜스폼(변환), 리소스(텍스쳐, 메시, 버퍼) 등등등등
*********************************************************************/
#include "Engine_Defines.h"

namespace Engine
{
	class _declspec(dllexport) CComponent
	{
	public:
		explicit CComponent(void);
		virtual ~CComponent(void);
	public:
		virtual HRESULT Initialize(void) {return S_OK;}
		virtual void Update(void) { return ;}
		virtual void Render(LPDIRECT3DDEVICE9 pGraphicDev, const _ulong& dwCnt = 0) {}

	};
}

#endif // Component_h__
