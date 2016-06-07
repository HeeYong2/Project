#ifndef Texture_h__
#define Texture_h__
/********************************************************************
	created:	2015/01/28
	file path:	Yong\Engine\Resource\Code
	file base:	Texture
	file ext:	h
	author:		Yong	
	purpose:	텍스쳐 추가
*********************************************************************/
#include "Resource.h"

namespace Engine
{
	class CTexture
		: public CResource
	{
	public:
		enum TEXTURETYPE {TYPE_NORMAL , TYPE_CUBE};

	public:
		explicit CTexture(void);
		virtual ~CTexture(void);
	public: // Getter
		IDirect3DBaseTexture9*	Get_Texture(const _ulong& dwCnt = 0) {
			return m_TextureList[dwCnt];}
	public:
		virtual CResource* Clone_Resource(void);
		virtual void Render(LPDIRECT3DDEVICE9 pGraphicDev , const _ulong & dwCnt = 0);

	public:
		HRESULT Insert_Texture(LPDIRECT3DDEVICE9 pGraphicDev , TEXTURETYPE TextureType
			, const TCHAR* pPath , const TCHAR* pFileName , const _ulong& dwCnt);
		HRESULT Insert_TexturePath(LPDIRECT3DDEVICE9 pGraphicDev , TEXTURETYPE TextureType, const TCHAR* pPath);

	public:
		static CTexture * Create(LPDIRECT3DDEVICE9 pGraphicDev , TEXTURETYPE TextureType
			, const TCHAR* pPath , const TCHAR* pFileName , const _ulong& dwCnt);

		static CTexture * CreatePath(LPDIRECT3DDEVICE9 pGraphicDev , TEXTURETYPE TextureType, const TCHAR* pFullPath);

	private:
		vector<IDirect3DBaseTexture9*>					m_TextureList;
		typedef vector<IDirect3DBaseTexture9*>			TEXTURELIST;

	private:
		void Release(void);
	};
}
#endif // Texture_h__
