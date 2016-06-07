#include "Texture.h"
#include <fstream>
using namespace Engine;
//---------------------------------------------------------------------------------------------------------------------------------

CTexture::CTexture(void)
{

}

//---------------------------------------------------------------------------------------------------------------------------------
CTexture::~CTexture(void)
{
	Release();
}

CResource* Engine::CTexture::Clone_Resource(void)
{
	CResource * pResource = new CTexture(*this);

	++(*m_pRefCnt);

	return pResource;
}

//---------------------------------------------------------------------------------------------------------------------------------
void Engine::CTexture::Render(LPDIRECT3DDEVICE9 pGraphicDev , const _ulong & dwCnt /* = 0 */)
{
	if(dwCnt >= m_TextureList.size())
		return;
	pGraphicDev->SetTexture(0 , m_TextureList[dwCnt]);
}

CTexture* CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev , TEXTURETYPE TextureType , const TCHAR* pPath , const TCHAR* pFileName , const _ulong& dwCnt)
{
	CTexture* pTexture = new CTexture;
	if(FAILED(pTexture->Insert_Texture(pGraphicDev , TextureType , pPath , pFileName , dwCnt)))
	{
		Engine::safe_delete(pTexture);
		return NULL;
	}
	return pTexture;
}

CTexture * Engine::CTexture::CreatePath(LPDIRECT3DDEVICE9 pGraphicDev , TEXTURETYPE TextureType, const TCHAR* pFullPath)
{
	CTexture* pTexture = new CTexture;
	if(FAILED(pTexture->Insert_TexturePath(pGraphicDev , TextureType , pFullPath)))
	{
		Engine::safe_delete(pTexture);
		return NULL;
	}
	return pTexture;	
}

HRESULT Engine::CTexture::Insert_TexturePath(LPDIRECT3DDEVICE9 pGraphicDev , TEXTURETYPE TextureType, const TCHAR* pPath)
{
	wifstream		LoadFile;

	LoadFile.open(pPath);

	if(TextureType == TYPE_NORMAL )
	{
		while(!LoadFile.eof())
		{
			TCHAR		szPath[256];
			LoadFile.getline(szPath, 256);

			LPDIRECT3DTEXTURE9		pTexture = NULL;
			if(FAILED(D3DXCreateTextureFromFile(pGraphicDev , szPath , &pTexture)))
			{
				msg_box("D3DXCreateTextureFromFile Failed");
				return E_FAIL;
			}
			m_TextureList.push_back(pTexture);
		}
	}
	else
	{
		while(!LoadFile.eof())
		{
			TCHAR		szPath[256];
			LoadFile.getline(szPath, 256);

			LPDIRECT3DCUBETEXTURE9			pTexture = NULL;

			if(FAILED(D3DXCreateCubeTextureFromFile(pGraphicDev, szPath, &pTexture)))
			{
				msg_box("D3DXCreateCubeTextureFromFile Failed");
				return E_FAIL;
			}
			m_TextureList.push_back(pTexture);
		}
	}



	return S_OK;
}

//---------------------------------------------------------------------------------------------------------------------------------------

HRESULT CTexture::Insert_Texture(LPDIRECT3DDEVICE9 pGraphicDev , TEXTURETYPE TextureType , const TCHAR* pPath , const TCHAR* pFileName , const _ulong& dwCnt)
{
	TCHAR		szFullPath[128] = L"";
	TCHAR		szFileName[128] = L"";

	for(_int i = 0 ; i < _int(dwCnt) ; ++i)
	{
		lstrcpy(szFullPath , pPath);
		wsprintf(szFileName , pFileName , i);
		lstrcat(szFullPath , szFileName );

		if(TextureType == TYPE_NORMAL )
		{
			LPDIRECT3DTEXTURE9		pTexture = NULL;

			if(FAILED(D3DXCreateTextureFromFile(pGraphicDev , szFullPath , &pTexture)))
			{
				msg_box("D3DXCreateTextureFromFile Failed");
				return E_FAIL;
			}
			m_TextureList.push_back(pTexture);
		}
		else
		{
			LPDIRECT3DCUBETEXTURE9			pTexture = NULL;

			if(FAILED(D3DXCreateCubeTextureFromFile(pGraphicDev, szFullPath, &pTexture)))
			{
				msg_box("D3DXCreateCubeTextureFromFile Failed");
				return E_FAIL;
			}
			m_TextureList.push_back(pTexture);
		}

	}

	return S_OK;
}
//---------------------------------------------------------------------------------------------------------------------------------------

void CTexture::Release(void)
{
	if((*m_pRefCnt) == 0)
	{
		for_each(m_TextureList.begin(), m_TextureList.end(), CRelease_Single());
		m_TextureList.clear();

		CResource::Release();
	}
	else
		--(*m_pRefCnt);
}