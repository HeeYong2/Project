#include "TimeMgr.h"

USING(Engine)

IMPLEMENT_SINGLETON(CTimeMgr)

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CTimeMgr::CTimeMgr(void)
: m_fTime(0.f)
{
	m_FrameTime.QuadPart = 0;
	m_FixTime.QuadPart = 0;
	m_CpuTick.QuadPart = 0;
	m_LastTime.QuadPart = 0;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CTimeMgr::~CTimeMgr(void)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTimeMgr::InitTime( void )
{
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);
	QueryPerformanceFrequency(&m_CpuTick);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CTimeMgr::GetTime( void )
{
	return m_fTime;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CTimeMgr::SetTime( void )
{
	QueryPerformanceCounter(&m_FrameTime);

	if(m_FrameTime.QuadPart - m_LastTime.QuadPart > m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);

		m_LastTime.QuadPart = m_FrameTime.QuadPart;
	}

	m_fTime = float(m_FrameTime.QuadPart - m_FixTime.QuadPart) / m_CpuTick.QuadPart;

	m_FixTime = m_FrameTime;
}


