/*!
 * \class CBase
 *
 * \ingroup GroupName
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note 
 *
 * \author Administrator
 *
 * \version 1.0
 *
 * \date 4¿ù 2015
 *
 * Contact: user@company.com
 *
 */
#ifndef Base_h__
#define Base_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CBase
{
public:
	explicit CBase(void);
	virtual ~CBase(void);
public:
	 _ulong AddRef(void);
	virtual _ulong Release(void);
protected:
	_ulong						m_dwRefCnt;
};

END

#endif // Base_h__