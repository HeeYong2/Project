#pragma once

#include "Tool_Defines.h"
#include "Layer.h"
#include "MainFrm.h"
#include "afxcmn.h"
#include "afxwin.h"
// CMapPage ��ȭ �����Դϴ�.


class CMapPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CMapPage)
public:
	enum LAYERID {LAYER_ENVIRONMENT , LAYER_GAMELOGIC , LAYER_UI};

public:
	CMapPage();
	virtual ~CMapPage();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MAPPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	DECLARE_MESSAGE_MAP()

public:
	HRESULT Ready_EnvironmentLayer(const _uint& wCntX, const _uint& wCntZ, const float& fItv);		//Terrain����
	HRESULT Ready_Player(void);		
	HRESULT Ready_Enemy(void);																		//Test�� Player ����
	//Test�� Player ����
	HRESULT Ready_StaticCamera(void);																//Test�� StaticCam ����
	HRESULT Ready_Terrain(const _uint& wCntX, const _uint& wCntZ, const float& fItv);				//Terrain1����
	HRESULT Ready_UI(void);																			//Dynamic Cam ����
	HRESULT Ready_SkyBox(CString strSkyName);														//SkyBox ����
	virtual BOOL OnInitDialog();																	//Map ��Ʈ������ Initialize

	//Getter
public:
	_int 	GetHeight(void);																		//���� ������
	_int 	GetRange(void);																			//���� ������
	_int 	GetVtx1Size(void);																		//Terrain1�� VtxSize
	_int 	GetVtx2Size(void);																		//Terrain2�� VtxSize
	_int 	GetNum(void);																			//� �ͷ����� �����ߴ���?
	_int	GetMixNum(void);																		//� ����ä������?
	_int	GetMode(void);																			//� ������ư�� ���ȴ��� ?
	_vec3 	GetPos(void);																			//�󸶳� �̵����״���?
	_bool 	GetMove(void);																			//�̵� ��ư�� �������� Ȯ��
	_bool	GetTest(void);																			//Test������ ( Player�� Static Cam�� ������� ������)
	_bool	GetSplatting(void);																		//Splatting�� ���ȴ���
	_bool	GetSplattingMode(void);																	//Splatting Mode�� �׸������� ���������

	//Setter
public:
	void SetMove(_bool isMove);
	void SetRadio(_int iSelectRadio);

