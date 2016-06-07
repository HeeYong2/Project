#include "MeshTextureCtrl.h"

USING(Engine)
IMPLEMENT_SINGLETON(CMeshTextureCtrl)

Engine::CMeshTextureCtrl::CMeshTextureCtrl(void)
:m_pGraphicDev(NULL)
{

}

Engine::CMeshTextureCtrl::~CMeshTextureCtrl(void)
{
	Release();
}

void Engine::CMeshTextureCtrl::InitMeshTextureCtrl(LPDIRECT3DDEVICE9 pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
}

HRESULT Engine::CMeshTextureCtrl::AddMeshTexture(TEXTUREID TexID, const TCHAR* pPath, const TCHAR* FileName, LPDIRECT3DTEXTURE9* ppCopyTexture)
{
	*ppCopyTexture = NULL;
	if(NULL == m_pGraphicDev)
	{
		msg_box("GraphicDev is NULL Pointer, Please Init MeshTextureCtrl");
		return E_FAIL;
	}
	if(NULL == pPath || NULL == FileName)
		return E_FAIL;

	if(TEX_END <= TexID)
		return E_FAIL;

	MAPTEX::iterator iter = find_if(m_mapTex[TexID].begin(), m_mapTex[TexID].end(), CTag_Finder(FileName));

	if(m_mapTex[TexID].end() != iter)
	{
		*ppCopyTexture = iter->second;
		return S_OK;
	}

	TCHAR*		szNewFileName = new TCHAR[128];
	ZeroMemory(szNewFileName, sizeof(TCHAR)*128);
	lstrcpy(szNewFileName, FileName);
	TCHAR		szFullPath[128] = L"";

	lstrcat(szFullPath, pPath);
	lstrcat(szFullPath, szNewFileName);
	LPDIRECT3DTEXTURE9 pTexture;
	if(FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &pTexture)))
	{
		delete [] szNewFileName;
		*ppCopyTexture = NULL;
		return E_FAIL;	 
	}

	*ppCopyTexture = pTexture;
	m_mapTex[TexID].insert(MAPTEX::value_type(szNewFileName, pTexture));

	return S_OK;
}

void Engine::CMeshTextureCtrl::Release(void)
{
	for(int i = 0; i < TEX_END; ++i)
	{
		MAPTEX::iterator iter = m_mapTex[i].begin();
		MAPTEX::iterator iter_end = m_mapTex[i].end();

		for(; iter != iter_end; ++iter)
		{
			if(NULL != iter->first)
				delete [] (iter->first);
			safe_delete(iter->second);
		}
	}
}