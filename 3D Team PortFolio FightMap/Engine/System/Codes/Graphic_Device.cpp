#include "Graphic_Device.h"

USING(Engine)

IMPLEMENT_SINGLETON(CGraphic_Device)

//--------------------------------------------------------------------------------------------------------------------------------------------------------
CGraphic_Device::CGraphic_Device(void)
: m_pSDK(NULL)
, m_pGraphicDev(NULL)
{

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
CGraphic_Device::~CGraphic_Device(void)
{
	Release();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CGraphic_Device::SetRenderState(_D3DRENDERSTATETYPE Type , _ulong dwFlag)
{
	m_pGraphicDev->SetRenderState(Type , dwFlag);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void Engine::CGraphic_Device::SetSamplerState(_D3DSAMPLERSTATETYPE Type, _ulong dwFlag)
{
	m_pGraphicDev->SetSamplerState(0 ,  Type , dwFlag);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CGraphic_Device::Init_GraphicDev(WINMODE WndMode, HWND hWnd, const _ulong& dwWndCX, const _ulong& dwWndCY)
{
	// m_pSDK객체를 생성한다.	
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	// 장치의 성능을 조사하자.
	D3DCAPS9		CapsInfo;
	ZeroMemory(&CapsInfo, sizeof(D3DCAPS9));

	if(FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &CapsInfo)))
		return E_FAIL;

	DWORD		dwBehaviorFlag = 0;

	// 위에서 조사한 정보를 바탕으로 내 장치의 동작 방식을 결정한다.
	if(CapsInfo.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)	
		dwBehaviorFlag |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		dwBehaviorFlag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;	

	dwBehaviorFlag |= D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS		d3dparameters;
	ZeroMemory(&d3dparameters, sizeof(d3dparameters));

	d3dparameters.BackBufferWidth = dwWndCX;
	d3dparameters.BackBufferHeight = dwWndCY;
	d3dparameters.BackBufferCount = 1;
	d3dparameters.BackBufferFormat = D3DFMT_A8R8G8B8;

	d3dparameters.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dparameters.MultiSampleQuality = 0;

	d3dparameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dparameters.hDeviceWindow = hWnd;
	d3dparameters.Windowed = WndMode;

	d3dparameters.EnableAutoDepthStencil = TRUE;
	d3dparameters.AutoDepthStencilFormat = D3DFMT_D24S8;

	// 전체화면에 관련된 속성
	d3dparameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dparameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// 장치를 생성한다.
	if(FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, dwBehaviorFlag
		, &d3dparameters, &m_pGraphicDev)))
		return E_FAIL;

	return S_OK;	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void CGraphic_Device::Release(void)
{
	Engine::safe_release(m_pGraphicDev);
	Engine::safe_release(m_pSDK);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

