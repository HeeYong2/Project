// MapPage.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "MapPage.h"
#include "Terrain.h"
#include "Terrain2.h"
#include "Layer.h"
#include "ResourceMgr.h"
#include "ToolView.h"
#include "DynamicCamera.h"
#include "StaticCamera.h"
#include "SkyBox.h"
#include "Player.h"
#include "Enemy.h"
#include "Export_Function.h"

// CMapPage 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMapPage, CPropertyPage)

CMapPage::CMapPage()
	: CPropertyPage(CMapPage::IDD)
	, m_iVtxItv(2)
	, m_iVtxSizeX(65)
	, m_iVtxSizeZ(65)
	, m_iVtx1Size(129)
	, m_iVtx2Size(129)
	, m_iHeight(0)
	, m_iRange(0)
	, m_fMoveX(0)
	, m_fMoveY(0)
	, m_fMoveZ(0)
	, m_iEditMode(0)
	, m_iBaseNum(0)
	, m_iMixNum(0)
	, m_iMix1Num(0)
	, m_iMix2Num(0)
	, m_iMix3Num(0)
	, m_iMix4Num(0)
	, m_iNum(0)
	, m_fUVRatio(1.f)
	, m_fMixRatio(1.f)
	, m_fsliderRatio(1.f)
	, m_SkyName(_T(""))
	, m_CreateSkyName(_T(""))
	, m_BaseTexture(_T(""))
	, m_Mix1Name(_T(""))
	, m_Mix2Name(_T(""))
	, m_Mix3Name(_T(""))
	, m_Mix4Name(_T(""))
	, m_isSkyRender(false)
	, m_isTest(false)
	, m_isPlayer(false)
	, m_isSplatting(false)
	, m_bMove(false)
	, m_isDrawingSplatting(false)
	, m_isEditTerrain(false)
	, m_pPlayer(NULL)
	, m_pEnemy(NULL)
	, m_pMapLayer(NULL)
	, m_pTerrain(NULL)
	, m_pSecondTerrain(NULL)
	, m_pStaticCamera(NULL)
	, m_pDynamicCamera(NULL)
{
	ZeroMemory(m_iTerrain1TextureNum , sizeof(_int) * 4);
	ZeroMemory(m_iTerrain2TextureNum , sizeof(_int) * 4);
	ZeroMemory(m_iBaseTextureNum, sizeof(_int) * 2);
	ZeroMemory(m_fTerrainUVRatio, sizeof(_float) * 2);
 	ZeroMemory(m_fTerrain1MixRatio, sizeof(_float) * 4);
 	ZeroMemory(m_fTerrain2MixRatio, sizeof(_float) * 4);
	memset(&m_tTexInfo , 0 , sizeof(TEXINFO));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

CMapPage::~CMapPage()
{
	Release();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CMapPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_iVtxSizeX);
	DDX_Text(pDX, IDC_EDIT2, m_iVtxSizeZ);
	DDX_Text(pDX, IDC_EDIT3, m_iVtxItv);
	DDX_Text(pDX, IDC_EDIT4, m_SkyName);
	DDX_Text(pDX, IDC_EDIT5, m_iHeight);
	DDX_Text(pDX, IDC_EDIT6, m_iRange);
	DDX_Text(pDX, IDC_EDIT7, m_fMoveX);
	DDX_Text(pDX, IDC_EDIT10, m_fMoveY);
	DDX_Text(pDX, IDC_EDIT11, m_fMoveZ);
	DDX_Text(pDX, IDC_EDIT14, m_BaseTexture);
	DDX_Text(pDX, IDC_EDIT15, m_fUVRatio);
	DDX_Text(pDX, IDC_EDIT16, m_Mix1Name);
	DDX_Text(pDX, IDC_EDIT17, m_Mix2Name);
	DDX_Text(pDX, IDC_EDIT18, m_Mix3Name);
	DDX_Text(pDX, IDC_EDIT19, m_Mix4Name);
	DDX_Text(pDX, IDC_EDIT21, m_fMixRatio);
	DDX_Control(pDX, IDC_SLIDER1, m_ctrHeight);
	DDX_Control(pDX, IDC_SLIDER2, m_ctrRange);
	DDX_Control(pDX, IDC_SLIDER10, m_ctrUVRatio);
	DDX_Control(pDX, IDC_SLIDER11, m_ctrMixRatio);
	DDX_Control(pDX, IDC_CHECK1, m_WireFrame);
	DDX_Control(pDX, IDC_CHECK3, m_ctrTestMode);
	DDX_Control(pDX, IDC_CHECK4, m_ctrSplatting);
	DDX_Control(pDX, IDC_RADIO1, m_Terrain1);
	DDX_Control(pDX, IDC_RADIO2, m_Terrain2);
	DDX_Control(pDX, IDC_RADIO3, m_ctrEditUpdown);
	DDX_Control(pDX, IDC_RADIO4, m_ctrEditPlateau);
	DDX_Control(pDX, IDC_RADIO5, m_ctrEditFlat);
	DDX_Control(pDX, IDC_RADIO6, m_ctrMix1);
	DDX_Control(pDX, IDC_RADIO7, m_ctrMix2);
	DDX_Control(pDX, IDC_RADIO8, m_ctrMix3);
	DDX_Control(pDX, IDC_RADIO9, m_ctrMix4);
	DDX_Control(pDX, IDC_RADIO10, m_ctrDrawSplatting);
	DDX_Control(pDX, IDC_RADIO11, m_ctrEraseSplatting);
	DDX_Control(pDX, IDC_CHECK5, m_ctrTerrainEDit);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CMapPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, &CMapPage::OnBnClickedApplyTerrain)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CMapPage::OnNMCustomdrawHeight)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CMapPage::OnNMCustomdrawRange)
	ON_BN_CLICKED(IDC_CHECK1, &CMapPage::OnBnClickedWireFrame)
	ON_BN_CLICKED(IDOK, &CMapPage::OnBnClickedSave)
	ON_BN_CLICKED(IDCANCEL, &CMapPage::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_RADIO1, &CMapPage::OnBnClickedRadio)
	ON_BN_CLICKED(IDC_RADIO2, &CMapPage::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_BUTTON4, &CMapPage::OnBnClickedMoveApply)
	ON_BN_CLICKED(IDC_BUTTON2, &CMapPage::OnBnClickedApplySkyBox)
	ON_BN_CLICKED(IDC_BUTTON3, &CMapPage::OnBnClickedLoadSkyBox)
	ON_BN_CLICKED(IDC_RADIO3, &CMapPage::OnBnClickedUpdown)
	ON_BN_CLICKED(IDC_RADIO4, &CMapPage::OnBnClickedPlateau)
	ON_BN_CLICKED(IDC_RADIO5, &CMapPage::OnBnClickedEditFlat)
	ON_BN_CLICKED(IDC_CHECK3, &CMapPage::OnBnClickedTestMode)
	ON_BN_CLICKED(IDC_BUTTON5, &CMapPage::OnBnClickedTextureLoad)
	ON_BN_CLICKED(IDC_BUTTON6, &CMapPage::OnBnClickedTextureApply)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER10, &CMapPage::OnNMCustomdrawUVRatio)
	ON_BN_CLICKED(IDC_CHECK4, &CMapPage::OnBnClickedSplatting)
	ON_BN_CLICKED(IDC_RADIO6, &CMapPage::OnBnClickedMix1)
	ON_BN_CLICKED(IDC_BUTTON8, &CMapPage::OnBnClickedMix1Load)
	ON_BN_CLICKED(IDC_BUTTON11, &CMapPage::OnBnClickedMix2Load)
	ON_BN_CLICKED(IDC_BUTTON14, &CMapPage::OnBnClickedMix3Load)
	ON_BN_CLICKED(IDC_BUTTON16, &CMapPage::OnBnClickedMix4Load)
	ON_BN_CLICKED(IDC_RADIO7, &CMapPage::OnBnClickedMix2)
	ON_BN_CLICKED(IDC_RADIO8, &CMapPage::OnBnClickedMix3)
	ON_BN_CLICKED(IDC_RADIO9, &CMapPage::OnBnClickedMix4)
	ON_BN_CLICKED(IDC_RADIO10, &CMapPage::OnBnClickedDrawSplatting)
	ON_BN_CLICKED(IDC_RADIO11, &CMapPage::OnBnClickedEraseSplatting)
	ON_BN_CLICKED(IDC_BUTTON18, &CMapPage::OnBnClickedSplattingSave)
	ON_BN_CLICKED(IDC_BUTTON19, &CMapPage::OnBnClickedSplattingLoad)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER11, &CMapPage::OnNMCustomdrawMixRatio)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_CHECK5, &CMapPage::OnBnClickedEditTerrainEdit)
