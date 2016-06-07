#ifndef Transform_h__
#define Transform_h__
/********************************************************************
	created:	2015/01/31
	file path:	Yong\Engine\Utility\Code
	file base:	Transform
	file ext:	h
	author:		Yong
	purpose:	ȭ�鿡 ����ϱ����� �뵵�� ��ü���� ������ �־���ϴ� ������
*********************************************************************/
#include "Component.h"

namespace Engine
{
	class _declspec(dllexport) CTransform 
		: public CComponent
	{
	public:
		enum ANGLE {ANGLE_X ,ANGLE_Y ,ANGLE_Z ,ANGLE_END};

	public:
		explicit CTransform(const _vec3* pGlobalLook);
		virtual ~CTransform(void);

	public:
		virtual HRESULT Initialize(void);
		virtual void Update(void);

	public:
		static CTransform* Create(const _vec3* pGlobalLook);

	public:
		_vec3				m_vScale;
		float				m_fAngle[ANGLE_END];
		_vec3				m_vPosition;
		_vec3				m_vDirection;
		_matrix				m_matWorld;
	};
}
#endif // Transform_h__
