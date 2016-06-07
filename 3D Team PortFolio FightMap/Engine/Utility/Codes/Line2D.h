#ifndef Line2D_h__
#define Line2D_h__
/********************************************************************
	created:	2015/03/09
	file path:	Yong\Engine\Resource\Code
	file base:	Line2D
	file ext:	h
	author:		Yong
	purpose:	NaviMesh의 경로 판단
*********************************************************************/

#include "Engine_Defines.h"

namespace Engine
{
	class _declspec(dllexport) CLine2D
	{
	public:
		explicit CLine2D(void);
		~CLine2D(void);

	public:
		const _vec2* Get_Line_Start(void);
		const _vec2* Get_Line_End(void);

	public:
		HRESULT	Init_Line(const _vec2* pStart , const _vec2* pEnd);
		_bool isIn(const _vec2* pPosition , const _vec2* pDirection);
		void MakeSlide(const _vec2* pDirection, _vec3* pSlide);
	public:
		static CLine2D*	Create(const _vec2* pStart , const _vec2* pEnd );
		_vec3  GetNormal(void);

	private:
		_vec2					m_vStart;
		_vec2					m_vEnd;
		_vec2					m_vNormal;

	};
}


#endif // Line2D_h__
