
// ToolView.cpp : CToolView Ŭ������ ����
//

#include "stdafx.h"
#include "Tool.h"
#include "ToolDoc.h"
#include "ToolView.h"
#include "Terrain.h"
#include "Terrain2.h"
#include "MainFrm.h"
#include "Sheet.h"
#include "MyForm.h"
#include "Mouse.h"
#include "Tool_Defines.h"
#include "Export_Function.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CToolView
HWND			g_hWnd;
HWND			g_SecondhWnd;
HINSTANCE		g_hInstance;

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// ǥ�� �μ� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CToolView::OnFilePrintPreview)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CToolView ����/�Ҹ�

CToolView::CToolView()
: m_iPage(0)
, m_iCount(0)
, m_iRange(0)
, m_iHeight(0)
, m_iVtx1Size(0)
, m_iNum(0)
, m_fTime(0.f)
, m_iMixNum(0)
, m_iLoadTextureNum(50)		//���� 46
, m_vPos(0.f , 0.f , 0.f)
, m_vPlayerPos(0.f , 0.f , 0.f)
, m_isMove(false)
, m_isPass(false)
, m_isSplatting(false)
, m_isSplattingMode(false)
, m_isTerrainEdit(false)
, m_isInputHome(false)
, m_pTerrain(NULL)
, m_pSecondTerrain(NULL)
, m_pSheet(NULL)
{
	ZeroMemory(szFps , sizeof(TCHAR) * 128);
	ZeroMemory(szTerrainNum , sizeof(TCHAR) * 128);
	ZeroMemory(szEditMode , sizeof(TCHAR) * 128);
	ZeroMemory(szPlayerPos , sizeof(TCHAR) * 128);
	D3DXMatrixIdentity(&m_MatWorld);

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
}

