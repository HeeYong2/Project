#ifndef GameObject_h__
#define GameObject_h__
/********************************************************************
	created:	2015/01/31
	file path:	Yong\Engine\Utility\Code
	file base:	GameObject
	file ext:	h
	author:		Yong
	purpose:	게임을 이루는 또는 레이어를 이루는 객체들의 부모
*********************************************************************/
#include "Engine_Defines.h"
#include "Component.h"
#include "Transform.h"
#include "Base.h"

namespace Engine
{
	class CLayer;
	class _declspec(dllexport) CGameObject
		: public Engine::CBase
	{
	public:
		explicit CGameObject(void);
		explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
		virtual ~CGameObject(void);

	public:
		Engine::CComponent* GetObjectComponent(const TCHAR* pComponentTag);

	public:
		virtual HRESULT Initialize(void) { return S_OK; }
		virtual void Update(void);
		virtual void Render(void) {};
		virtual _ulong Release(void);
	protected:
		map<const TCHAR* , CComponent*>				m_mapComponent;
		typedef map<const TCHAR * , CComponent*>	MAPCOM;

	protected:	
		LPDIRECT3DDEVICE9							m_pGraphicDev;
		Engine::CTransform*							m_pInfo;
		D3DMATERIAL9								m_MtrlInfo;

	};
}

#endif // GameObject_h__