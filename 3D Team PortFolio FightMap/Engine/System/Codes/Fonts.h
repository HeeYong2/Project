#ifndef Fonts_h__
#define Fonts_h__
/********************************************************************
	created:	2015/02/03
	file path:	Yong\Engine\System\Code
	file base:	Fonts
	file ext:	h
	author:		Yong	
	purpose:	Fonts
*********************************************************************/

#include "Engine_Defines.h"
#include "Font.h"

namespace Engine
{
	class _declspec(dllexport) CFonts
	{
		DECLARE_SINGLETON(CFonts)

	public:
		explicit CFonts(void);
		~CFonts(void);

	public:
		HRESULT Ready_Fonts(LPDIRECT3DDEVICE9 pGraphicDev , const TCHAR* pFontTag , const TCHAR* pFaceName , const _ulong& dwSizeX , const _ulong& dwSizeY , D3DXCOLOR Color);
		HRESULT Init_Font(LPDIRECT3DDEVICE9 pGraphicDev);
		void Render_Font(LPDIRECT3DDEVICE9 pGrahpicDev , const TCHAR* pFontTag , const TCHAR* pString , _matrix* pMatworld);

	private:
		map<const TCHAR* , CFont*>				m_mapFonts;
		typedef map<const TCHAR* , CFont*>		MAPFONT;

	private:
		void Release(void);
	};

	class CFonts_Finder
	{
	public:
		explicit CFonts_Finder(const TCHAR* pTag) : m_pTag(pTag) {}
		~CFonts_Finder(void)	{}

	public:
		template <typename T>
		bool operator () (T& Pair)
		{
			_int iResult = lstrcmp(Pair.first , m_pTag);

			if(iResult == 0)
				return true;
			return false;
		}
	private:
		const TCHAR*			m_pTag;
	};
}
#endif // Fonts_h__
