#ifndef Graphic_Device_h__
#define Graphic_Device_h__
/********************************************************************
	created:	2015/01/31
	file path:	Yong\Engine\System\Code
	file base:	Graphic_Device
	file ext:	h
	author:		Yong	
	purpose:	렌더링장치를 초기화합니다.
*********************************************************************/
#include "Engine_Defines.h"

namespace Engine
{
	class _declspec(dllexport) CGraphic_Device
	{
		DECLARE_SINGLETON(CGraphic_Device)
	public:
		enum WINMODE {MODE_FULL, MODE_WIN};
	private:
		explicit CGraphic_Device(void);
	public:
		~CGraphic_Device(void);
	public: // Getter
		LPDIRECT3DDEVICE9 GetGraphicDev(void) {
			return m_pGraphicDev;}
	public:
		HRESULT Init_GraphicDev(WINMODE WndMode, HWND hWnd, const _ulong& dwWndCX, const _ulong& dwWndCY);
		void SetRenderState(_D3DRENDERSTATETYPE Type , _ulong dwFlag);
		void SetSamplerState(_D3DSAMPLERSTATETYPE Type, _ulong dwFlag);

	private:
		LPDIRECT3D9					m_pSDK;
		LPDIRECT3DDEVICE9			m_pGraphicDev;
	private:
		void Release(void);
	};
}

#endif // Graphic_Device_h__