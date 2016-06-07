/*!
 * \class CRenderTarget
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

#ifndef RenderTarget_h__
#define RenderTarget_h__

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CRenderTarget : public Engine::CBase
{
private:
	explicit CRenderTarget(LPDIRECT3DDEVICE9 pGraphicDev);
	~CRenderTarget(void);
public://Getter
	LPDIRECT3DTEXTURE9 Get_Texture(void) {
		return m_pTargetTexture;}
public:
	HRESULT Init_RenderTarget(const _uint& iSizeX, const _uint& iSizeY, D3DFORMAT Format);
	HRESULT Init_DebugBuffer(const _float& fX, const _float& fY
		, const _float& fSizeX, const _float& fSizeY);
	void Begin(const _ulong& dwIndex);
	void End(const _ulong& dwIndex);
	void Render(void);
	_ulong Release(void);
public:
	static CRenderTarget* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iSizeX, const _uint& iSizeY, D3DFORMAT Format);
private:
	LPDIRECT3DDEVICE9				m_pGraphicDev;
	LPDIRECT3DTEXTURE9				m_pTargetTexture;
	LPDIRECT3DSURFACE9				m_pTargetSurface;
	LPDIRECT3DSURFACE9				m_pOldSurface;

	LPDIRECT3DVERTEXBUFFER9			m_pVB;	
	LPDIRECT3DINDEXBUFFER9			m_pIB;	

};

END


#endif // RenderTarget_h__