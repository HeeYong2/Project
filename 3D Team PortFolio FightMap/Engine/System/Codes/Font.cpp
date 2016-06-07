#include "Font.h"

using namespace Engine;

Engine::CFont::CFont(void)
{

}
//---------------------------------------------------------------------------------------------------------------------------------------

Engine::CFont::~CFont(void)
{
	Release();
}
//---------------------------------------------------------------------------------------------------------------------------------------

HRESULT CFont::Init_Font(LPDIRECT3DDEVICE9 pGraphicDev , const TCHAR* pFaceName , const _ulong& dwSizeX , const _ulong& dwSizeY , D3DXCOLOR Color)
{
	ZeroMemory(&m_FontInfo , sizeof(D3DXFONT_DESCW));

	m_FontInfo.Height = dwSizeY;
	m_FontInfo.Width = dwSizeX;
	m_FontInfo.Weight = FW_HEAVY;
	m_FontInfo.CharSet = HANGEUL_CHARSET;
	lstrcpy(m_FontInfo.FaceName , pFaceName);

	if(FAILED(D3DXCreateSprite(pGraphicDev , &m_pSprite)))
		return E_FAIL;

	if(FAILED(D3DXCreateFontIndirect(pGraphicDev , &m_FontInfo , &m_pFont)))
		return E_FAIL;

	return S_OK;
}
//---------------------------------------------------------------------------------------------------------------------------------------

void CFont::Render(LPDIRECT3DDEVICE9 pGraphicDev , const TCHAR* pString , _matrix* pMatWorld)
{
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	
	m_pFont->DrawTextW(m_pSprite , pString , lstrlen(pString) , NULL , DT_NOCLIP , D3DXCOLOR(1.f , 1.f , 1.f , 1.f));

	m_pSprite->End();

}
//---------------------------------------------------------------------------------------------------------------------------------------

CFont* CFont::Create(LPDIRECT3DDEVICE9 pGraphicDev , const TCHAR* pFaceName , const _ulong& dwSizeX , const _ulong& dwSizeY, D3DXCOLOR Color)
{
	CFont*		pFont = new CFont;

	if(FAILED(pFont->Init_Font(pGraphicDev , pFaceName , dwSizeX , dwSizeY , Color)))
	{
		Engine::safe_delete(pFont);
		return NULL;
	}
	return pFont;
}
//---------------------------------------------------------------------------------------------------------------------------------------

void CFont::Release(void)
{	
	Engine::safe_release(m_pSprite);
	Engine::safe_release(m_pFont);
}