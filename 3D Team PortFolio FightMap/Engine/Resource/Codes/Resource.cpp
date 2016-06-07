#include "Resource.h"

using namespace Engine;

//------------------------------------------------------------------------------------------------------------------------
CResource::CResource(void)
:m_pRefCnt(new _ulong(0))
{

}


//------------------------------------------------------------------------------------------------------------------------
CResource::~CResource(void)
{

}

void CResource::Release(void)
{
	if(m_pRefCnt != NULL)
	{
		Engine::safe_delete(m_pRefCnt);
	}
}