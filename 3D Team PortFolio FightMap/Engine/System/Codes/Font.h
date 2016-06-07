#ifndef Font_h__
#define Font_h__
/********************************************************************
	created:	2015/02/02
	file path:	Yong\Engine\System\Code
	file base:	Font
	file ext:	h
	author:		Yong	
	purpose:	±Û¾¾
*********************************************************************/

#include "Engine_Defines.h"
namespace Engine
{
	class _declspec(dllexport) CFont
	{
	public:
		explicit CFont(void);
		~CFont(void);

	public:
		HRESULT Init_Font(LPDIRECT3DDEVICE9 pGraphicDev , const TCHAR* pFaceName , const _ulong& dwSizeX , const _ulong& dwSizeY , D3DXCOLOR Color);
		void Render(LPDIRECT3DDEVICE9 pGraphicDev , const TCHAR* pString , _matrix* pMatWorld);
	
	public:
		static CFont* Create(LPDIRECT3DDEVICE9 pGraphicDev , const TCHAR* pFaceName , const _ulong& dwSizeX , const _ulong& dwSizeY , D3DXCOLOR Color);
	protected:
		D3DXFONT_DESCW			m_FontInfo;
		LPD3DXFONT				m_pFont;
		LPD3DXSPRITE			m_pSprite;

	private:
		void Release(void);

	};
}

#endif // Font_h__
