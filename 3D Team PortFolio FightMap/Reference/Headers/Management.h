#ifndef Management_h__
#define Management_h__
/********************************************************************
	created:	2015/01/22
	file path:	Yong\Engine\Utility\Code
	file base:	Management
	file ext:	h
	author:		Yong	
	purpose:	Management
*********************************************************************/
#include "Engine_Defines.h"
#include "Renderer.h"
#include "Component.h"
#include "GameObject.h"

namespace Engine
{
	class CScene;
	class CLayer;
	class _declspec(dllexport) CManagement
	{
		DECLARE_SINGLETON(CManagement)
	private:
		explicit CManagement(void);
	public:
		~CManagement(void);

	public://getter
		Engine::CComponent* GetObjectComponent(const _int& iLayerID , const TCHAR* pObjectTag , const TCHAR* pComponentTag , const _ushort& wCnt);
		list<CGameObject*>*	GetCurrentList(const _int& iLayer, const TCHAR* pObjectTag);		
		list<CGameObject*>*	GetList(const _int& iLayer, const TCHAR* pObjectTag);		
	public: // setter
		void Add_RenderGroup(Engine::CRenderer::RENDER_GROUP GroupID, Engine::CGameObject* pGameObject);
	public:
		template <typename T> 
		HRESULT Replace_Current_Scene(LPDIRECT3DDEVICE9 pGraphicDev, T& Functor);	

	public:
		HRESULT Init_Management(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iSizeX, const _uint& iSizeY);
		void Update(void);
		void Render(void);

	private:
		CScene*					m_pCurrentScene;
		CRenderer*				m_pRenderer;
	private:
		void Release(void);
	};

	#include "Management.inl"

}

#endif // Management_h__