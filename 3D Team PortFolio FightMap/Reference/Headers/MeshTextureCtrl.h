#ifndef MeshTextureCtrl_h__
#define MeshTextureCtrl_h__
/********************************************************************
	created:	2015/06/02
	file path:	Yong\Tool\Code
	file base:	MeshTextureCtrl
	file ext:	h
	author:		Yong	
	purpose:	중복되는 이미지 로드를 막는다
*********************************************************************/

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CMeshTextureCtrl
{
public:
	DECLARE_SINGLETON(CMeshTextureCtrl)

public:
	enum TEXTUREID{TEX_DIF, TEX_NOR, TEX_SPEC, TEX_ALPHA, TEX_END};

private:
	explicit CMeshTextureCtrl(void);
public:
	~CMeshTextureCtrl(void);

public:
	void InitMeshTextureCtrl(LPDIRECT3DDEVICE9 pGraphicDev);
	HRESULT AddMeshTexture(TEXTUREID TexID, const TCHAR* pFullPath, const TCHAR* FileName, LPDIRECT3DTEXTURE9* ppCopyTexture);

private:
	LPDIRECT3DDEVICE9								m_pGraphicDev;
	map<const TCHAR*, LPDIRECT3DTEXTURE9>			m_mapTex[TEX_END];
	typedef map<const TCHAR*, LPDIRECT3DTEXTURE9>	MAPTEX;

private:
	void Release(void);
};

END
#endif // MeshTextureCtrl_h__
