/*!
 * \class CRenderTargetMgr
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
 * \date 4월 2015
 *
 * Contact: user@company.com
 *
 */
#ifndef RenderTargetMgr_h__
#define RenderTargetMgr_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class CRenderTarget;
class ENGINE_DLL CRenderTargetMgr
{
	DECLARE_SINGLETON(CRenderTargetMgr)
private:
	explicit CRenderTargetMgr(void);
	~CRenderTargetMgr(void);
public: // Getter
	LPDIRECT3DTEXTURE9		Get_Texture(const TCHAR* pTargetTag);
	LPDIRECT3DSURFACE9		Get_Surface(const TCHAR* pTargetKey);

public:
	CRenderTarget* Find_Target(const TCHAR* pTargetTag);
	list<CRenderTarget*>* Find_MRT(const TCHAR* pMRTTag);
public:
	HRESULT Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pTargetTag, const _uint& iSizeX, const _uint& iSizeY, D3DFORMAT Format);
	HRESULT Init_DebugBuffer(const TCHAR* pTargetTag, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY);
	HRESULT Ready_MRT(const TCHAR* pMRTTag, const TCHAR* pTargetTag);
	void Begin(const TCHAR* pMRTTag);
	void End(const TCHAR* pMRTTag);
	void Render_Debug(void);

private:
	map<const TCHAR*, CRenderTarget*>					m_mapRenderTarget;
	typedef map<const TCHAR*, CRenderTarget*>			MAPTARGET;

	// 특정 렌더링을 수행할때 사용해야할 렌더타겟들의 집합
	map<const TCHAR*, list<CRenderTarget*>>				m_mapMRT; 
	typedef map<const TCHAR*, list<CRenderTarget*>>		MAPMRT;
public:
	void Release(void);
};

END


#endif // RenderTargetMgr_h__