public:
	afx_msg void OnNMCustomdrawHeight(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawRange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedWireFrame();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedLoad();
	afx_msg void OnBnClickedApplyTerrain();

public:
	afx_msg void OnBnClickedRadio();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedMoveApply();

public:
	afx_msg void OnBnClickedApplySkyBox();
	afx_msg void OnBnClickedLoadSkyBox();

public:
	afx_msg void OnBnClickedUpdown();
	afx_msg void OnBnClickedPlateau();
	afx_msg void OnBnClickedEditFlat();
	afx_msg void OnBnClickedTestMode();

public:
	afx_msg void OnBnClickedTextureLoad();
	afx_msg void OnBnClickedTextureApply();
	afx_msg void OnBnClickedSplatting();
	afx_msg void OnNMCustomdrawUVRatio(NMHDR *pNMHDR, LRESULT *pResult);

public:
	afx_msg void OnBnClickedMix1Load();
	afx_msg void OnBnClickedMix2Load();
	afx_msg void OnBnClickedMix3Load();
	afx_msg void OnBnClickedMix4Load();
	afx_msg void OnBnClickedMix1();
	afx_msg void OnBnClickedMix2();
	afx_msg void OnBnClickedMix3();
	afx_msg void OnBnClickedMix4();

public:
	afx_msg void OnBnClickedDrawSplatting();
	afx_msg void OnBnClickedEraseSplatting();
	afx_msg void OnBnClickedSplattingSave();
	afx_msg void OnBnClickedSplattingLoad();
	afx_msg void OnNMCustomdrawMixRatio(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedEditTerrainEdit();

public:
	wstring ConvertRelativePath(const wstring& szFullPath);
	_vec3   GetPlayerPos(void);
	_bool	GetEditTerrain(void);

private:
	void Release(void);

private:
	map<_int, Engine::CLayer*>*			m_pMapLayer;
	typedef map<_int, Engine::CLayer*>	MAPLAYER;
	CMainFrame*							m_pMainFrm;

private:
	list<VTXTEX*>						m_VtxList;
	list<VTXTEX*>						m_VtxList2;
	typedef list<VTXTEX*>				VTXLIST;			

private:
	_int m_iVtxSizeX;										//������ �ͷ����� ������X
	_int m_iVtxSizeZ;										//������ �ͷ����� ������Z
	_int m_iVtxItv;											//������ �ͷ����� ����
	_int m_iNum;											//� �ͷ����� �����ߴ���
	_int m_iHeight;											//������ Height
	_int m_iRange;											//������ Range
	_int m_iVtx1Size;										//Terrain1�� ������
	_int m_iVtx2Size;										//Terrain2�� ������
	_int m_iSkyNum;											//���° SkyBox ������ ���õǾ����� �Ǵ�
	_int m_iEditMode;										//� Edit�� �����ߴ��� �Ǵ�
	_int m_iMixNum;											//��� Mix�� ���� �Ǿ�����
	_int m_iMix1Num;										//���° Terrain Texture�� �׸�����?	
	_int m_iMix2Num;										//���° Terrain Texture�� �׸�����?	
	_int m_iMix3Num;										//���° Terrain Texture�� �׸�����?	
	_int m_iMix4Num;										//���° Terrain Texture�� �׸�����?	
	_int m_iTerrain1TextureNum[4];
	_int m_iTerrain2TextureNum[4];
	_int m_iBaseNum;
	_int m_iBaseTextureNum[2];
private:
	_float m_fMoveX;										//X�������� �̵�
	_float m_fMoveY;										//Y�������� �̵�
	_float m_fMoveZ;										//Z�������� �̵�
	_float m_fUVRatio;										//UV��ǥ
	_float m_fTerrain1MixRatio[4];
	_float m_fTerrain2MixRatio[4];
	_float m_fMixRatio;
	_float m_fTerrainUVRatio[2];
private:
	_bool	m_bMove;										//Terrain�� �̵��ߴ��� �Ǵ��ϴ� bool�� ����
	_bool	m_isPlayer;										//Player�� ����������� �Ǵ��ϴ� bool�� ����
	_bool	m_isSkyRender;									//SkyBox�� �׷������� �Ǵ��ϴ� bool�� ����
	_bool	m_isSplatting;									//���÷����� �������� �Ǵ��ϴ� bool�� ����
	_bool	m_isDrawingSplatting;							//���÷����� �׸������� ����� ���� �Ǵ�
	_bool	m_isTest;										//Test��ư�� ���ȴ��� �Ǵ��ϴ� bool�� ����

private:
	CButton 	m_ctrEditUpdown;							//Up & Down ���� ��ư�� ���ȴ��� Ȯ��
	CButton 	m_ctrEditPlateau;							//Plateau ���� ��ư�� ���ȴ��� Ȯ��
	CButton 	m_ctrEditFlat;								//Flat ������ư�� ���ȴ��� Ȯ��

public:
	CButton 	m_ctrTestMode;								//TestMode�� ���ȴ��� Ȯ��(Public��������)

private:
	CButton 	m_WireFrame;								//WireFrame�� On / Off��
	CButton 	m_Terrain1;									//Terrain1�� �����ϴ� ��ư
	CButton 	m_Terrain2;									//Terrain2�� �����ϴ� ��ư
	CButton		m_ctrSplatting;								//Splatting�� �������� Ȯ��
	CButton 	m_ctrMix1;									//Mix1�� ������ư�� �������� Ȯ��
	CButton 	m_ctrMix2;									//Mix2�� ������ư�� �������� Ȯ��
	CButton 	m_ctrMix3;									//Mix3�� ������ư�� �������� Ȯ��
	CButton 	m_ctrMix4;									//Mix4�� ������ư�� �������� Ȯ��
	CButton 	m_ctrDrawSplatting;							//SPlatting�� �׸������� 
	CButton 	m_ctrEraseSplatting;						//Splatting�� ����� ���� �Ǵ�
	CButton		m_ctrTerrainEDit;

	CSliderCtrl m_ctrHeight;								//Height�� �����ϴ� �����̴���Ʈ��
	CSliderCtrl m_ctrRange;									//Range�� �����ϴ� �����̴���Ʈ��
	CSliderCtrl m_ctrUVRatio;								//UV�� �����ϴ� �����̴���Ʈ��
	CSliderCtrl m_ctrMixRatio;								//Mix�� Uv�� �����ϴ� �����̴�

private:
	CString 	m_SkyName;									//SkyBox ��ü�� �Է¹޴� �̸� , Edit�ڽ��� �̸��̱⵵ ��
	CString 	m_CreateSkyName;							//�����Ǵ� SkyBox�� �̸�
	CString 	m_BaseTexture;								//Base Texture ��ü�� �Է¹޴� �̸�
	CString 	m_Mix1Name;									//Mix1�� �̸�			
	CString 	m_Mix2Name;									//Mix2�� �̸�
	CString 	m_Mix3Name;									//Mix3�� �̸�
	CString 	m_Mix4Name;									//Mix4�� �̸�

public:
	Engine::CGameObject*		m_pStaticCamera;			//Static Cam ���� �������
	Engine::CGameObject*		m_pDynamicCamera;			//Dynamic Cam ���� �������
	Engine::CGameObject*		m_pSkyBox;					//SkyBox ���� �������
	Engine::CGameObject*		m_pTerrain;					//Terrain1 ���� �������
	Engine::CGameObject*		m_pSecondTerrain;			//Terrain2 ���� �������
	Engine::CGameObject*		m_pPlayer;					//Test�� Player �������
	Engine::CGameObject*		m_pEnemy;					//Test�� Player �������


private:
	LPDIRECT3DTEXTURE9			m_pAlphaTexture[4];
	LPDIRECT3DSURFACE9			m_pTargetSurface[4];
	IDirect3DBaseTexture9*		m_pTexture;

private:
	wstring						m_strBaseTextureName[2];
	wstring						m_strTerrain1TextureName[4];		//1�ͷ���
	wstring						m_strTerrain2TextureName[4];		//2�ͷ���

private:
	TEXINFO						m_tTexInfo;

private:
	_float						m_fsliderRatio;
	_bool						m_isEditTerrain;
};