// NaviMeshPage.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "NaviMeshPage.h"
#include "Mouse.h"
#include "NaviFlag.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "ObjectTool.h"
#include "..\..\Engine\Utility\Codes\NaviMesh.h"
#include "..\..\Engine\Resource\Codes\StaticMesh.h"
#include "Export_Function.h"

// CNaviMeshPage ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CNaviMeshPage, CPropertyPage)

CNaviMeshPage::CNaviMeshPage()
: CPropertyPage(CNaviMeshPage::IDD)
, m_pNaviFlag(NULL)
, m_iCellCnt(0)
, m_isRender(false)
, m_iType(0)
, m_fFirstNaviPosX(0)
, m_fFirstNaviPosY(0)
, m_fFirstNaviPosZ(0)
, m_fSecNaviPosX(0)
, m_fSecNaviPosY(0)
, m_fSecNaviPosZ(0)
, m_fThirdNaviPosX(0)
, m_fThirdNaviPosY(0)
, m_fThirdNaviPosZ(0)
, m_fSelNaviPosX(0)
, m_fSelNaviPosY(0)
, m_fSelNaviPosZ(0)
{
	memset( m_vPoint , 0 , sizeof(_vec3) * 3 );
}

CNaviMeshPage::~CNaviMeshPage()
{
}

void CNaviMeshPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_iCellCnt);
	DDX_Text(pDX, IDC_EDIT2, m_fFirstNaviPosX);
	DDX_Text(pDX, IDC_EDIT22, m_fFirstNaviPosY);
	DDX_Text(pDX, IDC_EDIT26, m_fFirstNaviPosZ);
	DDX_Text(pDX, IDC_EDIT3, m_fSecNaviPosX);
	DDX_Text(pDX, IDC_EDIT23, m_fSecNaviPosY);
	DDX_Text(pDX, IDC_EDIT27, m_fSecNaviPosZ);
	DDX_Text(pDX, IDC_EDIT7, m_fThirdNaviPosX);
	DDX_Text(pDX, IDC_EDIT24, m_fThirdNaviPosY);
	DDX_Text(pDX, IDC_EDIT28, m_fThirdNaviPosZ);
	DDX_Text(pDX, IDC_EDIT20, m_fSelNaviPosX);
	DDX_Text(pDX, IDC_EDIT25, m_fSelNaviPosY);
	DDX_Text(pDX, IDC_EDIT29, m_fSelNaviPosZ);
	DDX_Control(pDX, IDC_RADIO1, m_ctrAdd);
	DDX_Control(pDX, IDC_RADIO2, m_ctrModify);
	DDX_Control(pDX, IDC_CHECK1, m_ctrNaviRender);
	DDX_Control(pDX, IDC_RADIO3, m_ctrPickingTerrain);
	DDX_Control(pDX, IDC_RADIO4, m_ctrPickingMesh);
	DDX_Control(pDX, IDC_BUTTON4, m_ctrLinkNavi);
}


BEGIN_MESSAGE_MAP(CNaviMeshPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, &CNaviMeshPage::OnBnClickedDeleteLastCell)
	ON_BN_CLICKED(IDC_BUTTON10, &CNaviMeshPage::OnBnClickedDeleteAllCell)
	ON_BN_CLICKED(IDOK, &CNaviMeshPage::OnBnClickedSave)
	ON_BN_CLICKED(IDCANCEL, &CNaviMeshPage::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_CHECK1, &CNaviMeshPage::OnBnClickedRenderNavi)
	ON_BN_CLICKED(IDC_RADIO3, &CNaviMeshPage::OnBnClickedPickingTerrain)
	ON_BN_CLICKED(IDC_RADIO4, &CNaviMeshPage::OnBnClickedPickingMesh)
	ON_BN_CLICKED(IDC_BUTTON4, &CNaviMeshPage::OnBnClickedLinkNavi)
END_MESSAGE_MAP()
//--------------------------------------------------------------------------------------------------------------------------------------------------------

