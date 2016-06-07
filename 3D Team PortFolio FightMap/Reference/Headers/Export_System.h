#ifndef Export_System_h__
#define Export_System_h__
/********************************************************************
	created:	2015/01/31
	file path:	Yong\Client\Code
	file base:	Export_Function
	file ext:	h
	author:		Yong	
	purpose:	렌더링장치, 입력장치, 사운드장치 등등등
*********************************************************************/
#include "../../Reference/Headers/Graphic_Device.h"
#include "../../Reference/Headers/Fonts.h"
#include "../../Reference/Headers/DxInput.h"
#include "../../Reference/Headers/TimeMgr.h"

namespace Engine
{
//=For.System.Getter===================================================================
	inline LPDIRECT3DDEVICE9 GetGraphicDev(void);
	inline _byte GetDIKeyState(BYTE byKey);
	inline _byte GetDIMouseState(Engine::CDXInput::DIM byKey);
	inline _long GetDIMouseMove(Engine::CDXInput::DIM byKey);
	inline _bool KeyPressed(DWORD dwKey , _byte byKeyIdex = 0);
	inline _bool KeyDown(DWORD dwKey , _byte byKeyIdex = 0);
	inline _bool KeyCombine(DWORD dwFirstKey , DWORD dwSecondKey , _byte byKeyIdex = 0);
	inline _bool KeyDownContinued(DWORD dwKey , _byte byKeyIdex = 0);
	inline void KeyCheck(void);
	inline _float GetTime(void);

//=For.System.Setter===================================================================
	inline void SetRenderState(_D3DRENDERSTATETYPE Type , _ulong dwFlag); 
	inline void SetSamplerState(_D3DSAMPLERSTATETYPE Type, _ulong dwFlag);
	inline void Receive_Device_State(void);
	inline void SetTime(void);

//=For.System.General==================================================================
	inline HRESULT Init_GraphicDevice(Engine::CGraphic_Device::WINMODE WndMode, HWND hWnd, const _ulong& dwWndCX, const _ulong& dwWndCY);
	inline HRESULT Init_InputDevice(HINSTANCE hInstance , HWND hWnd);
	inline void InitTime(void);
	inline HRESULT Ready_Fonts(LPDIRECT3DDEVICE9 pGraphicDev , const TCHAR* pFontTag , const TCHAR* pFaceName , const _ulong& dwSizeX , const _ulong& dwSizeY , D3DXCOLOR Color);
	inline void Render_Font(LPDIRECT3DDEVICE9 pGraphicDev , const TCHAR* pFontTag , const TCHAR* pString , _matrix* pMatWorld);



	inline void System_Release(void);
}

#include "Export_System.inl"

#endif 
