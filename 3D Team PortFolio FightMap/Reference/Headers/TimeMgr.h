#ifndef TimeMgr_h__
#define TimeMgr_h__

/********************************************************************
	created:	2015/02/09
	file path:	Yong\Engine\System\Code
	file base:	DxInput
	file ext:	h
	author:		Yong
	purpose:	게임내에 사용되는 시간을 관리한다.
*********************************************************************/
#include "Engine_Defines.h"

namespace Engine
{
	class _declspec(dllexport) CTimeMgr
	{
	public:
		DECLARE_SINGLETON(CTimeMgr)

	public:
		CTimeMgr(void);
		~CTimeMgr(void);
	public:
		float GetTime(void);
	public:
		void InitTime(void);
		void SetTime(void);
	private:
		LARGE_INTEGER		m_FrameTime;
		LARGE_INTEGER		m_FixTime;
		LARGE_INTEGER		m_LastTime;
		LARGE_INTEGER		m_CpuTick;
		float				m_fTime;
	};
}

#endif // TimeMgr_h__