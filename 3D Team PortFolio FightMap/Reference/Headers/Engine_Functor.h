#ifndef Engine_Functor_h__
#define Engine_Functor_h__

namespace Engine
{
	class CTag_Finder
	{
	public:
		explicit CTag_Finder(const TCHAR* pFindTag) : m_pFindTag(pFindTag) {}
		~CTag_Finder(void) {}
	public:
		template <typename T>
		bool operator () (T& Pair)
		{
			_int iResult = lstrcmp(Pair.first, m_pFindTag);

			if(0 == iResult)
				return true;
			return false;
		}
	private:
		const TCHAR*		m_pFindTag;

	};
	// For Release Functor	
	class CRelease_Single
	{
	public:
		explicit CRelease_Single(void) {}
		~CRelease_Single(void) {}
	public:
		template <typename T>
		void operator ( ) (T& Instance)
		{
			if(NULL != Instance)
			{
				Instance->Release();
				Instance = NULL;
			}
		}
	};

	class CDelete_Single
	{
	public:
		explicit CDelete_Single(void) {}
		~CDelete_Single(void) {}
	public:
		template <typename T>
		void operator ( ) (T& pInstance)
		{
			if(NULL != pInstance)
			{
				delete pInstance;
				pInstance = NULL;
			}
		}
	};

	class CDelete_Array_Single_Functor
	{

	};

	class CDelete_Pair
	{
	public:
		explicit CDelete_Pair(void) {}
		~CDelete_Pair(void) {}
	public:
		template <typename T>
		void operator ( ) (T& Pair)
		{
			if(NULL != Pair.second)
			{
				delete Pair.second;
				Pair.second = NULL;
			}
		}
	};

	class CRelease_Pair
	{
	public:
		explicit CRelease_Pair(void) {}
		~CRelease_Pair(void) {}
	public:
		template <typename T>
		void operator ( ) (T& Pair)
		{
			if(NULL != Pair.second)
			{
				if(Engine::safe_release(Pair.second))
					msg_box("safe_release Failed");
			}
		}
	};
}

#endif // Engine_Functor_h__
