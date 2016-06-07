
//=For.System.Getter===================================================================
inline LPDIRECT3DDEVICE9 Engine::GetGraphicDev(void)
{
	return Engine::CGraphic_Device::GetInstance()->GetGraphicDev();
}
//------------------------------------------------------------------------------------------------------------
inline _byte Engine::GetDIKeyState(BYTE byKey)
{
	return Engine::CDXInput::GetInstance()->GetDIKeyState(byKey);
}
//------------------------------------------------------------------------------------------------------------
inline _byte Engine::GetDIMouseState(Engine::CDXInput::DIM byKey)
{
	return Engine::CDXInput::GetInstance()->GetDIMouseState(byKey);
}
//------------------------------------------------------------------------------------------------------------
inline _long Engine::GetDIMouseMove(Engine::CDXInput::DIM byKey)
{
	return Engine::CDXInput::GetInstance()->GetDIMouseMove(byKey);
}
//------------------------------------------------------------------------------------------------------------
inline _bool Engine::KeyPressed(DWORD dwKey , _byte byKeyIdex /* = 0 */)
{
	return Engine::CDXInput::GetInstance()->KeyPressed(dwKey , byKeyIdex);
}
//------------------------------------------------------------------------------------------------------------
inline _bool Engine::KeyDown(DWORD dwKey , _byte byKeyIdex /* = 0 */)
{
	return Engine::CDXInput::GetInstance()->KeyDown(dwKey , byKeyIdex);
}
//------------------------------------------------------------------------------------------------------------
inline _bool Engine::KeyCombine(DWORD dwFirstKey , DWORD dwSecondKey , _byte byKeyIdex /* = 0 */)
{
	return Engine::CDXInput::GetInstance()->KeyCombine(dwFirstKey , dwSecondKey , byKeyIdex);
}
//-----------------------------------------------o-------------------------------------------------------------

inline _bool Engine::KeyDownContinued(DWORD dwKey , _byte byKeyIdex /* = 0 */)
{
	return Engine::CDXInput::GetInstance()->KeyDownContinued(dwKey , byKeyIdex );
}

//-----------------------------------------------o-------------------------------------------------------------
inline void Engine::KeyCheck(void)
{
	Engine::CDXInput::GetInstance()->KeyCheck();
}

//------------------------------------------------------------------------------------------------------------

inline _float Engine::GetTime(void)
{
	return Engine::CTimeMgr::GetInstance()->GetTime();
}

//=For.System.Setter===================================================================
inline void Engine::SetRenderState(_D3DRENDERSTATETYPE Type, _ulong dwFlag)
{
	Engine::CGraphic_Device::GetInstance()->SetRenderState(Type, dwFlag);
}

//------------------------------------------------------------------------------------------------------------
inline void Engine::SetSamplerState(_D3DSAMPLERSTATETYPE Type, _ulong dwFlag)
{
	Engine::CGraphic_Device::GetInstance()->SetSamplerState(Type , dwFlag);
}

//------------------------------------------------------------------------------------------------------------

inline void Engine::Receive_Device_State(void)
{
	Engine::CDXInput::GetInstance()->Receive_Device_State();
}

//------------------------------------------------------------------------------------------------------------
inline void Engine::SetTime(void)
{
	Engine::CTimeMgr::GetInstance()->SetTime();
}

//=For.System.General==================================================================
inline HRESULT Engine::Init_GraphicDevice(Engine::CGraphic_Device::WINMODE WndMode, HWND hWnd, const _ulong& dwWndCX, const _ulong& dwWndCY)
{
	return Engine::CGraphic_Device::GetInstance()->Init_GraphicDev(WndMode, hWnd, dwWndCX, dwWndCY);
}

//------------------------------------------------------------------------------------------------------------

inline HRESULT Engine::Init_InputDevice(HINSTANCE hInstance , HWND hWnd)
{
	return Engine::CDXInput::GetInstance()->Init_Input_Device(hInstance , hWnd);
}

//------------------------------------------------------------------------------------------------------------

inline void Engine::InitTime(void)
{
	Engine::CTimeMgr::GetInstance()->InitTime();
}

//------------------------------------------------------------------------------------------------------------

inline HRESULT Engine::Ready_Fonts(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pFontTag, const TCHAR* pFaceName, const _ulong& dwSizeX, const _ulong& dwSizeY, D3DXCOLOR Color)
{
	return Engine::CFonts::GetInstance()->Ready_Fonts(pGraphicDev, pFontTag, pFaceName, dwSizeX, dwSizeY, Color);
}

//------------------------------------------------------------------------------------------------------------

inline void Engine::Render_Font(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pFontTag, const TCHAR* pString, _matrix* pmatWorld)
{
	Engine::CFonts::GetInstance()->Render_Font(pGraphicDev, pFontTag, pString, pmatWorld);
}

//------------------------------------------------------------------------------------------------------------

inline void Engine::System_Release(void)
{
	Engine::CFonts::GetInstance()->DestroyInstance();
	Engine::CGraphic_Device::GetInstance()->DestroyInstance();
	Engine::CDXInput::GetInstance()->DestroyInstance();
	Engine::CTimeMgr::GetInstance()->DestroyInstance();
}