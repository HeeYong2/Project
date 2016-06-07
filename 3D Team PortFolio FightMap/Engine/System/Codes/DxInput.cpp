#include "DxInput.h"
//------------------------------------------------------------------------------------------------------------------------------

using namespace Engine;

#define KEY_CHECK_KEYBOARD(KEY)		GetDIKeyState(KEY) & 0x80

IMPLEMENT_SINGLETON(CDXInput)

CDXInput::CDXInput(void)
: m_dwKey(0)
{
	ZeroMemory(m_dwKeyPressed, sizeof(DWORD) * KEYCOUNT);
	ZeroMemory(m_dwKeyDown, sizeof(DWORD) * KEYCOUNT);
}
//------------------------------------------------------------------------------------------------------------------------------
CDXInput::~CDXInput(void)
{
	Release();
}

//------------------------------------------------------------------------------------------------------------------------------
_byte CDXInput::GetDIKeyState(BYTE byKey)
{
	return m_byKeyState[byKey];
}

//------------------------------------------------------------------------------------------------------------------------------
_byte CDXInput::GetDIMouseState(DIM byKey)
{
	return m_MouseState.rgbButtons[byKey];
}
//------------------------------------------------------------------------------------------------------------------------------
_long CDXInput::GetDIMouseMove(DIM byKey)
{
	return *( ((_long*)(&m_MouseState)) + byKey ) ;
}
//------------------------------------------------------------------------------------------------------------------------------

void CDXInput::Receive_Device_State(void)
{
	if(	m_pKeyBoard != NULL)
		m_pKeyBoard->GetDeviceState(sizeof(BYTE) * 256 , m_byKeyState);
	if( m_pMouse != NULL)
		m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE) , &m_MouseState);
}
//------------------------------------------------------------------------------------------------------------------------------
HRESULT CDXInput::Init_Input_Device(HINSTANCE hInstance , HWND hWnd)
{
	if(FAILED(DirectInput8Create(hInstance , DIRECTINPUT_VERSION , IID_IDirectInput8
		, (void**)&m_pInput ,  NULL)))
	{
		msg_box("DirectInput8Create Failed");
		return E_FAIL;
	}

	if(FAILED(Init_KeyBoard(hWnd)))
	{
		msg_box("Init_Keyboard Failed");
		return E_FAIL;
	}

	if(FAILED(Init_Mouse(hWnd)))
	{
		msg_box("Init_Keyboard Failed");
		return E_FAIL;
	}

	return S_OK;
}
//---------------------------------------------------------------------------------------------------------------------------------------

HRESULT CDXInput::Init_KeyBoard(HWND hWnd)
{
	if(FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, NULL)))
		return E_FAIL;

	if(FAILED(m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard)))
		return E_FAIL;

	if(FAILED(m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return E_FAIL;

	m_pKeyBoard->Acquire();
	return S_OK;
}
//---------------------------------------------------------------------------------------------------------------------------------------

HRESULT CDXInput::Init_Mouse(HWND hWnd)
{
	if(FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pMouse, NULL)))
		return E_FAIL;

	if(FAILED(m_pMouse->SetDataFormat(&c_dfDIMouse)))
		return E_FAIL;

	if(FAILED(m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
		return E_FAIL;

	m_pMouse->Acquire();
	return S_OK;
}
//------------------------------------------------------------------------------------------------------------------------------------

void CDXInput::KeyCheck(void)
{
	m_dwKey = 0;

	if(GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwKey |= KEY_RBUTTON;
	if(GetAsyncKeyState(VK_LSHIFT) & 0x8000)
		m_dwKey |= KEY_LSHIFT; 
	//if(KEY_CHECK_KEYBOARD(DIK_F))			m_dwKey |= KEY_F;
 	if(GetAsyncKeyState('F') & 0x8000)
 		m_dwKey |= KEY_F;
 	if(GetAsyncKeyState('W') & 0x8000)
 		m_dwKey |= KEY_W;
 	if(GetAsyncKeyState('A') & 0x8000)
 		m_dwKey |= KEY_A;
 	if(GetAsyncKeyState('S') & 0x8000)
 		m_dwKey |= KEY_S;
 	if(GetAsyncKeyState('D') & 0x8000)
 		m_dwKey |= KEY_D;
 	if(GetAsyncKeyState('R') & 0x8000)
 		m_dwKey |= KEY_R;
	if(GetAsyncKeyState('P') & 0x8000)
		m_dwKey |= KEY_P;

}
//------------------------------------------------------------------------------------------------------------------------------------

_bool CDXInput::KeyPressed(DWORD dwKey, BYTE byKeyIdx)
{
	//0001	&	0001	0010
	if(m_dwKey & dwKey)
	{
		//0011
		m_dwKeyPressed[byKeyIdx] |= dwKey;
		return false;
	}
	//0011	&	0001	0010
	else if(m_dwKeyPressed[byKeyIdx] & dwKey)
	{
		//0001	=	0001
		m_dwKeyPressed[byKeyIdx] = m_dwKey;
		return true;
	}

	return false;
}
//------------------------------------------------------------------------------------------------------------------------------------

DWORD CDXInput::GetKey(void)
{
	return m_dwKey;
}
//------------------------------------------------------------------------------------------------------------------------------------

_bool CDXInput::KeyDown(DWORD dwKey, BYTE byKeyIdx)
{
	//0001
	//0010
	if(KeyPressed(dwKey))
	{
		//0011	&	0001
		//0010
		m_dwKeyDown[byKeyIdx] ^= dwKey;
	}

	//0001	&	0001
	//0001	&	0010
	//0011

	//0010	&	0001
	if( !(m_dwKeyDown[byKeyIdx] & dwKey) )
	{
		//0011	&	0001
		//0011	&	0010

		//0010	&	0001
		if(m_dwKey & dwKey)
		{
			//0001
			//0011
			m_dwKeyDown[byKeyIdx] |= dwKey;
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------------------------------------------------------------------
_bool CDXInput::KeyDownContinued(DWORD dwKey, BYTE byKeyIdx )
{
	//0001
	//0010
	if(!KeyPressed(dwKey))
	{
		//0011	&	0001
		//0010
		m_dwKeyDown[byKeyIdx] ^= dwKey;
	}

	//0001	&	0001
	//0001	&	0010
	//0011

	//0010	&	0001
	if( !(m_dwKeyDown[byKeyIdx] & dwKey) )
	{
		//0011	&	0001
		//0011	&	0010

		//0010	&	0001
		if(m_dwKey & dwKey)
		{
			//0001
			//0011
			m_dwKeyDown[byKeyIdx] |= dwKey;
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------------------------------------------------------------------
bool CDXInput::KeyCombine(DWORD dwFirstKey, DWORD dwSecondKey, BYTE byKeyIdx /*= 0*/)
{
	if( m_dwKey & dwFirstKey )
	{
		if(KeyDown(dwSecondKey, byKeyIdx))
			return true;
	}

	return false;
}

//------------------------------------------------------------------------------------------------------------------------------------
void CDXInput::Release(void)
{
	Engine::safe_release(m_pKeyBoard);
	Engine::safe_release(m_pMouse);
	Engine::safe_release(m_pInput);
}