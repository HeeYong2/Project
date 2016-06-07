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
	// m_pSDK��ü�� �����Ѵ�.	
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);

	// ��ġ�� ������ ��������.
	D3DCAPS9		CapsInfo;
	ZeroMemory(&CapsInfo, sizeof(D3DCAPS9));

	if(FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &CapsInfo)))
		return E_FAIL;

	DWORD		dwBehaviorFlag = 0;

	// ������ ������ ������ �������� �� ��ġ�� ���� ����� �����Ѵ�.
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

	// ��üȭ�鿡 ���õ� �Ӽ�
	d3dparameters.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dparameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// ��ġ�� �����Ѵ�.
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

