#ifndef Camera_h__
#define Camera_h__
/********************************************************************
	created:	2015/02/05
	file path:	Yong\Engine\Utility\Code
	file base:	Camera
	file ext:	h
	author:		Yong
	purpose:	게임내에 사용할 수 있는 카메라 객체들의 부모클래스
*********************************************************************/
#include "GameObject.h"

namespace Engine
{
	class _declspec(dllexport) CCamera
		: public Engine::CGameObject
	{
	public:
		explicit CCamera(void);
		virtual ~CCamera(void);

	public:
		virtual HRESULT Initialize(LPDIRECT3DDEVICE9 pGraphicDev , const _vec3* pEye , const _vec3* pAt , const _vec3* pUp
			, _float fFov , _float fAspect , _float fNear , _float fFar) { return S_OK;}

		virtual void Update(void);

	protected:
		LPDIRECT3DDEVICE9				m_pGraphicDev;
		_vec3							m_vEye;
		_vec3							m_vAt;
		_vec3							m_vUp;
		_matrix							m_matView;

		_float							m_fAspect;
		_float							m_fFov;
		_float							m_fNear;
		_float							m_fFar;
		_matrix							m_matProjection;
		
	protected:
		_float							m_fCamSpeed;
	
	public:
		HRESULT							SetViewMatrix(void);
		HRESULT							SetProjMatrix(void);

	};
}
#endif // Camera_h__