CToolView::~CToolView()
{
	Engine::Utility_Release();
	Engine::Resource_Release();
	Engine::System_Release();
	if(m_pTerrain != NULL)
		((CTerrain*)m_pTerrain)->Release();
	if(m_pSecondTerrain != NULL)
		((CTerrain*)m_pSecondTerrain)->Release();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs�� �����Ͽ� ���⿡��
	//  Window Ŭ���� �Ǵ� ��Ÿ���� �����մϴ�.

	return CView::PreCreateWindow(cs);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

// CToolView �׸���

void CToolView::OnDraw(CDC* /*pDC*/)
{
	if(false == m_isPass)		//������ ���̴� �� �����Ѵ�
		return;

	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	Update();
	Render();
	// TODO: ���⿡ ���� �����Ϳ� ���� �׸��� �ڵ带 �߰��մϴ�.
	Invalidate(FALSE);
}

// CToolView �μ�

//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CToolView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// �⺻���� �غ�
	return DoPreparePrinting(pInfo);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ��ϱ� ���� �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: �μ� �� ���� �۾��� �߰��մϴ�.
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CToolView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CToolView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

// CToolView ����

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

CToolDoc* CToolView::GetDocument() const // ����׵��� ���� ������ �ζ������� �����˴ϴ�.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView �޽��� ó����
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	m_isPass = true;
	UpdateData(TRUE);
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	m_pMainFrm = (CMainFrame*)AfxGetMainWnd();




	/*�������� â�� ������ ũ�⸦ �����Ѵ�*/
	RECT		rcWnd;
	m_pMainFrm->GetWindowRect(&rcWnd);

	SetRect(&rcWnd, 0, 0,
		rcWnd.right - rcWnd.left, 
		rcWnd.bottom - rcWnd.top);

	RECT	rcMainView;
	GetClientRect(&rcMainView);

	float fRowFrm = float(rcWnd.right - rcMainView.right);
	float fColFrm = float(rcWnd.bottom - rcMainView.bottom);

	m_pMainFrm->SetWindowPos(NULL, 200, 150,
		int(WINSIZEX + fRowFrm), int(WINSIZEY + fColFrm),	SWP_NOZORDER);

	/*�ڵ��� �ΰ� ����Ͽ� DInput�� ����ϱ� ���ؼ� ����Ѵ�*/
	g_hWnd = m_hWnd;
	g_hInstance = AfxGetInstanceHandle();

	CWnd*		pWnd = AfxGetMainWnd();
	g_SecondhWnd = pWnd->m_hWnd;

	/*���� �Լ����� �ʱ�ȭ �Ͽ� ����Ѵ�*/

	if(FAILED(Engine::Init_InputDevice(g_hInstance, g_SecondhWnd)))
		return;

	Engine::Reserve_Container(RESOURCE_END);

	if(FAILED(Engine::Init_GraphicDevice(Engine::CGraphic_Device::MODE_WIN , g_hWnd , WINSIZEX , WINSIZEY)))
		return ;
	_matrix matProj;

	D3DXMatrixIdentity(&matProj);
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(45.f), WINSIZEX/float(WINSIZEY), 0.1f ,3000.f);
	Engine::GetGraphicDev()->SetTransform(D3DTS_PROJECTION, &matProj);

	if(FAILED(Engine::Init_Management(Engine::GetGraphicDev(), WINSIZEX, WINSIZEY)))
		return ;

	Engine::Ready_Fonts(Engine::GetGraphicDev(), L"Font_Batang", L"����", 10, 20, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	Engine::Ready_Buffers(Engine::GetGraphicDev(), RESOURCE_STATIC, Engine::CVIBuffer::TYPE_RCTEX, L"RcTex");
	Engine::Ready_Buffers(Engine::GetGraphicDev(), RESOURCE_STATIC, Engine::CVIBuffer::TYPE_TRICOL, L"TriCol");
	Engine::Ready_Buffers(Engine::GetGraphicDev() ,RESOURCE_STATIC , Engine::CVIBuffer::TYPE_CUBE ,L"CubeTex");
	Engine::Ready_Textures(Engine::GetGraphicDev(), RESOURCE_STAGE, Engine::CTexture::TYPE_NORMAL, L"Texture_Blue", L"../../Resource/Texture/Stage/Picking/", L"PickingBox.png");
	Engine::Ready_Textures(Engine::GetGraphicDev(), RESOURCE_STAGE, Engine::CTexture::TYPE_NORMAL, L"Texture_Flag", L"../../Resource/Texture/Stage/Picking/", L"Flag%d.png" , 2);
	Engine::Ready_Textures(Engine::GetGraphicDev(), RESOURCE_STAGE, Engine::CTexture::TYPE_NORMAL, L"Texture_Water", L"../../Resource/Texture/Stage/Water/", L"Water%d.jpg",3);
	Engine::Ready_Mesh(Engine::GetGraphicDev(), RESOURCE_STAGE, Engine::CMesh::TYPE_COLLIDER, L"Mesh_Box", NULL, NULL);
	Engine::Ready_Mesh(Engine::GetGraphicDev(), RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Player", L"../../Resource/Texture/Stage/Player/", L"Player.x");
	Engine::Ready_Mesh(Engine::GetGraphicDev(), RESOURCE_STAGE, Engine::CMesh::TYPE_STATIC, L"Enemy", L"../../Resource/Texture/Stage/Player/", L"Player.x");
	//�ͷ���dds -> ��ü�˻����� �ͷ���dds�� �˻��Ѵ� �� 3���� �ɷ��ִ�(Toolview �� MapPage���� Load�Ҷ�(Terrain1 , Terrain2))
	Engine::Ready_Textures(Engine::GetGraphicDev() , RESOURCE_STAGE , Engine::CTexture::TYPE_NORMAL, L"Tool_Texture_Terrain" , L"../../Resource/Texture/Stage/Terrain/", L"Terrain%d.dds" , m_iLoadTextureNum);
	Engine::Ready_Mesh(Engine::GetGraphicDev() , RESOURCE_STAGE , Engine::CMesh::TYPE_STATIC ,L"SkyBox" , L"../../Resource/Texture/Stage/SkyBox/" , L"SkyBox0.x");
	Engine::Ready_Mesh(Engine::GetGraphicDev() , RESOURCE_STAGE , Engine::CMesh::TYPE_STATIC ,L"SkyBox" , L"../../Resource/Texture/Stage/SkyBox/" , L"SkyBox1.x");
	Engine::Ready_Mesh(Engine::GetGraphicDev() , RESOURCE_STAGE , Engine::CMesh::TYPE_STATIC ,L"SkyBox" , L"../../Resource/Texture/Stage/SkyBox/" , L"SkyBox2.x");
	Engine::Ready_Shader(Engine::GetGraphicDev(), L"Shader_Terrain", L"../Bin/EffectFiles/Shader_Terrain.hpp");
	Engine::Ready_Shader(Engine::GetGraphicDev(), L"Shader_SkyBox", L"../Bin/EffectFiles/Shader_Sky.hpp");
	Engine::Ready_Shader(Engine::GetGraphicDev(), L"Shader_Default", L"../Bin/EffectFiles/Shader_Default.hpp");
	
	m_pSheet = m_pMainFrm->m_pMyForm->m_pSheet;
	Engine::InitTime();

	Engine::Init_Frustum(Engine::GetGraphicDev());
	UpdateData(FALSE);
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CToolView::Render(void)
{
 	Engine::GetGraphicDev()->Clear(0 , NULL , D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL ,D3DCOLOR_ARGB(255 , 163 , 204 , 163) , 1.f , 0);
 	Engine::GetGraphicDev()->BeginScene();

	MAPLAYER::iterator iter = m_mapLayer.begin();
	MAPLAYER::iterator iter_end = m_mapLayer.end();

	Engine::Render();
	
	if(m_pSheet->m_NaviMeshPage.GetNaviRender() == true)
		Engine::NaviMesh_Render();		//�׺�޽� �׸���

	for(; iter != iter_end; ++iter)
	{
		(iter->second)->Render();
	}

	++m_iCount;	

	m_fTime += Engine::GetTime();
	m_vPlayerPos = m_pSheet->m_MapPage.GetPlayerPos();

	if(m_fTime >= 1.f)
	{
		wsprintf(szFps, L"FPS : %d", m_iCount);
		m_fTime = 0.f;
		m_iCount = 0;
	}

	Engine::Render_Font(Engine::GetGraphicDev(), L"Font_Batang", szFps, &m_MatWorld );
	wsprintf(szTerrainNum , L"\n���õ� Terrain : %d", m_iNum + 1);
	Engine::Render_Font(Engine::GetGraphicDev(), L"Font_Batang", szTerrainNum, &m_MatWorld );


	swprintf_s(szPlayerPos , L"\n\n\nPlayerPos : %f , %f , %f", m_vPlayerPos.x , m_vPlayerPos.y , m_vPlayerPos.z);
	Engine::Render_Font(Engine::GetGraphicDev(), L"Font_Batang", szPlayerPos, &m_MatWorld );

	if(m_isTestMode == false)
	{
		wsprintf(szEditMode , L"\n\n���� ��� : EditMode");
		Engine::Render_Font(Engine::GetGraphicDev(), L"Font_Batang", szEditMode, &m_MatWorld );
	}
	else
	{	
		wsprintf(szEditMode , L"\n\n���� ��� : TestMode");
		Engine::Render_Font(Engine::GetGraphicDev(), L"Font_Batang", szEditMode, &m_MatWorld );
	}

 	Engine::GetGraphicDev()->EndScene();
 	Engine::GetGraphicDev()->Present(NULL , NULL , NULL , NULL);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------
void CToolView::SetTerrain(Engine::CGameObject* pTerrain)						//�ͷ��� ���鶧 �ּ� �޾ƿ���
{
	m_pTerrain = pTerrain;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void CToolView::SetSecondTerrain(Engine::CGameObject* pTerrain)					//�ͷ��� ���鶧 �ּ� �޾ƿ���
{
	m_pSecondTerrain = pTerrain;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CToolView::Update(void)
{
	Engine::SetRenderState(D3DRS_LIGHTING , FALSE);
	Engine::SetTime();
	Engine::Receive_Device_State();

	Engine::ViewToWorld(Engine::GetGraphicDev() ,false);

	CheckMove();
	SelectPage();
	if(m_isTestMode != true)
		TerrainKeyCheck();

	if(m_iPage == NAVIPAGE)
	{
		m_pSheet->m_NaviMeshPage.SetRender(true);
		NaviKeyCheck();
	}
	else
		m_pSheet->m_NaviMeshPage.SetRender(false);

	m_iRange = m_pSheet->m_MapPage.GetRange();
	m_iHeight = m_pSheet->m_MapPage.GetHeight();
	m_iVtx1Size = m_pSheet->m_MapPage.GetVtx1Size();
	m_iVtx2Size = m_pSheet->m_MapPage.GetVtx2Size();
	m_iEditMode = m_pSheet->m_MapPage.GetMode();
	m_isTestMode = m_pSheet->m_MapPage.GetTest();
	m_isSplatting = m_pSheet->m_MapPage.GetSplatting();
	m_iMixNum = m_pSheet->m_MapPage.GetMixNum();
	m_isSplattingMode = m_pSheet->m_MapPage.GetSplattingMode();
	m_isTerrainEdit = m_pSheet->m_MapPage.GetEditTerrain();

	if(BST_CHECKED == m_pSheet->m_MapPage.m_ctrTestMode.GetCheck())
	{
		if(NULL != m_pSheet->m_MapPage.m_pStaticCamera)
			m_pSheet->m_MapPage.m_pStaticCamera->Update();
	}

	if(BST_UNCHECKED == m_pSheet->m_MapPage.m_ctrTestMode.GetCheck())
	{
		if(NULL != m_pSheet->m_MapPage.m_pDynamicCamera)
			m_pSheet->m_MapPage.m_pDynamicCamera->Update();
	}

	MAPLAYER::iterator iter = m_mapLayer.begin();
	MAPLAYER::iterator iter_end = m_mapLayer.end();

	for(; iter != iter_end; ++iter)
		(iter->second)->Update();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CToolView::SelectPage(void)													//� Page�� �����ߴ���
{
	m_iPage = m_pSheet->GetActiveIndex();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)							//���콺�� �ǽð����� ������ �ʴ´� Ŭ���Ҷ��� ����
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	KeyCheck();
	CView::OnLButtonDown(nFlags, point);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CToolView::KeyCheck(void)														//Key�� üũ�Ͽ� �������� � �۾��� �Ұ����� �Ǵ�
{
	switch(m_iPage)
	{
	case OBJECTPAGE:
		if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			m_pSheet->m_ObjectPage.GetType();

		break;

	case NAVIPAGE:
		if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			m_pSheet->m_NaviMeshPage.AddMode();

		break;
	}

	RedrawWindow(NULL , NULL , RDW_INVALIDATE | RDW_UPDATENOW);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CToolView::NaviKeyCheck(void)													//Key�� üũ�Ͽ� �������� � �۾��� �Ұ����� �Ǵ�
{
	if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_pSheet->m_NaviMeshPage.ModifyMode();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void CToolView::TerrainKeyCheck(void)												//Key�� üũ�Ͽ� �������� � �۾��� �Ұ����� �Ǵ�
{
	if(GetAsyncKeyState(VK_F1))
	{
		m_pSheet->m_MapPage.SetRadio(1);
		if(m_pTerrain == NULL)
			return;
		Engine::VTXTEX* pTerrainVtx = ((CTerrain*)m_pTerrain)->GetVtxTex();
		CMouse::GetInstance()->SetVtx(((CTerrain*)m_pTerrain)->GetVtxSizeX() , ((CTerrain*)m_pTerrain)->GetVtxSizeZ() , (_int)((CTerrain*)m_pTerrain)->GetVtxItv());
		CMouse::GetInstance()->SetVtxTex(pTerrainVtx);
	}
	if(GetAsyncKeyState(VK_F2))
	{
		m_pSheet->m_MapPage.SetRadio(2);
		if(m_pSecondTerrain == NULL)
			return;
		Engine::VTXTEX* pTerrainVtx = ((CTerrain2*)m_pSecondTerrain)->GetVtxTex();
		CMouse::GetInstance()->SetVtx(((CTerrain2*)m_pSecondTerrain)->GetVtxSizeX() , ((CTerrain2*)m_pSecondTerrain)->GetVtxSizeZ() , (_int)((CTerrain2*)m_pSecondTerrain)->GetVtxItv());
		CMouse::GetInstance()->SetVtxTex(pTerrainVtx);
	}

	if(m_isTerrainEdit == false)
		return;

	if(m_iPage == 0)
	{
		if(m_isSplatting == false)
		{
			if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
				SetMapHeight();
		}
		else
		{
			if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
				Splatting();
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void CToolView::Splatting(void)
{
	///���÷���
	_int iNum = 0;
	iNum = m_pSheet->m_MapPage.GetNum();
	if(m_isSplatting == false)
		return;
	if(iNum == 0 )			//Terrain 1 ������ư ��������
	{
		if(m_pTerrain == NULL)
			return;
		Engine::VTXTEX* pTerrainVtx = ((CTerrain*)m_pTerrain)->GetVtxTex();
		_vec3 vPickPos = _vec3(0.f , 0.f , 0.f);
		CMouse::GetInstance()->SetVtx(((CTerrain*)m_pTerrain)->GetVtxSizeX() , ((CTerrain*)m_pTerrain)->GetVtxSizeZ() , (_int)((CTerrain*)m_pTerrain)->GetVtxItv());
		CMouse::GetInstance()->SetVtxTex(pTerrainVtx);
		
		_int iPickIndex = CMouse::GetInstance()->PickTerrain(&vPickPos );
		if(iPickIndex < 0)
			return ;

		float fx =  (vPickPos.x/* / 10*/);    //� ��ŷ�� �������
		float fz =  (vPickPos.z/* / 10*/);
		int startX = (int)(fx - m_iRange);     //������ �����Ұ��ΰ�
		int startZ = (int)(fz - m_iRange);
		int endX = (int)(fx + m_iRange);     //������ �����Ұ��ΰ�
		int endZ = (int)(fz + m_iRange);
		if( startX < 0 ) { startX = 0; }     //���� endX ������ ���� �ؾ� ������ �𸣰ڴ� ..
		if( startZ < 0 ) { startZ = 0; }
		if( endX >= m_iVtx1Size ) { endX = m_iVtx1Size - 1 ; } // 0���� �����ϱ⶧���� ���� -1  //
		if( endZ >= m_iVtx1Size ) { endZ = m_iVtx1Size - 1 ; } // 0���� �����ϱ⶧���� ���� -1
		_vec3 vPos = _vec3( 0.0f , 0.0f , 0.0f );

		m_pTexture = ((CTerrain*)m_pTerrain)->GetTexture(m_iMixNum);  // Index�� �ش��ϴ� Texture�� �������� �; ����
		if(m_pTexture == NULL)
			return;

		D3DLOCKED_RECT Rect;
		m_pTexture->LockRect(0 , &Rect , NULL , 0);
		for( int z = 0 ; z <= endZ ; ++z )
		{
			BYTE*  pByte = (BYTE*)Rect.pBits; 
			pByte = pByte + z * Rect.Pitch;
			for( int x = 0 ; x <= endX ; ++x )
			{
				_int iIndex = z * m_iVtx1Size + x;
				vPos = pTerrainVtx[iIndex].vPosition /*/ (float)10*/; //���͹��� �����ش�
				DWORD*  pPixel = (DWORD*)pByte + x;  
				// �귯�� ũ�⸸ŭ�� ��ġ���� ���콺 ��ġ������ �Ÿ��� ���Ѵ�.
				D3DXVECTOR3 Pix = vPos - ( D3DXVECTOR3( vPickPos.x , vPickPos.y , vPickPos.z ) );
				float Len = D3DXVec3Length( &Pix ) ;
				D3DXCOLOR dColor = ((D3DXCOLOR)*pPixel);
				
				if( Len < m_iRange )
				{
					if(m_isSplattingMode == true)  //ĥ�ϱ�   //Range ������� �۰�   1111���� ä���� ĥ�ϴ� ��
						dColor += ( D3DXCOLOR(0.5f * Engine::GetTime() , 0.5f * Engine::GetTime(), 0.5f * Engine::GetTime(), 0.5f * Engine::GetTime()) );
					else        //�����
						dColor -= ( D3DXCOLOR(0.5f * Engine::GetTime() , 0.5f * Engine::GetTime(), 0.5f * Engine::GetTime(), 0.5f * Engine::GetTime())  );
				}
				if(dColor.r >= 1.f)
					dColor = D3DXCOLOR(1.f , 1.f , 1.f , 1.f);
				else if(dColor.r <= 0.f)
					dColor = D3DXCOLOR(0.f , 0.f , 0.f , 0.f);
				*pPixel = dColor;
			}
		}
		m_pTexture->UnlockRect( 0 );
	}
	else if(iNum == 1)
	{
		if(m_pSecondTerrain == NULL)
			return;
		Engine::VTXTEX* pTerrainVtx = ((CTerrain2*)m_pSecondTerrain)->GetVtxTex();

		_vec3 vPickPos = _vec3(0.f , 0.f , 0.f);
		_int iPickIndex = CMouse::GetInstance()->PickTerrain(&vPickPos );
		CMouse::GetInstance()->SetVtx(((CTerrain2*)m_pSecondTerrain)->GetVtxSizeX() , ((CTerrain2*)m_pSecondTerrain)->GetVtxSizeZ() , (_int)((CTerrain2*)m_pSecondTerrain)->GetVtxItv());
		CMouse::GetInstance()->SetVtxTex(pTerrainVtx);

		if(iPickIndex < 0)
			return ;

		float fx =  (vPickPos.x/* / 10*/);
		float fz =  (vPickPos.z/* / 10*/);
		int startX = (int)(fx - m_iRange);
		int startZ = (int)(fz - m_iRange);
		int endX = (int)(fx + m_iRange);
		int endZ = (int)(fz + m_iRange);
		if( startX < 0 ) { startX = 0; }
		if( startZ < 0 ) { startZ = 0; }
		if( endX >= m_iVtx2Size ) { endX = m_iVtx2Size - 1 ; } // 0���� �����ϱ⶧���� ���� -1
		if( endZ >= m_iVtx2Size ) { endZ = m_iVtx2Size - 1 ; } // 0���� �����ϱ⶧���� ���� -1
		_vec3 vPos = _vec3( 0.0f , 0.0f , 0.0f );

		m_pTexture = ((CTerrain2*)m_pSecondTerrain)->GetTexture(m_iMixNum);  // Index�� �ش��ϴ� Texture�� �������� �; ����
		if(m_pTexture == NULL)
			return;

		D3DLOCKED_RECT Rect;
		m_pTexture->LockRect(0 , &Rect , NULL , 0);
		for( int z = 0 ; z <= endZ ; ++z )
		{
			BYTE*  pByte = (BYTE*)Rect.pBits; 
			pByte = pByte + z * Rect.Pitch;
			for( int x = 0 ; x <= endX ; ++x )
			{
				_int iIndex = z * m_iVtx2Size + x;
				vPos = pTerrainVtx[iIndex].vPosition /*/ (float)10*/; //���͹��� �����ش�
				DWORD*  pPixel = (DWORD*)pByte + x;  
				// �귯�� ũ�⸸ŭ�� ��ġ���� ���콺 ��ġ������ �Ÿ��� ���Ѵ�.
				D3DXVECTOR3 Pix = vPos - ( D3DXVECTOR3( vPickPos.x , vPickPos.y  , vPickPos.z ) );
				float Len = D3DXVec3Length( &Pix ) ;
				D3DXCOLOR dColor = ((D3DXCOLOR)*pPixel);
				if( Len < m_iRange )
				{
					if(m_isSplattingMode == true)  //ĥ�ϱ�   //Range ������� �۰�   1111���� ä���� ĥ�ϴ� ��
						dColor += ( D3DXCOLOR(0.5f* Engine::GetTime() , 0.5f * Engine::GetTime(), 0.5f * Engine::GetTime(), 0.5f* Engine::GetTime())  );
					else        //�����
						dColor -= ( D3DXCOLOR(0.5f * Engine::GetTime(), 0.5f * Engine::GetTime(), 0.5f * Engine::GetTime(), 0.5f* Engine::GetTime())  );
				}
				if(dColor.r >= 1.f)
					dColor = D3DXCOLOR(1.f , 1.f , 1.f , 1.f);
				else if(dColor.r <= 0.f)
					dColor = D3DXCOLOR(0.f , 0.f , 0.f , 0.f);
				*pPixel = dColor;
			}
		}
		m_pTexture->UnlockRect( 0 );
	}
} 
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CToolView::SetMapHeight(void)													//���̸� ���� �Լ�
{ 
	_int iNum = 0;
	iNum = m_pSheet->m_MapPage.GetNum();

	if(iNum == 0 )
	{
		if(m_pTerrain == NULL)
			return;
		Engine::VTXTEX* pTerrainVtx = ((CTerrain*)m_pTerrain)->GetVtxTex();

		_vec3	vPickPos = _vec3(0.f , 0.f , 0.f);
		CMouse::GetInstance()->SetVtx(((CTerrain*)m_pTerrain)->GetVtxSizeX() , ((CTerrain*)m_pTerrain)->GetVtxSizeZ() , (_int)((CTerrain*)m_pTerrain)->GetVtxItv());
		CMouse::GetInstance()->SetVtxTex(pTerrainVtx);
		_int	iPickIndex = CMouse::GetInstance()->PickTerrain(&vPickPos );

		if(iPickIndex < 0)
			return ;

		float fx	=  (vPickPos.x/* / 10*/);				//� ��ŷ�� �������
		float fz	=  (vPickPos.z/* / 10*/);

		int startX	= (int)(fx - m_iRange);					//������ �����Ұ��ΰ�
		int startZ	= (int)(fz - m_iRange);
		int endX	= (int)(fx + m_iRange);					//������ �����Ұ��ΰ�
		int endZ	= (int)(fz + m_iRange);

		if( startX < 0 ) { startX = 0; }					//���� endX ������ ���� �ؾ� ������ �𸣰ڴ� ..
		if( startZ < 0 ) { startZ = 0; }
		if( endX >= m_iVtx1Size ) { endX = m_iVtx1Size - 1 ; }	// 0���� �����ϱ⶧���� ���� -1		//
		if( endZ >= m_iVtx1Size ) { endZ = m_iVtx1Size - 1 ; }	// 0���� �����ϱ⶧���� ���� -1

		_vec3 vPos = _vec3( 0.0f , 0.0f , 0.0f );

		for( int z = 0 ; z <= endZ ; ++z )
		{
			for( int x = 0 ; x <= endX ; ++x )
			{
				_int iIndex = z * m_iVtx1Size + x;

				vPos = pTerrainVtx[iIndex].vPosition /*/ (float)10*/; //���͹��� �����ش�
				vPos.y = 0.0f;
				// �귯�� ũ�⸸ŭ�� ��ġ���� ���콺 ��ġ������ �Ÿ��� ���Ѵ�.
				D3DXVECTOR3 Pix = vPos - (D3DXVECTOR3( vPickPos.x , 0.0f , vPickPos.z ) /*/ (float)10*/);
				float Len = D3DXVec3Length( &Pix );

				float fy = -1.0f / (float)m_iRange * ( Len * Len ) + (float)m_iRange;

				switch(m_iEditMode)
				{
				case EDITUPDOWN:
					{
// 						if(m_iHeight == 0)
// 							pTerrainVtx[iIndex].vPosition.y = 0.f;

						if( Len < m_iRange )
							pTerrainVtx[iIndex].vPosition.y += ( fy / 10.0f ) * (m_iHeight / 100.f);
					}
					break;

				case EDITCENTER:
					{
 						if(Len <= m_iRange)
 						{
 							if( abs(pTerrainVtx[iIndex].vPosition.y - vPickPos.y) > 0.001f )
 							{
 								if(pTerrainVtx[iIndex].vPosition.y < vPickPos.y)
 								{
 									if(pTerrainVtx[iIndex].vPosition.y + ( ( fy / 10.f) * m_iHeight ) < vPickPos.y)
 										pTerrainVtx[iIndex].vPosition.y += ( fy / 10.f) * m_iHeight;
 								}
 							}
 							else if( pTerrainVtx[iIndex].vPosition.y > vPickPos.y )
 							{
 								if( pTerrainVtx[iIndex].vPosition.y - ((fy/10.0f) * m_iHeight) > vPickPos.y )
 									pTerrainVtx[iIndex].vPosition.y -= (fy/10.0f) * m_iHeight;
 								else
 									pTerrainVtx[iIndex].vPosition.y = vPickPos.y;
 							}
 						}
					
					}
					break;
				case EDITHILL:
					{
						if(Len <= m_iRange)
							pTerrainVtx[iIndex].vPosition.y = (_float)m_iHeight;
					}
					break;
				}
			}
		}
		((Engine::CVIBuffer*)((CTerrain*)m_pTerrain)->GetBuffer())->SetVtxInfo(pTerrainVtx);
	}
	else if(iNum == 1)
	{
		if(m_pSecondTerrain == NULL)
			return;
		Engine::VTXTEX* pTerrainVtx = ((CTerrain2*)m_pSecondTerrain)->GetVtxTex();

		_vec3	vPickPos = _vec3(0.f , 0.f , 0.f);
		_int	iPickIndex = CMouse::GetInstance()->PickTerrain(&vPickPos );

		CMouse::GetInstance()->SetVtx(((CTerrain2*)m_pSecondTerrain)->GetVtxSizeX() , ((CTerrain2*)m_pSecondTerrain)->GetVtxSizeZ() , (_int)((CTerrain2*)m_pSecondTerrain)->GetVtxItv());
		CMouse::GetInstance()->SetVtxTex(pTerrainVtx);

		if(iPickIndex < 0)
			return ;

		float fx	=  (vPickPos.x/* / 10*/);
		float fz	=  (vPickPos.z/* / 10*/);

		int startX	= (int)(fx - m_iRange);
		int startZ	= (int)(fz - m_iRange);
		int endX	= (int)(fx + m_iRange);
		int endZ	= (int)(fz + m_iRange);

		if( startX < 0 ) { startX = 0; }
		if( startZ < 0 ) { startZ = 0; }
		if( endX >= m_iVtx2Size ) { endX = m_iVtx2Size - 1 ; }	// 0���� �����ϱ⶧���� ���� -1
		if( endZ >= m_iVtx2Size ) { endZ = m_iVtx2Size - 1 ; }	// 0���� �����ϱ⶧���� ���� -1

		_vec3 vPos = _vec3( 0.0f , 0.0f , 0.0f );

		for( int z = 0 ; z <= endZ ; ++z )
		{
			for( int x = 0 ; x <= endX ; ++x )
			{
				_int iIndex = z * m_iVtx2Size + x;

				vPos = pTerrainVtx[iIndex].vPosition /*/ (float)10*/; //���͹��� �����ش�
				vPos.y = 0.0f;
				//-1.0f/(float)m_fBrushOutSize*(Len*Len) + (float)m_fBrushOutSize;
				// �귯�� ũ�⸸ŭ�� ��ġ���� ���콺 ��ġ������ �Ÿ��� ���Ѵ�.
				D3DXVECTOR3 Pix = vPos - (D3DXVECTOR3( vPickPos.x , 0.0f , vPickPos.z ) /*/ (float)10*/);
				float Len = D3DXVec3Length( &Pix );

				float fy = -1.0f / (float)m_iRange * ( Len * Len ) + (float)m_iRange;
				//Up

				switch(m_iEditMode)
				{
				case EDITUPDOWN:
					{
// 						if(m_iHeight == 0)
// 							pTerrainVtx[iIndex].vPosition.y = 0.f;

						if( Len < m_iRange )
							pTerrainVtx[iIndex].vPosition.y += ( fy / 10.0f ) * (m_iHeight / 100.f);
					}
					break;
				case EDITCENTER:
					{
						if(Len <= m_iRange)
						{
							if( abs(pTerrainVtx[iIndex].vPosition.y - vPickPos.y) > 0.001f )
							{
								if(pTerrainVtx[iIndex].vPosition.y < vPickPos.y)
								{
									if(pTerrainVtx[iIndex].vPosition.y + ( ( fy / 10.f) * m_iHeight ) < vPickPos.y)
										pTerrainVtx[iIndex].vPosition.y += ( fy / 10.f) * m_iHeight;
								}
							}
							else if( pTerrainVtx[iIndex].vPosition.y > vPickPos.y )
							{
								if( pTerrainVtx[iIndex].vPosition.y - ((fy/10.0f) * m_iHeight) > vPickPos.y )
									pTerrainVtx[iIndex].vPosition.y -= (fy/10.0f) * m_iHeight;
								else
									pTerrainVtx[iIndex].vPosition.y = vPickPos.y;
							}
						}
					}
					break;
				case EDITHILL:
					{
						if(Len <= m_iRange)
							pTerrainVtx[iIndex].vPosition.y = (_float)m_iHeight;
					}
					break;
				}
			}
		}
		((Engine::CVIBuffer*)((CTerrain2*)m_pSecondTerrain)->GetBuffer())->SetVtxInfo(pTerrainVtx);
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CToolView::CheckMove(void)																		//�ͷ��� �̵��� ���
{
	m_isMove = m_pSheet->m_MapPage.GetMove();	//��ư�� ���ȴ�
	m_iNum = m_pSheet->m_MapPage.GetNum();		//� �ͷ����� �����߳�
	m_vPos = m_pSheet->m_MapPage.GetPos();		//�󸶸�ŭ �̵���ų���ΰ�

	if(m_isMove == true)
	{
		if(m_iNum == 0)
			((CTerrain*)m_pTerrain)->SetVerTex(m_vPos);
		else
			((CTerrain2*)m_pSecondTerrain)->SetVerTex(m_vPos);

		m_pSheet->m_MapPage.SetMove(false);
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

list<Engine::CGameObject*>* CToolView::GetList(const TCHAR*	pObejctTag)								//List�� ������ ����
{
	MAPLAYER::iterator iter = m_mapLayer.find(0);

	if(iter == m_mapLayer.end())
		return NULL;

	return iter->second->GetCurrentList(pObejctTag);
}