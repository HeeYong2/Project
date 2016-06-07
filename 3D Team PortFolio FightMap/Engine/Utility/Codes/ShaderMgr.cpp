#include "ShaderMgr.h"
#include "Shader.h"

USING(Engine)

IMPLEMENT_SINGLETON(CShaderMgr)

//--------------------------------------------------------------------------------------------------------------------------------------------------------
CShaderMgr::CShaderMgr(void)
{

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
CShaderMgr::~CShaderMgr(void)
{
	Release();
}

LPD3DXEFFECT CShaderMgr::Get_Effect(const TCHAR* pShaderTag)
{
	CShader*	pShader = Find_Shader(pShaderTag);

	if(NULL == pShader)
	{
		msg_box("Get_Effect Failed");
		return NULL;
	}
	return pShader->Get_Effect();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
CShader* CShaderMgr::Find_Shader(const TCHAR* pShaderTag)
{
	MAPSHADER::iterator	iter = find_if(m_mapShader.begin(), m_mapShader.end(), CTag_Finder(pShaderTag));

	if(iter == m_mapShader.end())
		return NULL;

	return iter->second;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CShaderMgr::Ready_Shader(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pShaderTag, const TCHAR* pFilePath)
{
	CShader*	pShader = Find_Shader(pShaderTag);

	if(NULL != pShader)
		return E_FAIL;

	pShader = CShader::Create(pGraphicDev, pFilePath);

	if(NULL == pShader)	
		return E_FAIL;

	m_mapShader.insert(MAPSHADER::value_type(pShaderTag, pShader));

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void CShaderMgr::Release(void)
{
	for_each(m_mapShader.begin(), m_mapShader.end(), Engine::CRelease_Pair());
	m_mapShader.clear();
}