END_MESSAGE_MAP()


// CMapPage 메시지 처리기입니다.
//--------------------------------------------------------------------------------------------------------------------------------------------------------

BOOL CMapPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();



	//Layer Initialize Begin
	m_pMainFrm = (CMainFrame*)AfxGetMainWnd();
	Engine::CLayer*		pEnvironLayer = Engine::CLayer::Create();
	Engine::CLayer*		pUILayer = Engine::CLayer::Create();
	Engine::CLayer*		pGameLogicLayer = Engine::CLayer::Create();
	
	m_pMapLayer = &m_pMainFrm->m_pMainView->m_mapLayer;

	if(m_pMapLayer != NULL)
		m_pMapLayer->insert(MAPLAYER::value_type(LAYER_ENVIRONMENT , pEnvironLayer));

	if(m_pMapLayer != NULL)
		m_pMapLayer->insert(MAPLAYER::value_type(LAYER_UI , pUILayer));

	if(m_pMapLayer != NULL)
		m_pMapLayer->insert(MAPLAYER::value_type(LAYER_GAMELOGIC , pGameLogicLayer));
	//Layer Initialize End

	//Slider Control Begin
	m_ctrHeight.SetRange(-10 , 60);
	m_ctrHeight.SetPos(0);
	m_ctrHeight.SetTicFreq(1);

	m_ctrRange.SetRange(0 , 100);
	m_ctrRange.SetPos(0);
	m_ctrRange.SetTicFreq(1);

	m_ctrUVRatio.SetRange(1, 100);
	m_ctrUVRatio.SetPos(1);
	//m_ctrUVRatio.SetTicFreq(1);
	m_fUVRatio = (_float)m_ctrUVRatio.GetPos() / m_fsliderRatio;

	m_ctrMixRatio.SetRange(1, 100);
	m_ctrMixRatio.SetPos(1);
	//m_ctrMixRatio.SetTicFreq(1);
	m_fMixRatio = (_float)m_ctrMixRatio.GetPos() / m_fsliderRatio;


	//Slider Control End

	m_Terrain1.SetCheck(BST_CHECKED);	
	m_ctrEditUpdown.SetCheck(BST_CHECKED);
	m_ctrEditUpdown.SetCheck(BST_CHECKED);
	m_ctrDrawSplatting.SetCheck(BST_CHECKED);

	for(int i = 0 ; i < 4; ++i)
	{
		m_fTerrain1MixRatio[i] = 1.f;
		m_fTerrain2MixRatio[i] = 1.f;
	}
	return TRUE;  
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void CMapPage::OnBnClickedApplyTerrain()
{
	UpdateData(TRUE);	
	
	if(m_iVtxSizeX <= 0)
	{
		AfxMessageBox(L"0으로는 만들수 없습니다 " , MB_OK);
		return;
	}
	
	if(m_Terrain1.GetCheck() == 1)
	{
		Engine::Ready_Terrain_Buffers(Engine::GetGraphicDev() , RESOURCE_STAGE , L"Buffer_Terrain" , m_iVtxSizeX ,m_iVtxSizeZ , float(m_iVtxItv) , 0);

		if(FAILED(Ready_EnvironmentLayer(m_iVtxSizeX , m_iVtxSizeZ , float(m_iVtxItv))))
		{
			AfxMessageBox(L"Ready_EnvironmentLayer  생성 실패");
			return ;
		}
	}
	else if(m_Terrain2.GetCheck() == 1)
	{
		Engine::Ready_Terrain_Buffers(Engine::GetGraphicDev() , RESOURCE_STAGE , L"Buffer_Terrain2" , m_iVtxSizeX ,m_iVtxSizeZ , float(m_iVtxItv) , 1);

		if(FAILED(Ready_Terrain(m_iVtxSizeX , m_iVtxSizeZ , float(m_iVtxItv))))
		{
			AfxMessageBox(L"Ready_EnvironmentLayer  생성 실패");
			return ;
		}
	}

	if(FAILED(Ready_UI()))
	{ 
		AfxMessageBox(L"Ready_UI  생성 실패");
		return ;
	}

	UpdateData(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

HRESULT CMapPage::Ready_EnvironmentLayer(const _uint& wCntX, const _uint& wCntZ, const float& fItv)
{
	//Terrain Initialize Begin
	//=Terrain===============================================================================================

	m_pTerrain = CTerrain::Create(wCntX , wCntZ , float(fItv));
	m_iVtx1Size = wCntX;
	((CTerrain*)m_pTerrain)->SetUVRatio(m_fUVRatio);

	if(m_pTerrain == NULL)
		return E_FAIL;

	(*m_pMapLayer)[LAYER_ENVIRONMENT]->Insert_GameObject(L"ToolTerrain" , m_pTerrain);

	m_pMainFrm->m_pMainView->SetTerrain(m_pTerrain);

	return S_OK;
	//Terrain Initialize End
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

HRESULT CMapPage::Ready_Terrain(const _uint& wCntX, const _uint& wCntZ, const float& fItv)
{
	m_pSecondTerrain = CTerrain2::Create(wCntX , wCntZ , float(fItv));
	m_iVtx2Size = wCntX;
	if(m_pSecondTerrain == NULL)
		return E_FAIL;
	((CTerrain2*)m_pSecondTerrain)->SetUVRatio(m_fUVRatio);
	(*m_pMapLayer)[LAYER_ENVIRONMENT]->Insert_GameObject(L"ToolTerrain2" , m_pSecondTerrain);

	m_pMainFrm->m_pMainView->SetSecondTerrain(m_pSecondTerrain);

	return S_OK;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

HRESULT CMapPage::Ready_Player(void)
{
	m_pPlayer = CPlayer::Create();
	Ready_StaticCamera();
	if(m_pPlayer == NULL)
		return E_FAIL;

	(*m_pMapLayer)[LAYER_GAMELOGIC]->Insert_GameObject(L"Player" , m_pPlayer);

	return S_OK;	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

HRESULT CMapPage::Ready_Enemy(void)
{
	m_pEnemy = CEnemy::Create();
	if(m_pEnemy == NULL)
		return E_FAIL;

	(*m_pMapLayer)[LAYER_GAMELOGIC]->Insert_GameObject(L"Enemy" , m_pEnemy);

	return S_OK;	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CMapPage::Ready_StaticCamera(void)
{
	m_pStaticCamera = NULL;	
	m_pStaticCamera = CStaticCamera::Create(Engine::GetGraphicDev() , &D3DXVECTOR3(0.f, 1.f, 10.f)
		, &D3DXVECTOR3(0.f, 0.f, 0.f)
		, &D3DXVECTOR3(0.f, 1.f, 0.f));

	if(m_pStaticCamera == NULL)
		return E_FAIL;

	((CStaticCamera*)m_pStaticCamera)->SetTransCamera( ((CPlayer*)m_pPlayer)->GetPlayerInfo() );
	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

HRESULT CMapPage::Ready_UI(void)
{
	//Camera Initialize Begin
	m_pDynamicCamera = NULL;	
	m_pDynamicCamera = CDynamicCamera::Create(Engine::GetGraphicDev(), &_vec3(0.f, 10.f, -20.f), &_vec3(0.f, 0.0f, 0.0f), &_vec3(0.f, 1.f, 0.f));

	if(m_pDynamicCamera == NULL)
		return E_FAIL;

	return S_OK;
	//Camera Initialize End
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

HRESULT CMapPage::Ready_SkyBox(CString strSkyName)
{
	m_pSkyBox = NULL;	
	m_pSkyBox = CSkyBox::Create(strSkyName);
	((CSkyBox*)m_pSkyBox)->SetSkyNum(m_iSkyNum);

	if(m_pSkyBox == NULL)
		return E_FAIL;

	(*m_pMapLayer)[LAYER_UI]->Insert_GameObject(L"SkyBox" , m_pSkyBox); 

	return S_OK;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CMapPage::OnNMCustomdrawHeight(NMHDR *pNMHDR, LRESULT *pResult)		//실시간 슬라이더컨트롤의 높이
{
	//실시간 Slider Height
	UpdateData(TRUE);
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	m_iHeight = m_ctrHeight.GetPos();

	*pResult = 0;
	UpdateData(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CMapPage::OnNMCustomdrawRange(NMHDR *pNMHDR, LRESULT *pResult)			//실시간 슬라이더컨트롤의 범위
{
	//실시간 Slider Range
	UpdateData(TRUE);
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);

	m_iRange = m_ctrRange.GetPos();

	*pResult = 0;
	UpdateData(FALSE);
}


//--------------------------------------------------------------------------------------------------------------------------------------------------------
void CMapPage::OnBnClickedWireFrame()										//WireFrame의 On / Off
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);																			//	UpdateData(TRUE);	0518추가

	if(m_Terrain1.GetCheck() == 1)
	{
		if(m_WireFrame.GetCheck())
			((CTerrain*)m_pTerrain)->SetFrameType(3);
		else
			((CTerrain*)m_pTerrain)->SetFrameType(2);
	}
	if(m_Terrain2.GetCheck() == 1)
	{
		if(m_WireFrame.GetCheck())
			((CTerrain2*)m_pSecondTerrain)->SetFrameType(3);
		else
			((CTerrain2*)m_pSecondTerrain)->SetFrameType(2);
	}
	UpdateData(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CMapPage::OnBnClickedSave
()											//Terrain Save
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CFileDialog		Dlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.dat", this);

	TCHAR		szDirtoryPath[MAX_PATH];
	GetCurrentDirectory(sizeof(szDirtoryPath), szDirtoryPath);
	SetCurrentDirectory(szDirtoryPath);

	PathRemoveFileSpec(szDirtoryPath);
	lstrcat(szDirtoryPath, L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szDirtoryPath;

	if(Dlg.DoModal() == IDCANCEL)
		return;

	DWORD		dwByte = 0;

	HANDLE hFile = CreateFile(Dlg.GetPathName(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS
		, FILE_ATTRIBUTE_NORMAL, NULL);

	if(m_Terrain1.GetCheck() == 1)
	{
		Engine::VTXTEX* pTerrainVtx = ((CTerrain*)m_pTerrain)->GetVtxTex();

		for(_int z = 0 ; z < m_iVtx1Size ; ++z)
		{
			for(_int x = 0 ; x < m_iVtx1Size; ++x)
			{
				_int iIndex = m_iVtxSizeX * z + x;
				pTerrainVtx[iIndex].vTexUV = _vec2(m_fTerrainUVRatio[0] , m_fTerrainUVRatio[0]);
				WriteFile(hFile, &pTerrainVtx[iIndex], sizeof(Engine::VTXTEX), &dwByte, NULL);
			}
		}
	}
	if(m_Terrain2.GetCheck() == 1)
	{
		Engine::VTXTEX* pTerrainVtx = ((CTerrain2*)m_pSecondTerrain)->GetVtxTex();

		for(_int z = 0 ; z < m_iVtx2Size ; ++z)
		{
			for(_int x = 0 ; x < m_iVtx2Size; ++x)
			{
				_int iIndex = m_iVtxSizeX * z + x;
				pTerrainVtx[iIndex].vTexUV = _vec2(m_fTerrainUVRatio[1] , m_fTerrainUVRatio[1]);
				WriteFile(hFile, &pTerrainVtx[iIndex], sizeof(Engine::VTXTEX), &dwByte, NULL);
			}
		}
	}

	CloseHandle(hFile);
	UpdateData(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CMapPage::OnBnClickedLoad()							//Terrain Load
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CFileDialog		Dlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.dat", this);


	if(Dlg.DoModal() == IDCANCEL)
		return;

	HANDLE		hFile;
	DWORD		dwByte;

	hFile = CreateFile(Dlg.GetPathName(), GENERIC_READ, 0, NULL, OPEN_EXISTING
		, FILE_ATTRIBUTE_NORMAL, NULL);

	if(m_Terrain1.GetCheck() == 1)
	{
		while( true )
		{
			VTXTEX*	pVtxTex = new VTXTEX;
			ReadFile(hFile, pVtxTex, sizeof(VTXTEX), &dwByte, NULL);

			if(dwByte == 0)
			{
				::safe_delete(pVtxTex);
				break;
			}
			m_VtxList.push_back(pVtxTex);
		}

		if(FAILED(Ready_UI()))
			return ;

		VTXLIST::iterator iter = m_VtxList.begin();
		_int iSize = int(sqrt(_float(m_VtxList.size())));

		m_iVtxSizeX = iSize;
		m_iVtxSizeZ = iSize;

		Engine::Ready_Terrain_Buffers(Engine::GetGraphicDev() , RESOURCE_STAGE , L"Buffer_Terrain" , iSize ,iSize , 1 , 0);

		//터레인dds

		if(FAILED(Ready_EnvironmentLayer(iSize , iSize  , 1)))
			return ;

		Engine::VTXTEX*		pTerrainVtx = ((CTerrain*)m_pTerrain)->GetVtxTex();

		for(_int z = 0 ; z < iSize ; ++z)
		{
			for(_int x = 0 ; x < iSize ; ++x , ++iter)
			{
				int iIndex = iSize * z + x;
				pTerrainVtx[iIndex].vPosition = (*iter)->vPosition;
			} 
		}
		((CTerrain*)m_pTerrain)->SetLoadVerTex(pTerrainVtx->vPosition);	
	}
	
	if(m_Terrain2.GetCheck() == 1)
	{
		while( true )
		{
			VTXTEX*	pVtxTex = new VTXTEX;
			ReadFile(hFile, pVtxTex, sizeof(VTXTEX), &dwByte, NULL);

			if(dwByte == 0)
			{
				::safe_delete(pVtxTex);
				break; 
			}
			m_VtxList2.push_back(pVtxTex);
		}

		if(FAILED(Ready_UI()))
			return ;

		VTXLIST::iterator iter = m_VtxList2.begin();
		_int iSize = int(sqrt(_float(m_VtxList2.size())));

		m_iVtxSizeX = iSize;
		m_iVtxSizeZ = iSize;
		_float fItv = 2;
		m_iVtxItv = _int(fItv);


		Engine::Ready_Terrain_Buffers(Engine::GetGraphicDev() , RESOURCE_STAGE , L"Buffer_Terrain2" , iSize ,iSize , float(fItv) , 1);
		//터레인dds
		if(FAILED(Ready_Terrain(iSize , iSize  , fItv)))	
			return ;

		Engine::VTXTEX*		pTerrainVtx = ((CTerrain2*)m_pSecondTerrain)->GetVtxTex();

		for(_int z = 0 ; z < iSize ; ++z)
		{
			for(_int x = 0 ; x < iSize ; ++x , ++iter)
			{
				int iIndex = iSize * z + x;
				pTerrainVtx[iIndex].vPosition = (*iter)->vPosition;
			}
		}
		((CTerrain2*)m_pSecondTerrain)->SetLoadVerTex(pTerrainVtx->vPosition);	
	}
		//!!마지막에 셋 버텍스 한번 해보자 !!
	Invalidate(FALSE);
	CloseHandle(hFile);
	UpdateData(FALSE);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CMapPage::OnBnClickedRadio()											//첫번째 Terrain 선택
{
	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iNum = 0;
	m_iVtxSizeX = m_iVtx1Size;
	m_iVtxSizeZ = m_iVtx1Size;
	m_Mix1Name = m_strTerrain1TextureName[0].c_str();
	m_Mix2Name = m_strTerrain1TextureName[1].c_str();
	m_Mix3Name = m_strTerrain1TextureName[2].c_str();
	m_Mix4Name = m_strTerrain1TextureName[3].c_str();
	m_BaseTexture = m_strBaseTextureName[0].c_str();
	m_fUVRatio = m_fTerrainUVRatio[0] ;

	UpdateData(FALSE);
	m_ctrUVRatio.SetPos( (_int)(m_fTerrainUVRatio[0] ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CMapPage::OnBnClickedRadio2()											//두번째 Terrain 선택
{
	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iNum = 1;
	m_iVtxSizeX = m_iVtx2Size;
	m_iVtxSizeZ = m_iVtx2Size;
	m_Mix1Name = m_strTerrain2TextureName[0].c_str();
	m_Mix2Name = m_strTerrain2TextureName[1].c_str();
	m_Mix3Name = m_strTerrain2TextureName[2].c_str();
	m_Mix4Name = m_strTerrain2TextureName[3].c_str();
	m_BaseTexture = m_strBaseTextureName[1].c_str();
	m_fUVRatio = m_fTerrainUVRatio[1] ;
	UpdateData(FALSE);
	m_ctrUVRatio.SetPos( (_int)(m_fTerrainUVRatio[1] ) );
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CMapPage::OnBnClickedMoveApply()										//Move 버튼 클릭
{
	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bMove = true;
	UpdateData(FALSE);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CMapPage::OnBnClickedApplySkyBox()										 //SkyBox 생성
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_isSkyRender == false)
	{
		m_isSkyRender = true;
		if(FAILED(Ready_SkyBox(m_CreateSkyName)))
		{ 
			AfxMessageBox(L"Ready_SkyBox  생성 실패");
			return ;
		}
	}
	else
		((CSkyBox*)m_pSkyBox)->SetSkyNum(m_iSkyNum);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CMapPage::OnBnClickedLoadSkyBox()										//SkyBox 경로 읽어오기
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CFileDialog		Dlg(TRUE, L"x", L"*.*", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.*", this);


	if(Dlg.DoModal() == IDCANCEL)
		return;

	TCHAR		szPath[MAX_PATH] = L"";
	wstring		wstrSkyName;
	wstring		wstrTextureName = Dlg.GetFileTitle().operator LPCWSTR();


	lstrcpy(szPath, (LPWSTR)Dlg.GetPathName().operator LPCWSTR());
	PathRemoveFileSpec(szPath);

	//상대 경로
	PathCombine(szPath, szPath, (LPWSTR)wstrTextureName.c_str());
	wstrSkyName = ConvertRelativePath(szPath);

	//StateKey
	PathRemoveFileSpec(szPath);
	wstrSkyName = PathFindFileName(szPath);

	//ObjKey
	m_CreateSkyName = wstrSkyName.c_str();

///////////////////////////////////////////////////////////////////////////////
	TCHAR		szPath2[MAX_PATH] = L"";
	wstring		wstrSkyName2;
	wstring		wstrTextureName2 = Dlg.GetFileTitle().operator LPCWSTR();


	lstrcpy(szPath2, (LPWSTR)Dlg.GetPathName().operator LPCWSTR());
	PathRemoveFileSpec(szPath2);

	//상대 경로
	PathCombine(szPath2, szPath2, (LPWSTR)wstrTextureName2.c_str());
	wstrSkyName2 = ConvertRelativePath(szPath2);

	//StateKey
	//PathRemoveFileSpec(szPath2);
	wstrSkyName2 = PathFindFileName(szPath2);

	wstring wstrFileTag2 = wstrSkyName2.substr(6, wstrSkyName2.length() );
	m_iSkyNum = _wtoi(wstrFileTag2.c_str());

	//ObjKey
	m_SkyName = wstrSkyName2.c_str();

	UpdateData(FALSE);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

std::wstring CMapPage::ConvertRelativePath(const wstring& szFullPath)			//SkyBox 경로 읽어오기
{
	TCHAR		szRelativePath[MAX_PATH] = L"";
	TCHAR		szDirtoryPath[MAX_PATH] = L"";

	GetCurrentDirectory(sizeof(szDirtoryPath), szDirtoryPath);
	PathRelativePathTo((LPWSTR)szRelativePath
		, szDirtoryPath
		, FILE_ATTRIBUTE_DIRECTORY
		, szFullPath.c_str()
		, FILE_ATTRIBUTE_NORMAL);

	return szRelativePath;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CMapPage::OnBnClickedUpdown()												//Up & Down 수정버튼 선택
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iEditMode = 0;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CMapPage::OnBnClickedPlateau()												//Plateau 수정버튼 선택
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iEditMode = 1;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CMapPage::OnBnClickedEditFlat()											//Flat 수정버튼 선택
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iEditMode =2;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CMapPage::OnBnClickedTestMode()											//Test모드 선택
{
	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_isPlayer == false)
	{
		Ready_Player();	
		Ready_Enemy();
	}

	m_isPlayer = true;

 	if( m_ctrTestMode.GetCheck() == 1 )
 		m_isTest = true;
 	else
 		m_isTest = false;
	UpdateData(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CMapPage::OnBnClickedTextureLoad()											//Terrain Texture를 선택
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CFileDialog		Dlg(TRUE, L"dds", L"*.*", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.*", this);


	if(Dlg.DoModal() == IDCANCEL)
		return;

	///////////////////////////////////////////////////////////////////////////////
	TCHAR		szPath2[MAX_PATH] = L"";
	wstring		wstrTextureName;
	wstring		wstrTempName = Dlg.GetFileTitle().operator LPCWSTR();


	lstrcpy(szPath2, (LPWSTR)Dlg.GetPathName().operator LPCWSTR());
	PathRemoveFileSpec(szPath2);

	//상대 경로
	PathCombine(szPath2, szPath2, (LPWSTR)wstrTempName.c_str());
	wstrTextureName = ConvertRelativePath(szPath2);

	//StateKey
	//PathRemoveFileSpec(szPath2);
	wstrTextureName = PathFindFileName(szPath2);

	wstring wstrFileTag = wstrTextureName.substr(7, wstrTextureName.length() );
	m_iBaseNum = _wtoi(wstrFileTag.c_str());


	//ObjKey
	m_BaseTexture = wstrTextureName.c_str();

	if(m_Terrain1.GetCheck() == 1)
	{
		m_iBaseTextureNum[0] = m_iBaseNum;
		m_strBaseTextureName[0] = m_BaseTexture;
	}
	else
	{
		m_iBaseTextureNum[1] = m_iBaseNum;
		m_strBaseTextureName[1] = m_BaseTexture;
	}

	UpdateData(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CMapPage::OnBnClickedTextureApply()										//Terrain Texture를 씌운다
{
	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_pTerrain != NULL)
	{
		if(m_Terrain1.GetCheck() == 1)
			((CTerrain*)m_pTerrain)->SetTextureNum(m_iBaseTextureNum[0]);
	}

	if(m_pSecondTerrain != NULL)
	{
		if(m_Terrain2.GetCheck() == 1)
			((CTerrain2*)m_pSecondTerrain)->SetTextureNum(m_iBaseTextureNum[1]);
	}

	UpdateData(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

_bool CMapPage::GetTest(void)													//Test 모드를 선택했는지
{
	return m_isTest;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
_int CMapPage::GetMode(void)													//어떤 수정버튼을 선택했는지
{
	return m_iEditMode;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

_int CMapPage::GetHeight(void)													//높이의 수치
{
	return m_ctrHeight.GetPos();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

_int CMapPage::GetRange(void)													//범위의 수치
{
	return m_ctrRange.GetPos();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

_int CMapPage::GetVtx1Size(void)												//Vtx1의 사이즈
{
	return m_iVtx1Size;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
_int CMapPage::GetVtx2Size(void)												//Vtx2의 사이즈
{
	return m_iVtx2Size;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

_int CMapPage::GetNum(void)														//어떤 터레인이 선택 되었는지
{
	return m_iNum;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CMapPage::SetMove(_bool isMove)											//움직임의 제한
{
	m_bMove = isMove;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CMapPage::SetRadio(_int iSelectRadio)
{
	UpdateData(TRUE);									//0518 	UpdateData(TRUE);				추가
	if(iSelectRadio == 1)
	{
		m_iNum = 0;
		m_iVtxSizeX = m_iVtx1Size;
		m_iVtxSizeZ = m_iVtx1Size;
	}
	else if(iSelectRadio == 2)
	{
		m_iNum = 1;
		m_iVtxSizeX = m_iVtx2Size;
		m_iVtxSizeZ = m_iVtx2Size;
	}
	UpdateData(FALSE);					
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

_vec3 CMapPage::GetPos(void)													//Pos를 알아오자
{
	_vec3 vPos = _vec3(m_fMoveX * m_iVtxItv, m_fMoveY , m_fMoveZ * m_iVtxItv);
	return vPos;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

_bool CMapPage::GetMove(void)													//현재 Move의 상태
{
	return m_bMove;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CMapPage::Release(void)
{
	VTXLIST::iterator iter = m_VtxList.begin();
	VTXLIST::iterator iter_end = m_VtxList.end();

	for( ; iter != iter_end; ++iter)
	{
		::safe_delete((*iter));
	}

	VTXLIST::iterator iter2 = m_VtxList2.begin();
	VTXLIST::iterator iter2_end = m_VtxList2.end();

	for( ; iter2 != iter2_end; ++iter2)
	{
		::safe_delete((*iter2));
	}
}

void CMapPage::OnNMCustomdrawUVRatio(NMHDR *pNMHDR, LRESULT *pResult)
{//유브이
	UpdateData(TRUE);
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_fUVRatio = (_float)m_ctrUVRatio.GetPos() / m_fsliderRatio; 

	if(m_pTerrain != NULL)
	{	
		if(m_Terrain1.GetCheck() == 1)
		{
			((CTerrain*)m_pTerrain)->SetUVRatio(m_fUVRatio);
			m_fTerrainUVRatio[0] = m_fUVRatio;
		}
	}

	if(m_pSecondTerrain != NULL)
	{
		if(m_Terrain2.GetCheck() == 1)
		{
			((CTerrain2*)m_pSecondTerrain)->SetUVRatio(m_fUVRatio);
			m_fTerrainUVRatio[1] = m_fUVRatio;
		}
	}

	*pResult = 0;
	UpdateData(FALSE);
}

void CMapPage::OnBnClickedSplatting()
{
	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(m_ctrSplatting.GetCheck() == 1)
	{
		if(m_pTerrain != NULL)
			((CTerrain*)m_pTerrain)->CreateAlphaMap();

		if(m_pSecondTerrain != NULL)
			((CTerrain2*)m_pSecondTerrain)->CreateAlphaMap();

		m_isSplatting = true;
	}
	else
		m_isSplatting = false;

	UpdateData(FALSE);
}

_bool CMapPage::GetSplatting(void)
{
	return m_isSplatting;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

_int CMapPage::GetMixNum(void)
{
	return m_iMixNum;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CMapPage::OnBnClickedMix1Load()
{
	UpdateData(TRUE);

	CFileDialog		Dlg(TRUE, L"dds", L"*.*", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.*", this);


	if(Dlg.DoModal() == IDCANCEL)
		return;

	///////////////////////////////////////////////////////////////////////////////
	TCHAR		szPath[MAX_PATH] = L"";
	wstring		wstrTextureName;
	wstring		wstrTempName = Dlg.GetFileTitle().operator LPCWSTR();


	lstrcpy(szPath, (LPWSTR)Dlg.GetPathName().operator LPCWSTR());
	PathRemoveFileSpec(szPath);

	//상대 경로
	PathCombine(szPath, szPath, (LPWSTR)wstrTempName.c_str());
	wstrTextureName = ConvertRelativePath(szPath);

	//StateKey
	wstrTextureName = PathFindFileName(szPath);

	wstring wstrFileTag = wstrTextureName.substr(7, wstrTextureName.length() );
	m_iMix1Num = _wtoi(wstrFileTag.c_str());

	//ObjKey
	m_Mix1Name = wstrTextureName.c_str();

	if(m_Terrain1.GetCheck() == 1)
	{
		m_strTerrain1TextureName[0] = wstrTextureName;
		((CTerrain*)m_pTerrain)->SetMix1num(m_iMix1Num);
		m_iTerrain1TextureNum[0] = m_iMix1Num;
	}
	else
	{
		m_strTerrain2TextureName[0] = wstrTextureName;
		((CTerrain2*)m_pSecondTerrain)->SetMix1num(m_iMix1Num);
		m_iTerrain2TextureNum[0] = m_iMix1Num;
	}

	UpdateData(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CMapPage::OnBnClickedMix2Load()
{
	UpdateData(TRUE);

	CFileDialog		Dlg(TRUE, L"dds", L"*.*", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.*", this);


	if(Dlg.DoModal() == IDCANCEL)
		return;

	///////////////////////////////////////////////////////////////////////////////
	TCHAR		szPath[MAX_PATH] = L"";
	wstring		wstrTextureName;
	wstring		wstrTempName = Dlg.GetFileTitle().operator LPCWSTR();


	lstrcpy(szPath, (LPWSTR)Dlg.GetPathName().operator LPCWSTR());
	PathRemoveFileSpec(szPath);

	//상대 경로
	PathCombine(szPath, szPath, (LPWSTR)wstrTempName.c_str());
	wstrTextureName = ConvertRelativePath(szPath);

	//StateKey
	wstrTextureName = PathFindFileName(szPath);

	wstring wstrFileTag = wstrTextureName.substr(7, wstrTextureName.length() );
	m_iMix2Num = _wtoi(wstrFileTag.c_str());

	//ObjKey
	m_Mix2Name = wstrTextureName.c_str();

	if(m_Terrain1.GetCheck() == 1)
	{
		m_strTerrain1TextureName[1] = wstrTextureName;
		((CTerrain*)m_pTerrain)->SetMix2num(m_iMix2Num);
		m_iTerrain1TextureNum[1] = m_iMix2Num;
	}
	else
	{
		m_strTerrain2TextureName[1] = wstrTextureName;
		((CTerrain2*)m_pSecondTerrain)->SetMix2num(m_iMix2Num);
		m_iTerrain2TextureNum[1] = m_iMix2Num;
	}

	UpdateData(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CMapPage::OnBnClickedMix3Load()
{
	UpdateData(TRUE);

	CFileDialog		Dlg(TRUE, L"dds", L"*.*", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.*", this);


	if(Dlg.DoModal() == IDCANCEL)
		return;

	///////////////////////////////////////////////////////////////////////////////
	TCHAR		szPath[MAX_PATH] = L"";
	wstring		wstrTextureName;
	wstring		wstrTempName = Dlg.GetFileTitle().operator LPCWSTR();


	lstrcpy(szPath, (LPWSTR)Dlg.GetPathName().operator LPCWSTR());
	PathRemoveFileSpec(szPath);

	//상대 경로
	PathCombine(szPath, szPath, (LPWSTR)wstrTempName.c_str());
	wstrTextureName = ConvertRelativePath(szPath);

	//StateKey
	wstrTextureName = PathFindFileName(szPath);

	wstring wstrFileTag = wstrTextureName.substr(7, wstrTextureName.length() );
	m_iMix3Num = _wtoi(wstrFileTag.c_str());

	//ObjKey
	m_Mix3Name = wstrTextureName.c_str();

	if(m_Terrain1.GetCheck() == 1)
	{
		m_strTerrain1TextureName[2] = wstrTextureName;
		((CTerrain*)m_pTerrain)->SetMix3num(m_iMix3Num);
		m_iTerrain1TextureNum[2] = m_iMix3Num;
	}
	else
	{
		m_strTerrain2TextureName[2] = wstrTextureName;
		((CTerrain2*)m_pSecondTerrain)->SetMix3num(m_iMix3Num);
		m_iTerrain2TextureNum[2] = m_iMix3Num;
	}
	
	UpdateData(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CMapPage::OnBnClickedMix4Load()
{
	UpdateData(TRUE);

	CFileDialog		Dlg(TRUE, L"dds", L"*.*", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.*", this);

	if(Dlg.DoModal() == IDCANCEL)
		return;

	///////////////////////////////////////////////////////////////////////////////
	TCHAR		szPath[MAX_PATH] = L"";
	wstring		wstrTextureName;
	wstring		wstrTempName = Dlg.GetFileTitle().operator LPCWSTR();


	lstrcpy(szPath, (LPWSTR)Dlg.GetPathName().operator LPCWSTR());
	PathRemoveFileSpec(szPath);

	//상대 경로
	PathCombine(szPath, szPath, (LPWSTR)wstrTempName.c_str());
	wstrTextureName = ConvertRelativePath(szPath);

	//StateKey
	wstrTextureName = PathFindFileName(szPath);

	wstring wstrFileTag = wstrTextureName.substr(7, wstrTextureName.length() );
	m_iMix4Num = _wtoi(wstrFileTag.c_str());

	//ObjKey
	m_Mix4Name = wstrTextureName.c_str();

	if(m_Terrain1.GetCheck() == 1)
	{
		m_strTerrain1TextureName[3] = wstrTextureName;
		((CTerrain*)m_pTerrain)->SetMix4num(m_iMix4Num);
		m_iTerrain1TextureNum[3] = m_iMix4Num;
	}
	else
	{
		m_strTerrain2TextureName[3] = wstrTextureName;
		((CTerrain2*)m_pSecondTerrain)->SetMix4num(m_iMix4Num);
		m_iTerrain2TextureNum[3] = m_iMix4Num;
	}
	UpdateData(FALSE);
}
void CMapPage::OnBnClickedDrawSplatting()
{
	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( m_ctrDrawSplatting.GetCheck() == 1 )
		m_isDrawingSplatting = true;
	else
		m_isDrawingSplatting = false;
	UpdateData(FALSE);

}

void CMapPage::OnBnClickedEraseSplatting()
{
	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( m_ctrEraseSplatting.GetCheck() == 1 )
		m_isDrawingSplatting = false;
	else
		m_isDrawingSplatting = true;
	UpdateData(FALSE);
}

_bool CMapPage::GetSplattingMode(void)			
{
	return m_isDrawingSplatting;
}

void CMapPage::OnBnClickedSplattingSave()	//스플래팅
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	if(m_Terrain1.GetCheck() == 1)
	{
	
		m_tTexInfo = ((CTerrain*)m_pTerrain)->GetTexInfo();

		TCHAR			szFullPath0[128] = L"";
		TCHAR*			pFileName0 = L"Terrain1TexturePixel.dat";
		TCHAR*			szPath0 = L"../../Data/";
		lstrcpy(szFullPath0, szPath0);
		lstrcat(szFullPath0, pFileName0);

		DWORD		dwByte0 = 0;

		HANDLE hFile0 = CreateFile(szFullPath0, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS
			, FILE_ATTRIBUTE_NORMAL, NULL);

		LPDIRECT3DTEXTURE9						pTexture;
		for(int iAlpahSize =  0 ; iAlpahSize < 4; ++iAlpahSize)				//일단 4개의 알파맵을 생각하자
		//알파맵 4개를 모두 저장하기 위함
		{
			pTexture = ((CTerrain*)m_pTerrain)->GetTexture(iAlpahSize);
			if(pTexture == NULL)
				return;

			D3DLOCKED_RECT Rect;
			pTexture->LockRect(0 , &Rect , NULL , 0);
			_vec4		vTemp;
			
			for( int z = 0 ; z < m_iVtx1Size ; ++z )
			{
				BYTE*  pByte = (BYTE*)Rect.pBits;	//BYTE == unsigned char
				pByte = pByte + z * Rect.Pitch;
				for( int x = 0 ; x < m_iVtx1Size ; ++x )
				{
					DWORD*  pPixel = (DWORD*)pByte + x;	//DWORD == unsigned long
					D3DXCOLOR dColor = ((D3DXCOLOR)*pPixel);
					vTemp = _vec4(dColor.r , dColor.g , dColor.b , dColor.a);
					WriteFile(hFile0, vTemp, sizeof(_vec4), &dwByte0, NULL);
				}
			}
			pTexture->UnlockRect( 0 );
		}
		CloseHandle(hFile0);

		TCHAR			szFullPath1[128] = L"";										//기본 텍스쳐 정보 세이브
		TCHAR*			pFileName1 = L"Terrain1TextureInfo.dat";
		TCHAR*			szPath1 = L"../../Data/";
		lstrcpy(szFullPath1, szPath1);
		lstrcat(szFullPath1, pFileName1);

		DWORD		dwByte1 = 0;

		HANDLE hFile1 = CreateFile(szFullPath1, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS
			, FILE_ATTRIBUTE_NORMAL, NULL);

		WriteFile(hFile1, &m_tTexInfo, sizeof(TEXINFO), &dwByte1, NULL);
		CloseHandle(hFile1);

	}
/////////////////////////////////////////////////////////////////////
	if(m_Terrain2.GetCheck() == 1)
	{

		m_tTexInfo = ((CTerrain2*)m_pSecondTerrain)->GetTexInfo();

		TCHAR			szFullPath3[128] = L"";
		TCHAR*			pFileName3 = L"Terrain2TexturePixel.dat";
		TCHAR*			szPath3 = L"../../Data/";
		lstrcpy(szFullPath3, szPath3);
		lstrcat(szFullPath3, pFileName3);

		DWORD		dwByte3 = 0;

		HANDLE hFile3 = CreateFile(szFullPath3, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS
			, FILE_ATTRIBUTE_NORMAL, NULL);

		LPDIRECT3DTEXTURE9						pTexture2;
		for(int iAlpahSize =  0 ; iAlpahSize < 4; ++iAlpahSize)				//일단 4개의 알파맵을 생각하자
			//알파맵 4개를 모두 저장하기 위함
		{
			pTexture2 = ((CTerrain2*)m_pSecondTerrain)->GetTexture(iAlpahSize);
			if(pTexture2 == NULL)
				return;

			D3DLOCKED_RECT Rect;
			pTexture2->LockRect(0 , &Rect , NULL , 0);
			_vec4		vTemp;

			for( int z = 0 ; z < m_iVtx2Size ; ++z )
			{
				BYTE*  pByte = (BYTE*)Rect.pBits;	//BYTE == unsigned char
				pByte = pByte + z * Rect.Pitch;
				for( int x = 0 ; x < m_iVtx2Size ; ++x )
				{
					/*_int iIndex = z * m_iVtx1Size + x;*/
					DWORD*  pPixel = (DWORD*)pByte + x;	//DWORD == unsigned long
					D3DXCOLOR dColor = ((D3DXCOLOR)*pPixel);
					vTemp = _vec4(dColor.r , dColor.g , dColor.b , dColor.a);
					WriteFile(hFile3, vTemp, sizeof(_vec4), &dwByte3, NULL);
				}
			}
			pTexture2->UnlockRect( 0 );
		}
		CloseHandle(hFile3);

		TCHAR			szFullPath4[128] = L"";
		TCHAR*			pFileName4 = L"Terrain2TextureInfo.dat";
		TCHAR*			szPath4 = L"../../Data/";
		lstrcpy(szFullPath4, szPath4);
		lstrcat(szFullPath4, pFileName4);

		DWORD		dwByte4 = 0;

		HANDLE hFile4 = CreateFile(szFullPath4, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS
			, FILE_ATTRIBUTE_NORMAL, NULL);

		WriteFile(hFile4, &m_tTexInfo, sizeof(TEXINFO), &dwByte4, NULL);
		CloseHandle(hFile4);

	}
 	UpdateData(FALSE);	
}

void CMapPage::OnBnClickedSplattingLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);	
	if(m_Terrain1.GetCheck() == 1)
	{
	
		TCHAR			szFullPath0[128] = L"";									///////////////퍼스트 터레인의 알파 로드
		TCHAR*			pFileName0 = L"Terrain1TexturePixel.dat";
		TCHAR*			szPath0 = L"../../Data/";
		lstrcpy(szFullPath0, szPath0);
		lstrcat(szFullPath0, pFileName0);

		HANDLE		hFile0 = CreateFile(szFullPath0 , GENERIC_READ, 0, NULL, OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL, NULL);
		DWORD		dwByte0 = 0;

		if(m_pTerrain != NULL)
			((CTerrain*)m_pTerrain)->CreateAlphaMap();

		LPDIRECT3DTEXTURE9						pTexture;
		_vec4									vTemp;
 		for(int iAlpahSize =  0 ; iAlpahSize < 4; ++iAlpahSize)				//일단 4개의 알파맵을 생각하자
 		{
 			pTexture = ((CTerrain*)m_pTerrain)->GetTexture(iAlpahSize);
	 
 			D3DLOCKED_RECT		Rect;								//RECT변수를 이용 하여 LockRect과 UnlockRect을  사용한다
 			pTexture->LockRect(0 , &Rect , NULL , 0);
	 
 			for(_int z = 0; z < m_iVtx1Size ; ++z)					//Bits는 첫번째 주소
 			{														//pitch는 가로폭의 바이트수
 				BYTE*	pByte = (BYTE*)Rect.pBits;					//pBits는 void*형이다 그러니까 사용할때 변환해서 사용하자
	 
 				pByte = pByte + z * Rect.Pitch;						//Pitch는 int형
	 
 				for(_int x = 0; x < m_iVtx1Size; ++x)
 				{
 					DWORD*		pPixel	= (DWORD*)pByte + x;
 					ReadFile(hFile0, vTemp, sizeof(_vec4), &dwByte0, NULL);
	 				
 					*pPixel = D3DXCOLOR(vTemp.x , vTemp.y , vTemp.z , vTemp.w);		//픽셀의 색을 0으로 초기화하자
 				}
 			}
			pTexture->UnlockRect(0 );
		}
		

		CloseHandle(hFile0);
		/////////////////////////새로운 방법 끝
		TCHAR			szFullPath1[128] = L"";									///////////////퍼스트 터레인의 알파 로드
		TCHAR*			pFileName1 = L"Terrain1TextureInfo.dat";
		TCHAR*			szPath1 = L"../../Data/";
		lstrcpy(szFullPath1, szPath1);
		lstrcat(szFullPath1, pFileName1);

		HANDLE		hFile1 = CreateFile(szFullPath1 , GENERIC_READ, 0, NULL, OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL, NULL);
		DWORD		dwByte1 = 0;

 		while( true )
 		{
 			TEXINFO			tTexInfo;
 			ReadFile(hFile1, &tTexInfo, sizeof(TEXINFO), &dwByte1, NULL);
	 
 			if(dwByte1 == 0)
 				break;
	 		
 			((CTerrain*)(m_pTerrain))->SetTexInfo(tTexInfo); 
			((CTerrain*)m_pTerrain)->SetMix1Ratio(tTexInfo.fAlpahUV[0]);
			((CTerrain*)m_pTerrain)->SetMix2Ratio(tTexInfo.fAlpahUV[1]);
			((CTerrain*)m_pTerrain)->SetMix3Ratio(tTexInfo.fAlpahUV[2]);
			((CTerrain*)m_pTerrain)->SetMix4Ratio(tTexInfo.fAlpahUV[3]);

 		}
		CloseHandle(hFile1);
	}
	//////////////////////////////////////터레인 2를 로드하자///////////////////////////////////////////////////////

	if(m_Terrain2.GetCheck() == 1)
	{

		TCHAR			szFullPath3[128] = L"";									///////////////세컨 터레인의 알파 로드
		TCHAR*			pFileName3 = L"Terrain2TexturePixel.dat";
		TCHAR*			szPath3 = L"../../Data/";
		lstrcpy(szFullPath3, szPath3);
		lstrcat(szFullPath3, pFileName3);

		HANDLE		hFile3 = CreateFile(szFullPath3 , GENERIC_READ, 0, NULL, OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL, NULL);
		DWORD		dwByte3 = 0;

		if(m_pSecondTerrain != NULL)
			((CTerrain2*)m_pSecondTerrain)->CreateAlphaMap();

		LPDIRECT3DTEXTURE9						pTexture2;
		_vec4									vTemp2;
		for(int iAlpahSize =  0 ; iAlpahSize < 4; ++iAlpahSize)				//일단 4개의 알파맵을 생각하자
		{
			pTexture2 = ((CTerrain2*)m_pSecondTerrain)->GetTexture(iAlpahSize);

			D3DLOCKED_RECT		Rect;								//RECT변수를 이용 하여 LockRect과 UnlockRect을  사용한다
			pTexture2->LockRect(0 , &Rect , NULL , 0);

			for(_int z = 0; z < m_iVtx2Size ; ++z)					//Bits는 첫번째 주소
			{														//pitch는 가로폭의 바이트수
				BYTE*	pByte = (BYTE*)Rect.pBits;					//pBits는 void*형이다 그러니까 사용할때 변환해서 사용하자

				pByte = pByte + z * Rect.Pitch;						//Pitch는 int형

				for(_int x = 0; x < m_iVtx2Size; ++x)
				{
					DWORD*		pPixel	= (DWORD*)pByte + x;
					ReadFile(hFile3, vTemp2, sizeof(_vec4), &dwByte3, NULL);

					*pPixel = D3DXCOLOR(vTemp2.x , vTemp2.y , vTemp2.z , vTemp2.w);		//픽셀의 색을 0으로 초기화하자
				}
			}
			pTexture2->UnlockRect(0 );
		}
		CloseHandle(hFile3);

		TCHAR			szFullPath4[128] = L"";									///////////////퍼스트 터레인의 알파 로드
		TCHAR*			pFileName4 = L"Terrain2TextureInfo.dat";
		TCHAR*			szPath4 = L"../../Data/";
		lstrcpy(szFullPath4, szPath4);
		lstrcat(szFullPath4, pFileName4);

		HANDLE		hFile4 = CreateFile(szFullPath4 , GENERIC_READ, 0, NULL, OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL, NULL);
		DWORD		dwByte4 = 0;

		while( true )
		{
			TEXINFO			tTexInfo;
			ReadFile(hFile4, &tTexInfo, sizeof(TEXINFO), &dwByte4, NULL);

			if(dwByte4 == 0)
				break;

			((CTerrain2*)(m_pSecondTerrain))->SetTexInfo(tTexInfo); 
			((CTerrain2*)m_pSecondTerrain)->SetMix1Ratio(tTexInfo.fAlpahUV[0]);
			((CTerrain2*)m_pSecondTerrain)->SetMix2Ratio(tTexInfo.fAlpahUV[1]);
			((CTerrain2*)m_pSecondTerrain)->SetMix3Ratio(tTexInfo.fAlpahUV[2]);
			((CTerrain2*)m_pSecondTerrain)->SetMix4Ratio(tTexInfo.fAlpahUV[3]);

		}
		CloseHandle(hFile4);
	}

	UpdateData(FALSE);	
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CMapPage::OnBnClickedMix1()			//Mix1 비율
{
	UpdateData(TRUE);
	m_iMixNum = 0;
	if(m_Terrain1.GetCheck() == 1)	//터레인 1
	{
		m_fMixRatio = m_fTerrain1MixRatio[0] ;
		m_ctrMixRatio.SetPos( (_int)(m_fTerrain1MixRatio[0] ) );
		((CTerrain*)m_pTerrain)->SetMix1num(m_iTerrain1TextureNum[0]);
		((CTerrain*)m_pTerrain)->SetMix1Ratio(m_fTerrain1MixRatio[0]);
	}
	else							//터레인 1
	{
		m_fMixRatio = m_fTerrain2MixRatio[0] ;
		m_ctrMixRatio.SetPos( (_int)(m_fTerrain2MixRatio[0] ) );
		((CTerrain2*)m_pSecondTerrain)->SetMix1num(m_iTerrain2TextureNum[0]);
		((CTerrain2*)m_pSecondTerrain)->SetMix1Ratio(m_fTerrain2MixRatio[0]);
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CMapPage::OnBnClickedMix2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_iMixNum = 1;
	if(m_Terrain1.GetCheck() == 1)	//터레인 1
	{
		m_fMixRatio = m_fTerrain1MixRatio[1] ;
		m_ctrMixRatio.SetPos( (_int)(m_fTerrain1MixRatio[1] ) );
		((CTerrain*)m_pTerrain)->SetMix2num(m_iTerrain1TextureNum[1]);
		((CTerrain*)m_pTerrain)->SetMix2Ratio(m_fTerrain1MixRatio[1]);
	}
	else							//터레인 1
	{
		m_fMixRatio = m_fTerrain2MixRatio[1] ;
		m_ctrMixRatio.SetPos( (_int)(m_fTerrain2MixRatio[1] ) );
		((CTerrain2*)m_pSecondTerrain)->SetMix2num(m_iTerrain2TextureNum[1]);
		((CTerrain2*)m_pSecondTerrain)->SetMix2Ratio(m_fTerrain2MixRatio[1]);
	}

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CMapPage::OnBnClickedMix3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_iMixNum = 2;
	if(m_Terrain1.GetCheck() == 1)	//터레인 1
	{
		m_fMixRatio = m_fTerrain1MixRatio[2] ;
		m_ctrMixRatio.SetPos( (_int)(m_fTerrain1MixRatio[2] ) );
		((CTerrain*)m_pTerrain)->SetMix3num(m_iTerrain1TextureNum[2]);
		((CTerrain*)m_pTerrain)->SetMix3Ratio(m_fTerrain1MixRatio[2]);
	}
	else							//터레인 1
	{
		m_fMixRatio = m_fTerrain2MixRatio[2] ;
		m_ctrMixRatio.SetPos( (_int)(m_fTerrain2MixRatio[2] ) );
		((CTerrain2*)m_pSecondTerrain)->SetMix3num(m_iTerrain2TextureNum[2]);
		((CTerrain2*)m_pSecondTerrain)->SetMix3Ratio(m_fTerrain2MixRatio[2]);
	}
	UpdateData(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CMapPage::OnBnClickedMix4()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_iMixNum = 3;
	if(m_Terrain1.GetCheck() == 1)	//터레인 1
	{
		m_fMixRatio = m_fTerrain1MixRatio[3] ;
		m_ctrMixRatio.SetPos( (_int)(m_fTerrain1MixRatio[3] ) );
		((CTerrain*)m_pTerrain)->SetMix4num(m_iTerrain1TextureNum[3]);
		((CTerrain*)m_pTerrain)->SetMix4Ratio(m_fTerrain1MixRatio[3]);
	}
	else							//터레인 1
	{
		m_fMixRatio = m_fTerrain2MixRatio[3] ;
		m_ctrMixRatio.SetPos( (_int)(m_fTerrain2MixRatio[3] ) );
		((CTerrain2*)m_pSecondTerrain)->SetMix4num(m_iTerrain2TextureNum[3]);
		((CTerrain2*)m_pSecondTerrain)->SetMix4Ratio(m_fTerrain2MixRatio[3]);
	}
	UpdateData(FALSE);
}
void CMapPage::OnNMCustomdrawMixRatio(NMHDR *pNMHDR, LRESULT *pResult)
{
	UpdateData(TRUE);
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_fMixRatio = (_float)m_ctrMixRatio.GetPos() / m_fsliderRatio;

	*pResult = 0;
	UpdateData(FALSE);
}

void CMapPage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	UpdateData(TRUE);
	if( m_Terrain1.GetCheck() == 1)
	{
		if(m_ctrMix1.GetCheck() == 1)
		{
			m_fTerrain1MixRatio[0] = (_float)m_ctrMixRatio.GetPos() / m_fsliderRatio ;
			m_fMixRatio = (_float)m_ctrMixRatio.GetPos() / m_fsliderRatio ;
			((CTerrain*)m_pTerrain)->SetMix1Ratio(m_fTerrain1MixRatio[0]);
		}
		else if(m_ctrMix2.GetCheck() == 1)	//Mix 2
		{
			m_fTerrain1MixRatio[1] = (_float)m_ctrMixRatio.GetPos() / m_fsliderRatio;
			m_fMixRatio = (_float)m_ctrMixRatio.GetPos() / m_fsliderRatio ;
			((CTerrain*)m_pTerrain)->SetMix2Ratio(m_fTerrain1MixRatio[1]);
		}
		else if(m_ctrMix3.GetCheck() == 1)	//Mix 3
		{
			m_fTerrain1MixRatio[2] = (_float)m_ctrMixRatio.GetPos() / m_fsliderRatio;
			m_fMixRatio = (_float)m_ctrMixRatio.GetPos() / m_fsliderRatio ;
			((CTerrain*)m_pTerrain)->SetMix3Ratio(m_fTerrain1MixRatio[2]);
		}
		else if(m_ctrMix4.GetCheck() == 1)	//Mix 4
		{
			m_fTerrain1MixRatio[3] = (_float)m_ctrMixRatio.GetPos() / m_fsliderRatio;
			m_fMixRatio = (_float)m_ctrMixRatio.GetPos() / m_fsliderRatio ;
			((CTerrain*)m_pTerrain)->SetMix4Ratio(m_fTerrain1MixRatio[3]);
		}
	}
	else
	{
		if(m_ctrMix1.GetCheck() == 1)
		{
			m_fTerrain2MixRatio[0] = (_float)m_ctrMixRatio.GetPos() / m_fsliderRatio;
			m_fMixRatio = (_float)m_ctrMixRatio.GetPos() / m_fsliderRatio ;
			((CTerrain2*)m_pSecondTerrain)->SetMix1Ratio(m_fTerrain2MixRatio[0]);
		}
		else if(m_ctrMix2.GetCheck() == 1)
		{
			m_fTerrain2MixRatio[1] = (_float)m_ctrMixRatio.GetPos() / m_fsliderRatio;
			m_fMixRatio = (_float)m_ctrMixRatio.GetPos() / m_fsliderRatio ;
			((CTerrain2*)m_pSecondTerrain)->SetMix2Ratio(m_fTerrain2MixRatio[1]);
		}
		else if(m_ctrMix3.GetCheck() == 1)
		{
			m_fTerrain2MixRatio[2] = (_float)m_ctrMixRatio.GetPos() / m_fsliderRatio;
			m_fMixRatio = (_float)m_ctrMixRatio.GetPos() / m_fsliderRatio ;
			((CTerrain2*)m_pSecondTerrain)->SetMix3Ratio(m_fTerrain2MixRatio[2]);
		}
		else if(m_ctrMix4.GetCheck() == 1)
		{
			m_fTerrain2MixRatio[3] = (_float)m_ctrMixRatio.GetPos() / m_fsliderRatio;
			m_fMixRatio = (_float)m_ctrMixRatio.GetPos() / m_fsliderRatio ;
			((CTerrain2*)m_pSecondTerrain)->SetMix4Ratio(m_fTerrain2MixRatio[3]);
		}
	}
	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
	UpdateData(FALSE);
}

_vec3 CMapPage::GetPlayerPos(void)
{
	if(m_pPlayer != NULL)
		return ((CPlayer*)m_pPlayer)->GetPlayerPos();

	return _vec3(0.f , 0.f ,0.f);
}
void CMapPage::OnBnClickedEditTerrainEdit()
{
	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(m_ctrTerrainEDit.GetCheck() == 1)
		m_isEditTerrain = true;			//Edit를 누르면 true가 되고
	else
		m_isEditTerrain = false;
	UpdateData(FALSE);
}

_bool CMapPage::GetEditTerrain(void)
{
	return m_isEditTerrain;
}
