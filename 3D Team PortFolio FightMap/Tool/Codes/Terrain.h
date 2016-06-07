#ifndef Terrain_h__
#define Terrain_h__

/********************************************************************
	created:	2015/04/14
	file path:	Yong\Tool\Code
	file base:	Terrain
	file ext:	h
	author:		Yong	
	purpose:	Terrain
*********************************************************************/
#include "GameObject.h"
#include "Tool_Defines.h"

class CTerrain
	: public Engine::CGameObject
{
public:
	explicit CTerrain(void);
	~CTerrain(void);

public:
	virtual HRESULT Initialize(const _uint& wCntX, const _uint& wCntZ, const float& fItv);
	virtual void Update(void);
	virtual void Render(void);
	virtual _ulong Release(void);

public:
	static CTerrain* Create(const _uint& wCntX, const _uint& wCntZ, const float& fItv);
	void SetLoadVerTex(_vec3 vPos);

private:
	HRESULT Add_Component(void);
	void Render_Texture(void);
	void Render_Buffer(void);

public:
	Engine::VTXTEX*	GetVtxTex(void);			//Vtx정보 가져오기
	Engine::CComponent*		GetBuffer(void);	//Buffer정보 가져오기

public:
	void SetFrameType(_int iFrameNum);			//WireFrame 설정
	void SetVerTex(_vec3 vPos);

public:
	WORD GetVtxSizeX(void);
	WORD GetVtxSizeZ(void);
	_float GetVtxItv(void);

public:
	void SetMix1num(_int iMix1Num);
	void SetMix2num(_int iMix2Num);
	void SetMix3num(_int iMix3Num);
	void SetMix4num(_int iMix4Num);

public:
	void SetUVRatio(_float UVRatio);
	void SetMix1Ratio(_float iMix1Ratio);
	void SetMix2Ratio(_float iMix2Ratio);
	void SetMix3Ratio(_float iMix3Ratio);
	void SetMix4Ratio(_float iMix4Ratio);

public:
	void						SetTextureNum(_int iTextureNum);
	void						Set_ConstantTable(void);	
	void						CreateAlphaMap(void);
	LPDIRECT3DTEXTURE9			GetTexture(_int iNum);
	void						SetAlphaTexture(LPDIRECT3DTEXTURE9* ppLoadTexture);
	TEXINFO						GetTexInfo(void);
	void						SetTexInfo(TEXINFO tTexInfo);
	
private:
	Engine::CTransform*			m_pInfo;
	Engine::CComponent*			m_pTexture;
	Engine::CComponent*			m_pBuffer;
	Engine::VTXTEX*				m_pVtxInfo;
	LPDIRECT3DSURFACE9			m_pSurface[4];
	Engine::INDEX32*			m_pIndex;
	Engine::CComponent*			m_pAlphaTex;
	_ulong						m_iTriCnt;

private:
	LPD3DXEFFECT				m_pEffect;
	LPDIRECT3DTEXTURE9			m_pAlphaTexture[4];

private:
	_int						m_iFrameNum;
	_int                        m_wResult;
	_int						m_iTextureNum;
	
private:
	_float						m_fUVRatio;
	_float						m_fVtxItv;

private:
	WORD 						m_VtxSizeX;
	WORD 						m_VtxSizeZ;
	_bool						m_isCreateSplatting;
	TEXINFO						m_tTexInfo;

};
#endif // Terrain_h__
