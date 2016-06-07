#ifndef Renderer_h__
#define Renderer_h__
/********************************************************************
	created:	2015/02/05
	file path:	Yong\Engine\Utility\Code
	file base:	Renderer
	file ext:	h
	author:		Yong
	purpose:	그리기 관련일을 수행
*********************************************************************/
#include "Engine_Defines.h"

namespace Engine
{
	class CGameObject;
	class CScene;
	class _declspec(dllexport) CRenderer
	{
	public:
		enum RENDER_GROUP {RENDER_DEFERRED, RENDER_EFFECT, RENDER_UI, RENDER_END};

	public:
		explicit CRenderer(LPDIRECT3DDEVICE9 pGraphicDev);
		~CRenderer(void);

	public:
		void Receive_CurrentScene(CScene* pCurrentScene) {
			m_pCurrentScene = pCurrentScene;}
		void Add_RenderGroup(RENDER_GROUP GroupID, Engine::CGameObject* pGameObject){
			m_RenderGroup[GroupID].push_back(pGameObject);}

	public:
		HRESULT Init_Renderer(const _uint& iSizeX, const _uint& iSizeY);
		void Render_Deferred(void); // 디퍼드로 그랴야할 객첻르을 그린다.	
		void Render_Light(void); // 디퍼드로 그랴야할 객첻르을 그린다.	
		void Render(void);
	public:
		static CRenderer* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iSizeX, const _uint& iSizeY);

	private:
		CScene*					m_pCurrentScene;
		LPDIRECT3DDEVICE9		m_pGraphicDev;

	private:
		list<Engine::CGameObject*>			m_RenderGroup[RENDER_END];
		typedef list<Engine::CGameObject*>	RENDERLIST;

		LPDIRECT3DVERTEXBUFFER9			m_pVB;	
		LPDIRECT3DINDEXBUFFER9			m_pIB;	

	private:
		void Clear_RenderGroup(void);
		void Release(void);	
	};
}
#endif // Renderer_h__
