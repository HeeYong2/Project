#include "Shader.h"

USING(Engine)

//--------------------------------------------------------------------------------------------------------------------------------------------------------
CShader::CShader(LPDIRECT3DDEVICE9 pGraphicDev)
: m_pGraphicDev(pGraphicDev)
{

}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
CShader::~CShader(void)
{

}


HRESULT CShader::Init_Shader(const TCHAR* pFilePath)
{
	if(FAILED(D3DXCreateEffectFromFile(m_pGraphicDev, pFilePath, NULL, NULL, D3DXSHADER_DEBUG, NULL, &m_pEffect, &m_pErrBuffer)))
	{
		MessageBoxA(NULL, ((char*)m_pErrBuffer->GetBufferPointer()), "System_Error", MB_OK);
		return E_FAIL;
	}
	return S_OK;
}



CShader* CShader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pFilePath)
{
	CShader*			pShader = new CShader(pGraphicDev);

	if(FAILED(pShader->Init_Shader(pFilePath)))
		::safe_delete(pShader);

	return pShader;
}

_ulong CShader::Release(void)
{
	_ulong		dwRefCnt = CBase::Release();

	if(0 == dwRefCnt)
	{
		if(Engine::safe_release(m_pEffect))
			msg_box("m_pEffect safe_release Failed");
		delete this;
	}
	return dwRefCnt;

}