#ifndef Light_h__
#define Light_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CLight
{
public:
	explicit CLight(LPDIRECT3DDEVICE9 pGraphicDev);
	~CLight(void);	
public:
	HRESULT Init_Light(const D3DLIGHT9* pLightInfo, const _int& iIdx);
	void Render(LPD3DXEFFECT pEffect);
public:
	static CLight* Create(LPDIRECT3DDEVICE9	pGraphicDev, const D3DLIGHT9* pLightInfo, const _int& iIdx);
private:
	D3DLIGHT9						m_Light;
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	LPDIRECT3DVERTEXBUFFER9			m_pVB;	
	LPDIRECT3DINDEXBUFFER9			m_pIB;	
private:
	_int					m_iIndex;

};



END

#endif // Light_h__
