#ifndef DxInput_h__
#define DxInput_h__
/********************************************************************
	created:	2015/02/03
	file path:	Yong\Engine\System\Code
	file base:	DxInput
	file ext:	h
	author:		Yong
	purpose:	다이렉트 입력장치를 초기화 한다.	
*********************************************************************/
#include "Engine_Defines.h"

const BYTE KEYCOUNT = 3;

namespace Engine
{
	class _declspec(dllexport) CDXInput
	{
		DECLARE_SINGLETON(CDXInput)

	public:
		enum DIM {DIM_LBUTTON , DIM_RBUTTON , DIM_MBUTTON , DIM_XBUTTON , DIM_XMOVE = 0 , DIM_YMOVE , DIM_ZMOVE};

	private:
		explicit CDXInput(void);

	public:
		~CDXInput(void);

	public://Getter
		_byte GetDIKeyState(BYTE byKey);
		_byte GetDIMouseState(DIM byKey);
		_long GetDIMouseMove(DIM byKey);

	public:
		HRESULT Init_Input_Device(HINSTANCE hInstance , HWND hWnd);
		HRESULT Init_KeyBoard(HWND hWnd);
		HRESULT Init_Mouse(HWND hWnd);

	public:	//Setter
		void Receive_Device_State(void);

	private:
		LPDIRECTINPUT8			m_pInput;
		LPDIRECTINPUTDEVICE8    m_pKeyBoard;
		BYTE					m_byKeyState[256];

	private:
		LPDIRECTINPUTDEVICE8	m_pMouse;
		DIMOUSESTATE			m_MouseState;

	private:
		void Release(void);

	///////중복 키 체크
	private:
		DWORD		m_dwKey;
		DWORD		m_dwKeyPressed[KEYCOUNT];
		DWORD		m_dwKeyDown[KEYCOUNT];

	public:
		DWORD GetKey(void);

	public:
		void KeyCheck(void);

	public:
		_bool KeyPressed(DWORD dwKey, BYTE byKeyIdx = 0);
		_bool KeyDown(DWORD dwKey, BYTE byKeyIdx = 0);
		_bool KeyDownContinued(DWORD dwKey, BYTE byKeyIdx = 0);
		_bool KeyCombine(DWORD dwFirstKey, DWORD dwSecondKey, BYTE byKeyIdx = 0);

	};
}
#endif // DxInput_h__
