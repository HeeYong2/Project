#include "Fonts.h"

using namespace Engine;

IMPLEMENT_SINGLETON(CFonts)

//------------------------------------------------------------------------------------------------------------------------------------------------
CFonts::CFonts(void)
{

}
//------------------------------------------------------------------------------------------------------------------------------------------------
CFonts::~CFonts(void)
{
	Release();
}

HRESULT CFonts::Ready_Fonts(LPDIRECT3DDEVICE9 pGraphicDev , const TCHAR* pFontTag , const TCHAR* pFaceName , const _ulong& dwSizeX , const _ulong& dwSizeY , D3DXCOLOR Color)
{
	//MAPFONT::iterator iter = find_if(m_mapFonts.begin() , m_mapFonts.end() , 
	MAPFONT::iterator iter = find_if(m_mapFonts.begin() , m_mapFonts.end() , CFonts_Finder(pFontTag));

	if(iter != m_mapFonts.end())
		return E_FAIL;

	CFont*		pFont = CFont::Create(pGraphicDev , pFaceName , dwSizeX , dwSizeY , Color);

	if(pFont == NULL)
		return E_FAIL;

	m_mapFonts.insert(MAPFONT::value_type(pFontTag , pFont));

	return S_OK;
}

void CFonts::Render_Font(LPDIRECT3DDEVICE9 pGrahpicDev , const TCHAR* pFontTag , const TCHAR* pString , _matrix* pMatworld)
{
	MAPFONT::iterator iter = find_if(m_mapFonts.begin() , m_mapFonts.end() , CFonts_Finder(pFontTag));

	if(iter == m_mapFonts.end())
		return ;

	iter->second->Render(pGrahpicDev , pString , pMatworld);
}

void CFonts::Release(void)
{
	for_each(m_mapFonts.begin(), m_mapFonts.end(), CDelete_Pair());
	m_mapFonts.clear();
}