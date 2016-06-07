#include "Base.h"

USING(Engine)

CBase::CBase(void)
: m_dwRefCnt(0)
{

}

CBase::~CBase(void)
{

}

_ulong CBase::AddRef(void)
{
	++m_dwRefCnt;

	return m_dwRefCnt;
}

_ulong CBase::Release(void)
{
	_ulong	dwPreRefCnt = 0;

	dwPreRefCnt = m_dwRefCnt;

	--m_dwRefCnt;

	return dwPreRefCnt;
}
