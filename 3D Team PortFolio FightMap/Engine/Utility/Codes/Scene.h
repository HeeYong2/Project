#ifndef Scene_h__
#define Scene_h__
/********************************************************************
	created:	2015/01/31
	file path:	Yong\Engine\Utility\Code
	file base:	Scene
	file ext:	h
	author:		Yong
	purpose:	게임의 장면을 이루는 클래스들의 부모클래스다.
*********************************************************************/
#include "Engine_Defines.h"
#include "Layer.h"

namespace Engine
{
	class _declspec(dllexport) CScene
	{
	public:
		explicit CScene(void);
		explicit CScene(LPDIRECT3DDEVICE9 pGraphicDev);
		virtual ~CScene(void);

	public:
		Engine::CComponent* GetObjectComponent(const _int& iLayerID , const TCHAR* pObjectTag , const TCHAR* pComponentTag , const _ushort& wCnt);
		list<CGameObject*>*	GetCurrentList(const _int& iLayer, const TCHAR* pObjectTag);
		list<CGameObject*>*	GetList(const _int& iLayer, const TCHAR* pObjectTag);

	public:
		virtual HRESULT Initialize(void)PURE;
		virtual void Update(void);
		virtual void Render(void);
	protected:
		LPDIRECT3DDEVICE9			m_pGraphicDev;

	protected:
		map<_int, CLayer*>			m_mapLayer;
		typedef map<int, CLayer*>	MAPLAYER;
	
	private:
		void Release(void);
	};
}



#endif // Scene_h__