BOOL CNaviMeshPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	Ready_NaviMesh();
	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	m_pMainFrm = (CMainFrame*)AfxGetMainWnd();
	m_pMapLayer = &m_pMainFrm->m_pMainView->m_mapLayer;

	m_pNaviFlag = CNaviFlag::Create();
	
	if(m_pNaviFlag == NULL)
		return E_FAIL;

	(*m_pMapLayer)[LAYER_ENVIRONMENT]->Insert_GameObject(L"NaviFlag" , m_pNaviFlag);
	
	m_ctrAdd.SetCheck(BST_CHECKED);
	m_ctrPickingTerrain.SetCheck(BST_CHECKED);
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

// CNaviMeshPage �޽��� ó�����Դϴ�.


void CNaviMeshPage::Ready_NaviMesh(void)										//�׺�޽� �ʱ�ȭ
{
	Engine::Reserve_NaviMeshSize(Engine::GetGraphicDev() , 50);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CNaviMeshPage::AddMode(void)												//�׺�޽� �߰�
{
	UpdateData(TRUE);

	_vec3	vPickPos = _vec3(0.f , 0.f , 0.f );

	if(m_iType == 0)
		CMouse::GetInstance()->GetTerrainInfo(&vPickPos);
	else
	{
		list<Engine::CGameObject*>* ObjList = m_pMainFrm->m_pMainView->GetList(L"ToolObject");

		if(ObjList == NULL)
			return;

		LISTOBJECT::iterator iter_list = ObjList->begin();
		LISTOBJECT::iterator iter_list_end = ObjList->end();

		for( ; iter_list != iter_list_end; ++iter_list)
		{
			_matrix* pMatrix = &((CObjectTool*)(*iter_list))->GetInfo()->m_matWorld;

			Engine::CComponent*	pBaseMesh = ((CObjectTool*)(*iter_list))->GetMeshCom();

			if(CMouse::GetInstance()->GetPickMeshPos(&vPickPos ,pMatrix , ((Engine::CStaticMesh*)pBaseMesh)->GetMesh() , WINSIZEX , WINSIZEY))
				break;
		}
	}
	if( vPickPos == _vec3(0.f , 0.f , 0.f) )
		return;

	m_vPoint[m_iCellCnt] = vPickPos;

	m_vecNaviMesh = *Engine::GetVecNaviMesh();

	VECNAVI::iterator iter = m_vecNaviMesh.begin();
	VECNAVI::iterator iter_end = m_vecNaviMesh.end();

	_float fDistance = 0.f;
	((CNaviFlag*)m_pNaviFlag)->SetPos(&m_vPoint[m_iCellCnt]);

	if(m_ctrAdd.GetCheck() == 1)
	{
		m_fFirstNaviPosX = m_vPoint[0].x;
		m_fFirstNaviPosY = m_vPoint[0].y;
		m_fFirstNaviPosZ = m_vPoint[0].z;

		m_fSecNaviPosX = m_vPoint[1].x;
		m_fSecNaviPosY = m_vPoint[1].y;
		m_fSecNaviPosZ = m_vPoint[1].z;

		m_fThirdNaviPosX = m_vPoint[2].x;
		m_fThirdNaviPosY = m_vPoint[2].y;
		m_fThirdNaviPosZ = m_vPoint[2].z;

		m_fSelNaviPosX = m_vPoint[m_iCellCnt].x;
		m_fSelNaviPosY = m_vPoint[m_iCellCnt].y;
		m_fSelNaviPosZ = m_vPoint[m_iCellCnt].z;

		for( ;  iter != iter_end; ++iter )
		{
			fDistance = D3DXVec3Length( &( *((*iter)->GetPoint(Engine::CNaviMesh::POINT_A))  - m_vPoint[m_iCellCnt] ));

			if(fDistance <= 2.f)
			{
				m_vPoint[m_iCellCnt] = *((*iter)->GetPoint(Engine::CNaviMesh::POINT_A)) ;
				((CNaviFlag*)m_pNaviFlag)->SetTexture(1);
				break;
			}
			else
				((CNaviFlag*)m_pNaviFlag)->SetTexture(0);

			fDistance = D3DXVec3Length( &( *((*iter)->GetPoint(Engine::CNaviMesh::POINT_B))  - m_vPoint[m_iCellCnt] ));

			if(fDistance <= 2.f)
			{
				m_vPoint[m_iCellCnt] = *((*iter)->GetPoint(Engine::CNaviMesh::POINT_B)) ;
				((CNaviFlag*)m_pNaviFlag)->SetTexture(1);
				break;
			}
			else
				((CNaviFlag*)m_pNaviFlag)->SetTexture(0);

			fDistance = D3DXVec3Length( &( *((*iter)->GetPoint(Engine::CNaviMesh::POINT_C))  - m_vPoint[m_iCellCnt] ));

			if(fDistance <= 2.f)
			{
				m_vPoint[m_iCellCnt] = *((*iter)->GetPoint(Engine::CNaviMesh::POINT_C)) ;
				((CNaviFlag*)m_pNaviFlag)->SetTexture(1);
				break;
			}
			else
				((CNaviFlag*)m_pNaviFlag)->SetTexture(0);
		}
		++m_iCellCnt;
		if( m_iCellCnt > 2 )
		{
			m_iCellCnt = 0;
			_vec3 vTemp = _vec3(0.f , 0.f , 0.f ) ;

			D3DXVec3Cross(&vTemp ,  & ( m_vPoint[1] - m_vPoint[0] ), & ( m_vPoint[2] - m_vPoint[1]) );	//2������ 1������ ������ �ߴ� 

			if( vTemp.y < 0 )
			{
				_vec3 vTemp = _vec3(0.f ,0.f ,0.f);
				vTemp = m_vPoint[2] ;
				m_vPoint[2] = m_vPoint[0];
				m_vPoint[0] = vTemp;

				m_iCellCnt = 0;
			}

			Engine::Add_NaviMesh( &m_vPoint[0] , &m_vPoint[1] , &m_vPoint[2] );
			//Engine::Link_NaviMesh();
			
		}
	}
	
	UpdateData(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CNaviMeshPage::ModifyMode(void)										//�׺�޽� ����
{
	UpdateData(TRUE);
	
	_float fDistance = 0.f;

	_vec3	vPickPos = _vec3(0.f , 0.f , 0.f );

	if(m_iType == 0)
		CMouse::GetInstance()->GetTerrainInfo(&vPickPos);
	else
	{
		list<Engine::CGameObject*>* ObjList = m_pMainFrm->m_pMainView->GetList(L"ToolObject");

		if(ObjList == NULL)
			return;

		LISTOBJECT::iterator iter_list = ObjList->begin();
		LISTOBJECT::iterator iter_list_end = ObjList->end();

		for( ; iter_list != iter_list_end; ++iter_list)
		{
			_matrix* pMatrix = &((CObjectTool*)(*iter_list))->GetInfo()->m_matWorld;

			Engine::CComponent*	pBaseMesh = ((CObjectTool*)(*iter_list))->GetMeshCom();

			if(CMouse::GetInstance()->GetPickMeshPos(&vPickPos ,pMatrix , ((Engine::CStaticMesh*)pBaseMesh)->GetMesh() , WINSIZEX , WINSIZEY))
				break;
		}
	}

	if( vPickPos == _vec3(0.f , 0.f , 0.f) )
		return;

	m_vecNaviMesh = *Engine::GetVecNaviMesh();

	VECNAVI::iterator iter2 = m_vecNaviMesh.begin();
	VECNAVI::iterator iter_end2 = m_vecNaviMesh.end();

	if(m_ctrModify.GetCheck() == 1)	//�׷��ϱ� ���� ��ư�� �������� 
	{
		for( ; iter2 != iter_end2; ++iter2)
		{
			fDistance = D3DXVec3Length( &( *((*iter2)->GetPoint(Engine::CNaviMesh::POINT_A))  - vPickPos ));		//���콺�� �����ִ� ���� Distance�� ���Ѵ�

			if(fDistance <= 2.f)																				//�ѻ����� �Ÿ��� 0.3�Ʒ��� �� ���� ���콺�� ��ǥ�� �缼�� �Ѵ�
			{
				((CNaviFlag*)m_pNaviFlag)->SetTexture(1);
				if(Engine::GetDIKeyState(DIK_HOME) & 0x8000)
					(*iter2)->UpDownPoint(Engine::CNaviMesh::POINT_A ,& ( *((*iter2)->GetPoint(Engine::CNaviMesh::POINT_A)) + _vec3(0.f , 0.001f , 0.f) ));
				else if(Engine::GetDIKeyState(DIK_END) & 0x8000)
					(*iter2)->UpDownPoint(Engine::CNaviMesh::POINT_A ,& ( *((*iter2)->GetPoint(Engine::CNaviMesh::POINT_A)) + _vec3(0.f , -0.001f , 0.f) ));
				else
					(*iter2)->ReSetNaviPoint(Engine::CNaviMesh::POINT_A ,&vPickPos);
			}
			else
				((CNaviFlag*)m_pNaviFlag)->SetTexture(0);


			fDistance = D3DXVec3Length( &( *((*iter2)->GetPoint(Engine::CNaviMesh::POINT_B))  - vPickPos ));

			if(fDistance <= 2.f)
			{
				((CNaviFlag*)m_pNaviFlag)->SetTexture(1);
				if(Engine::GetDIKeyState(DIK_HOME) & 0x8000)
					(*iter2)->UpDownPoint(Engine::CNaviMesh::POINT_B ,& ( *((*iter2)->GetPoint(Engine::CNaviMesh::POINT_B)) + _vec3(0.f , 0.001f , 0.f) ));
				else if(Engine::GetDIKeyState(DIK_END) & 0x8000)
					(*iter2)->UpDownPoint(Engine::CNaviMesh::POINT_B ,& ( *((*iter2)->GetPoint(Engine::CNaviMesh::POINT_B)) + _vec3(0.f , -0.001f , 0.f) ));
				else
					(*iter2)->ReSetNaviPoint(Engine::CNaviMesh::POINT_B ,&vPickPos);
			}
			else
				((CNaviFlag*)m_pNaviFlag)->SetTexture(0);

			fDistance = D3DXVec3Length( &( *((*iter2)->GetPoint(Engine::CNaviMesh::POINT_C))  - vPickPos ));

			if(fDistance <= 2.f)
			{
				((CNaviFlag*)m_pNaviFlag)->SetTexture(1);
				if(Engine::GetDIKeyState(DIK_HOME) & 0x8000)
					(*iter2)->UpDownPoint(Engine::CNaviMesh::POINT_C ,& ( *((*iter2)->GetPoint(Engine::CNaviMesh::POINT_C)) + _vec3(0.f , 0.001f , 0.f) ));
				else if(Engine::GetDIKeyState(DIK_END) & 0x8000)
					(*iter2)->UpDownPoint(Engine::CNaviMesh::POINT_C ,& ( *((*iter2)->GetPoint(Engine::CNaviMesh::POINT_C)) + _vec3(0.f , -0.001f , 0.f) ));
				else
					(*iter2)->ReSetNaviPoint(Engine::CNaviMesh::POINT_C ,&vPickPos);
			}
			else
				((CNaviFlag*)m_pNaviFlag)->SetTexture(0);
			
			m_fSelNaviPosX = vPickPos.x;
			m_fSelNaviPosY = vPickPos.y;
			m_fSelNaviPosZ = vPickPos.z;

		}
	}
	
	UpdateData(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CNaviMeshPage::OnBnClickedDeleteLastCell()													//���� �׺�޽� �����
{
	UpdateData(TRUE);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	Engine::CheckDelete(0);
	m_iCellCnt = 0;
	UpdateData(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CNaviMeshPage::OnBnClickedDeleteAllCell()													//��� �׺�޽� �����
{
	UpdateData(TRUE);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	Engine::CheckDelete(1);
	m_iCellCnt = 0;
	UpdateData(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CNaviMeshPage::OnBnClickedSave()															//�׺�޽� Save
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	CFileDialog		Dlg(FALSE , L"dat" , L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.dat" , this);
// 
	if(Dlg.DoModal() == IDCANCEL)
		return;

	DWORD		dwByte = 0;

	HANDLE hFile = CreateFile(Dlg.GetPathName() , GENERIC_WRITE , 0 , NULL , CREATE_ALWAYS
		, FILE_ATTRIBUTE_NORMAL , NULL);

	m_vecNaviMesh = *Engine::GetVecNaviMesh();

	VECNAVI::iterator iter = m_vecNaviMesh.begin();
	VECNAVI::iterator iter_end = m_vecNaviMesh.end();

	int i = m_vecNaviMesh.size();
	for( ; iter != iter_end; ++iter)
	{
		Engine::NAVICELL					pNaviCell;

		pNaviCell.vPosition[Engine::CNaviMesh::POINT_A] = *(*iter)->GetPoint(Engine::CNaviMesh::POINT_A);
		pNaviCell.vPosition[Engine::CNaviMesh::POINT_B] = *(*iter)->GetPoint(Engine::CNaviMesh::POINT_B);
		pNaviCell.vPosition[Engine::CNaviMesh::POINT_C] = *(*iter)->GetPoint(Engine::CNaviMesh::POINT_C);

		WriteFile(hFile , &pNaviCell , sizeof(Engine::NAVICELL) , &dwByte , NULL);
		//WriteFile(hFile�ڵ鿡 pNiveCell��ŭ�� NAVICELL������� ������ �Ѵ� dwByte������ );
	}

	CloseHandle(hFile);
	UpdateData(FALSE);

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CNaviMeshPage::OnBnClickedLoad()													//�׺�޽� Load
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	CFileDialog		Dlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.dat", this);

	if(Dlg.DoModal() == IDCANCEL)
		return;

	HANDLE		hFile = CreateFile(Dlg.GetPathName(), GENERIC_READ, 0, NULL, OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD		dwByte = 0;

	while ( true )
	{
		Engine::NAVICELL			pNavi;
		ReadFile(hFile , &pNavi , sizeof(Engine::NAVICELL) , &dwByte , NULL);

		if(dwByte == 0)
			break;

		Engine::Add_NaviMesh( &pNavi.vPosition[Engine::CNaviMesh::POINT_A] , &pNavi.vPosition[Engine::CNaviMesh::POINT_B] , &pNavi.vPosition[Engine::CNaviMesh::POINT_C] );

	}

	Engine::Link_NaviMesh();
	Engine::Init_Point_Neighbor();
	Engine::Init_Point_NeighborNavi();

	CloseHandle(hFile);
	UpdateData(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CNaviMeshPage::SetRender(_bool isRender)													//NaviMesh Flag ������
{
	if(m_pNaviFlag == NULL)
		return;

	m_isRender = isRender;
	
	if(m_isRender == true)
		((CNaviFlag*)m_pNaviFlag)->SetRender(true);
	else
		((CNaviFlag*)m_pNaviFlag)->SetRender(false);
}
void CNaviMeshPage::OnBnClickedRenderNavi()														//NaviMesh Rendering
{
	UpdateData(TRUE);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	if(m_ctrNaviRender.GetCheck() == 1)
		m_isNaviRender = true;
	else
		m_isNaviRender = false;
	UpdateData(FALSE);
}

_bool CNaviMeshPage::GetNaviRender(void)
{
	return m_isNaviRender;
}

void CNaviMeshPage::OnBnClickedPickingTerrain()
{
	UpdateData(TRUE);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_iType = 0;
	UpdateData(FALSE);
}

void CNaviMeshPage::OnBnClickedPickingMesh()
{
	UpdateData(TRUE);
	m_iType = 1;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(FALSE);
}
void CNaviMeshPage::OnBnClickedLinkNavi()
{
	UpdateData(TRUE);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	Engine::Link_NaviMesh();
	Engine::Init_Point_Neighbor();
	Engine::Init_Point_NeighborNavi();

	UpdateData(FALSE);
}