#ifndef LightMgr_h__
#define LightMgr_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class CLight;
class ENGINE_DLL CLightMgr
{
	DECLARE_SINGLETON(CLightMgr)
public:
	explicit CLightMgr(void);
	~CLightMgr(void);	
public:
	HRESULT	Ready_Light(LPDIRECT3DDEVICE9	pGraphicDev, const D3DLIGHT9* pLightInfo);
	void Render(LPD3DXEFFECT pEffect);
private:
	list<CLight*>				m_LightList;
	typedef list<CLight*>		LIGHTLIST;
private:
	void Release(void);
};



END

#endif // LightMgr_h__
