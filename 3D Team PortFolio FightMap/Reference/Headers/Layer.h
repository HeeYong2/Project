#ifndef Layer_h__
#define Layer_h__
/********************************************************************
	created:	2015/01/31
	file path:	Yong\Engine\Utility\Code
	file base:	Layer
	file ext:	h
	author:		Yong	
	purpose:	Scene을 구성하는 층
*********************************************************************/
#include "Engine_Defines.h"
#include "GameObject.h"

namespace Engine
{
	class _declspec(dllexport) CLayer
	{
	public:
		explicit CLayer(void);
		~CLayer(void);	

	public://getter
		Engine::CComponent* GetObjectComponent(const TCHAR* pObjectTag , const TCHAR* pComponentTag , const _ushort& wCnt);
		list<CGameObject*>*		GetCurrentList(const TCHAR* pObjectTag);
		list<CGameObject*>*		GetList(const TCHAR* pObjectTag);

	public:
		HRESULT Insert_GameObject(const TCHAR* pObjectTag, CGameObject* pGameObejct);
		void Update(void);
		void Render(void);

	public:
		static CLayer* Create(void);

	private:
		map<const TCHAR*, list<CGameObject*>>				m_mapObject;
		typedef list<CGameObject*>							LISTOBJECT;
		typedef map<const TCHAR*, list<CGameObject*>>		MAPLIST;

	private:
		void Release(void);
	};

	class CGameObject_Finder
	{
	public:
		explicit CGameObject_Finder(const TCHAR* pTag) : m_pTag(pTag) {}
		~CGameObject_Finder(void) {}
	public:
		template <typename T>
		bool operator () (T& Pair)
		{
			_int iResult = lstrcmp(Pair.first, m_pTag);

			if(0 == iResult)
				return true;
			return false;
		}

	private:
		const TCHAR*				m_pTag;
	};
}

#endif // Layer_h__
