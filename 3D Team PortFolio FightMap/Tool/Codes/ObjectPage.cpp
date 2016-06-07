// ObjectPage.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "ObjectPage.h"
#include "FileInfo.h"
#include "MainFrm.h"
#include "ObjectTool.h"
#include "ToolView.h"
#include "ObjectView.h"
#include "Mouse.h"
#include "Export_Function.h"

// CObjectPage ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CObjectPage, CPropertyPage)

CObjectPage::CObjectPage()
	: CPropertyPage(CObjectPage::IDD)
	, m_fScaleX(1.f)
	, m_fScaleY(1.f)
	, m_fScaleZ(1.f)
	, m_fScaleRatio(1000.f)
	, m_iSelectComboNum(0)
	, m_fAngleX(0)
	, m_fAngleY(0)
	, m_fAngleZ(0)
	, m_fPosX(0)
	, m_fPosY(0)
	, m_fPosZ(0)
	, m_pMapResource(NULL)
	, m_isNeedUV(false)
	, m_isInstancing(false)
{
	m_strSelectName = L"";
}
//---------------------------------------------------------- ----------------------------------------------------------------------------------------------

CObjectPage::~CObjectPage()
{
	Release();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_fScaleX);
	DDX_Text(pDX, IDC_EDIT2, m_fScaleY);
	DDX_Text(pDX, IDC_EDIT3, m_fScaleZ);
	DDX_Text(pDX, IDC_EDIT7, m_fAngleX);
	DDX_Text(pDX, IDC_EDIT8, m_fAngleY);
	DDX_Text(pDX, IDC_EDIT9, m_fAngleZ);
	DDX_Text(pDX, IDC_EDIT12, m_fPosX);
	DDX_Text(pDX, IDC_EDIT13, m_fPosY);
	DDX_Text(pDX, IDC_EDIT14, m_fPosZ);
	DDX_Control(pDX, IDC_COMBO1, m_ObjectType);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_RADIO1, m_ctrAdd);
	DDX_Control(pDX, IDC_RADIO2, m_ctrModify);
	DDX_Control(pDX, IDC_CHECK1, m_ctrPicking);
	DDX_Control(pDX, IDC_CHECK2, m_ctrCollider);
	DDX_Control(pDX, IDC_SLIDER5, m_ctrScaleX);
	DDX_Control(pDX, IDC_SLIDER6, m_ctrScaleY);
	DDX_Control(pDX, IDC_SLIDER7, m_ctrScaleZ);
	DDX_Control(pDX, IDC_SLIDER2, m_ctrAngleX);
	DDX_Control(pDX, IDC_SLIDER3, m_ctrAngleY);
	DDX_Control(pDX, IDC_SLIDER4, m_ctrAngleZ);
	DDX_Control(pDX, IDC_BUTTON9, m_ctrMoveMinusZ);
	DDX_Control(pDX, IDC_BUTTON11, m_ctrMovePlusZ);
	DDX_Control(pDX, IDC_BUTTON5, m_ctrMovePlusY);
	DDX_Control(pDX, IDC_BUTTON7, m_ctrMoveMinusY);
	DDX_Control(pDX, IDC_BUTTON6, m_ctrMoveMinusX);
	DDX_Control(pDX, IDC_BUTTON8, m_ctrMovePlusX);
	DDX_Control(pDX, IDC_BUTTON12, m_ctrPreview);
	DDX_Control(pDX, IDC_CHECK4, m_Delete);
	DDX_Control(pDX, IDC_CHECK5, m_ctrObjectWireFrame);
	DDX_Control(pDX, IDC_CHECK6, m_ctrBillBorad);
	DDX_Control(pDX, IDC_CHECK8, m_ctrNeedUV);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CObjectPage, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CObjectPage::OnCbnSelchangeObjectType)
	ON_WM_DROPFILES()
	ON_LBN_SELCHANGE(IDC_LIST1, &CObjectPage::OnLbnSelchangeList)
	ON_BN_CLICKED(IDC_CHECK1, &CObjectPage::OnBnClickedPickingBox)
	ON_BN_CLICKED(IDC_CHECK2, &CObjectPage::OnBnClickedColliderBox)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER5, &CObjectPage::OnNMCustomdrawScaleX)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER6, &CObjectPage::OnNMCustomdrawScaleY)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER7, &CObjectPage::OnNMCustomdrawScaleZ)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CObjectPage::OnNMCustomdrawAngleX)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER3, &CObjectPage::OnNMCustomdrawAngleY)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER4, &CObjectPage::OnNMCustomdrawAngleZ)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDOK, &CObjectPage::OnBnClickedSave)
	ON_BN_CLICKED(IDCANCEL, &CObjectPage::OnBnClickedLoad)
	ON_BN_CLICKED(IDC_BUTTON9, &CObjectPage::OnBnClickedMoveMinusZ)
	ON_BN_CLICKED(IDC_BUTTON11, &CObjectPage::OnBnClickedMovePlusZ)
	ON_BN_CLICKED(IDC_BUTTON5, &CObjectPage::OnBnClickedMovePlusY)
	ON_BN_CLICKED(IDC_BUTTON7, &CObjectPage::OnBnClickedMoveMinusY)
	ON_BN_CLICKED(IDC_BUTTON6, &CObjectPage::OnBnClickedMoveMinusX)
	ON_BN_CLICKED(IDC_BUTTON8, &CObjectPage::OnBnClickedMovePlusX)
	ON_BN_CLICKED(IDC_BUTTON12, &CObjectPage::OnBnClickedPreview)
	ON_BN_CLICKED(IDC_CHECK5, &CObjectPage::OnBnClickedObjectWireFrame)
	ON_BN_CLICKED(IDC_CHECK6, &CObjectPage::OnBnClickedBillBorad)
	ON_BN_CLICKED(IDC_CHECK8, &CObjectPage::OnBnClickedNeedUV)
END_MESSAGE_MAP()


// CObjectPage �޽��� ó�����Դϴ�.
//--------------------------------------------------------------------------------------------------------------------------------------------------------

BOOL CObjectPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_pMainFrm = (CMainFrame*)AfxGetMainWnd();
	m_pMapLayer = &m_pMainFrm->m_pMainView->m_mapLayer;

	///�����̴� ���� �ʱ�ȭ

	m_fScaleX = m_ctrScaleX.GetPos() / (float)m_ctrScaleX.GetRangeMax();
	m_ctrScaleX.SetRange(0, 5000);
	m_ctrScaleX.SetPos((int)m_ctrScaleX.GetRangeMax());


	m_fScaleY = m_ctrScaleY.GetPos() / (float)m_ctrScaleY.GetRangeMax();
	m_ctrScaleY.SetRange(0, 5000);
	m_ctrScaleY.SetPos((int)m_ctrScaleY.GetRangeMax());

	m_fScaleX = m_ctrScaleZ.GetPos() / (float)m_ctrScaleZ.GetRangeMax();
	m_ctrScaleZ.SetRange(0, 5000);
	m_ctrScaleZ.SetPos((int)m_ctrScaleZ.GetRangeMax());

	m_ctrAngleX.SetRange(0 , 360);
	m_ctrAngleX.SetPos(0);
	m_ctrAngleX.SetTicFreq(1);

	m_ctrAngleY.SetRange(0 , 360);
	m_ctrAngleY.SetPos(0);
	m_ctrAngleY.SetTicFreq(1);

	m_ctrAngleZ.SetRange(0 , 360);
	m_ctrAngleZ.SetPos(0);
	m_ctrAngleZ.SetTicFreq(1);

	m_ctrAdd.SetCheck(BST_CHECKED);			//���ϴ� ��Ʈ�� �Լ��� üũ �Ǿ� �ְ� �Ѵ� 

	Load_MeshList();

	MAPLIST::iterator		iter = m_MeshPathMap.begin();
	MAPLIST::iterator		iter_end = m_MeshPathMap.end();

	for( ; iter != iter_end; ++iter)
		m_ObjectType.AddString(iter->first);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectPage::OnCbnSelchangeObjectType()					//Ÿ���� ���� �޺��ڽ�
{
	UpdateData(TRUE);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString	strTemp;
	m_iSelectComboNum = m_ObjectType.GetCurSel();

	m_ObjectType.GetLBText(m_iSelectComboNum, strTemp);

	m_ListBox.ResetContent();

	TCHAR      szGroupTag[64] = L"";
	lstrcpy(szGroupTag, (LPWSTR)strTemp.operator LPCWSTR());   

	Set_MeshList( Find_MeshTag(szGroupTag) );
	
	UpdateData(FALSE);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectPage::OnLbnSelchangeList()														//ListBox ��Ʈ��
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	int iSelectIndex = m_ListBox.GetCurSel();

	if(iSelectIndex < 0)
		return;

	m_ListBox.GetText(iSelectIndex , m_strSelectName);

	TCHAR szTemp[MAX_PATH];
	lstrcpy(szTemp , m_strSelectName);
	m_Preview.SetName(szTemp);
	UpdateData(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

HRESULT CObjectPage::Ready_EnvironmentLayer(OBJINFO* ObjectInfo)			//Object ����
{
	m_pToolObject = CObjectTool::Create(ObjectInfo );

	if(m_pToolObject == NULL)
		return E_FAIL;

	(*m_pMapLayer)[LAYER_ENVIRONMENT]->Insert_GameObject(L"ToolObject" , m_pToolObject);

	return S_OK;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectPage::LoadMesh(const TCHAR* pMeshTag, const TCHAR* pFullPath ,const TCHAR* pFileTag)						//Mesh Load
{
  	if(FAILED(Engine::Ready_Mesh(Engine::GetGraphicDev() , RESOURCE_STATIC, Engine::CMesh::TYPE_STATIC , pMeshTag, pFullPath ,pFileTag )))
  	{
  		::msg_box("Ready_Mesh FAILED");
  		return;
  	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectPage::GetType(void)																	//���ϴ� Type�� �޺��ڽ� ����
{
	UpdateData(TRUE);

	if(m_ctrAdd.GetCheck() == 1)														//�߰� �ϴ� ��ư�� ������
	{
// 		if(m_pToolObject == NULL)
// 			return;

		_vec3	vPickPos =	_vec3(0.f , 0.f , 0.f);

		CMouse::GetInstance()->GetTerrainInfo(&vPickPos );

		if(vPickPos == _vec3(0.f , 0.f ,0.f))		//���� ���� ��ư ����	` 
			return;
		else
		{
			if (m_isInstancing == true)
				m_tObjectInfo.isInstancing = true;
			else
				m_tObjectInfo.isInstancing = false;

			if(m_isNeedUV == true)
				m_tObjectInfo.isNeedUV = true;
			else
				m_tObjectInfo.isNeedUV = false;

			lstrcpy(m_tObjectInfo.szName , m_strSelectName.operator LPCWSTR());
			m_tObjectInfo.vScale = _vec3(m_fScaleX , m_fScaleY ,m_fScaleZ);
			m_tObjectInfo.vPos = vPickPos;
			m_tObjectInfo.vAngle = _vec3(m_fAngleX , m_fAngleY , m_fAngleZ);

			m_fPosX = m_tObjectInfo.vPos.x;
			m_fPosY = m_tObjectInfo.vPos.y;
			m_fPosZ = m_tObjectInfo.vPos.z;

			if(FAILED(Ready_EnvironmentLayer(&m_tObjectInfo )))
			{
				AfxMessageBox(L"Ready_EnvironmentLayer  ���� ����");
				return ;
			}
		}
	}
	else if(m_ctrModify.GetCheck() == 1)
	{
		// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
		list<Engine::CGameObject*>* ObjList = m_pMainFrm->m_pMainView->GetList(L"ToolObject");

		if(ObjList == NULL)
			return;

		LISTOBJECT::iterator iter = ObjList->begin();
		LISTOBJECT::iterator iter_end = ObjList->end();

		for( ; iter != iter_end; ++iter)
		{
			_matrix* pMatrix = &((CObjectTool*)(*iter))->GetBoxWorld();
			const Engine::VTXTEX* pVtxTex = ((CObjectTool*)(*iter))->GetVtxTex();
			_bool isPicking = CMouse::GetInstance()->PickObject(pMatrix , pVtxTex);		//�ϴ� Transform�� ������ �ѱ�� 

			if(isPicking == true)				
			{
				m_pObjectInfo = ((CObjectTool*)(*iter))->GetInfo();

				m_fScaleX = m_pObjectInfo->m_vScale.x;			//������ ������ ������ ����Ʈ �ڽ��� ��´� 
				m_fScaleY = m_pObjectInfo->m_vScale.y;
				m_fScaleZ = m_pObjectInfo->m_vScale.z;

				m_fAngleX = m_pObjectInfo->m_fAngle[m_pObjectInfo->ANGLE_X];//������ ȸ�� ������ ����Ʈ �ڽ��� ��´� 
				m_fAngleY = m_pObjectInfo->m_fAngle[m_pObjectInfo->ANGLE_Y];
				m_fAngleZ = m_pObjectInfo->m_fAngle[m_pObjectInfo->ANGLE_Z];			//�� �ٲ�

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 				m_fPosX = m_pObjectInfo->m_vPosition.x;
				m_fPosY = m_pObjectInfo->m_vPosition.y;
				m_fPosZ = m_pObjectInfo->m_vPosition.z;

 				((CObjectTool*)(*iter))->SetInfo(m_pObjectInfo);				//�־���� ������ �ٽ� �������ش� -> �ǽð����� �ٲ�� �ϰ� �ʹ�

				if(m_Delete.GetCheck() == 1)
				{
					::safe_delete(*iter);
					iter = ObjList->erase(iter);
					break;
				}
			}
		}
	}

	UpdateData(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectPage::OnBnClickedPickingBox()													//�浹�� �ڽ� ����
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	list<Engine::CGameObject*>* ObjList = m_pMainFrm->m_pMainView->GetList(L"ToolObject");

	if(ObjList == NULL)
		return;

	LISTOBJECT::iterator iter = ObjList->begin();
	LISTOBJECT::iterator iter_end = ObjList->end();

	for( ; iter != iter_end ; ++iter)
	{
		if(m_ctrPicking.GetCheck() == 1)		//��ŷ �ڽ��� ������ Ʈ��
			((CObjectTool*)(*iter))->SetPickingRender(true);
		else									//��ŷ �ڽ��� �ȴ����� �޽�
			((CObjectTool*)(*iter))->SetPickingRender(false);
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectPage::OnBnClickedColliderBox()													//�浹�� �ڽ� ����
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	list<Engine::CGameObject*>* ObjList = m_pMainFrm->m_pMainView->GetList(L"ToolObject");

	if(ObjList == NULL)
		return;

	LISTOBJECT::iterator iter = ObjList->begin();
	LISTOBJECT::iterator iter_end = ObjList->end();

	for( ; iter != iter_end ; ++iter)
	{
		if(m_ctrCollider.GetCheck() == 1)		//��ŷ �ڽ��� ������ Ʈ��
			((CObjectTool*)(*iter))->SetColliderRender(true);
		else									//��ŷ �ڽ��� �ȴ����� �޽�
			((CObjectTool*)(*iter))->SetColliderRender(false);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void CObjectPage::OnNMCustomdrawScaleX(NMHDR *pNMHDR, LRESULT *pResult)						//ũ��� �����̴� X
{
	UpdateData(TRUE);

	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_fScaleX = ( (float)m_ctrScaleX.GetPos() / m_fScaleRatio ) ;

	*pResult = 0;
	UpdateData(FALSE);

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectPage::OnNMCustomdrawScaleY(NMHDR *pNMHDR, LRESULT *pResult)						//ũ��� �����̴� Y
{
	UpdateData(TRUE);
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_fScaleY = ( (float)m_ctrScaleY.GetPos() / m_fScaleRatio ) ;

	*pResult = 0;
	UpdateData(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectPage::OnNMCustomdrawScaleZ(NMHDR *pNMHDR, LRESULT *pResult)						//ũ��� �����̴� Z
{
	UpdateData(TRUE);
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_fScaleZ = ( (float)m_ctrScaleZ.GetPos() / m_fScaleRatio ) ;
	*pResult = 0;
	UpdateData(FALSE);

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectPage::OnNMCustomdrawAngleX(NMHDR *pNMHDR, LRESULT *pResult)						//ȸ���� �����̴� X
{
	UpdateData(TRUE);
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_fAngleX = float(m_ctrAngleX.GetPos());

	*pResult = 0;
	UpdateData(FALSE);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void CObjectPage::OnNMCustomdrawAngleY(NMHDR *pNMHDR, LRESULT *pResult)						//ȸ���� �����̴� Y
{
	UpdateData(TRUE);

	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	m_fAngleY = float(m_ctrAngleY.GetPos());

	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;
	UpdateData(FALSE);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void CObjectPage::OnNMCustomdrawAngleZ(NMHDR *pNMHDR, LRESULT *pResult)						//ȸ���� �����̴� Z
{
	UpdateData(TRUE);
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_fAngleZ = float(m_ctrAngleZ.GetPos());

	*pResult = 0;
	UpdateData(FALSE);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void CObjectPage::OnBnClickedMoveMinusZ()													//�̵��� ���̳ʽ� ��ư Z
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	m_fPosZ -= 1.f;
	m_pObjectInfo->m_vPosition.z = m_fPosZ;
	UpdateData(FALSE);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void CObjectPage::OnBnClickedMovePlusZ()													//�̵��� �÷��� ��ư Z
{
	UpdateData(TRUE);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_fPosZ += 1.f;
	m_pObjectInfo->m_vPosition.z = m_fPosZ;
	UpdateData(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectPage::OnBnClickedMovePlusY()													//�̵��� �÷��� ��ư Y
{
	UpdateData(TRUE);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_fPosY += 1.f;
	m_pObjectInfo->m_vPosition.y = m_fPosY;
	UpdateData(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectPage::OnBnClickedMoveMinusY()													//�̵��� ���̳ʽ� ��ư Y
{
	UpdateData(TRUE);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_fPosY -= 1.f;
	m_pObjectInfo->m_vPosition.y = m_fPosY;
	UpdateData(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectPage::OnBnClickedMoveMinusX()													//�̵��� ���̳ʽ� ��ư X
{
	UpdateData(TRUE);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_fPosX -= 1.f;
	m_pObjectInfo->m_vPosition.x = m_fPosX;
	UpdateData(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectPage::OnBnClickedMovePlusX()													//�̵��� �÷��� ��ư Z
{
	UpdateData(TRUE);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_fPosX += 1.f;
	m_pObjectInfo->m_vPosition.x = m_fPosX;
	UpdateData(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectPage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)				//�ǽð� �����̴� üũ
{
 	UpdateData(TRUE);
 	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
 
 	CPropertyPage::OnHScroll(nSBCode, nPos, pScrollBar);
 	if(m_ctrModify.GetCheck() == 1)															//���� �ϴ� ��ư�� ������
 	{
		m_pObjectInfo->m_vScale.x = float( m_ctrScaleX.GetPos() * 0.001f );		//�����̴� �����̴� ������ �־��ش�
		m_pObjectInfo->m_vScale.y = float( m_ctrScaleY.GetPos() * 0.001f );
		m_pObjectInfo->m_vScale.z = float( m_ctrScaleZ.GetPos() * 0.001f );

		m_pObjectInfo->m_fAngle[m_pObjectInfo->ANGLE_X] = D3DXToRadian( float( m_ctrAngleX.GetPos() ));
		m_pObjectInfo->m_fAngle[m_pObjectInfo->ANGLE_Y] = D3DXToRadian( float( m_ctrAngleY.GetPos() ));
		m_pObjectInfo->m_fAngle[m_pObjectInfo->ANGLE_Z] = D3DXToRadian( float( m_ctrAngleZ.GetPos() ));
 
 	}
 	UpdateData(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectPage::OnBnClickedSave()															//������Ʈ Save
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);

	CFileDialog		Dlg(FALSE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.dat", this);

	TCHAR		szDirtoryPath[MAX_PATH];
	GetCurrentDirectory( sizeof(szDirtoryPath) , szDirtoryPath);
	SetCurrentDirectory(szDirtoryPath);

	PathRemoveFileSpec( szDirtoryPath );
	lstrcat( szDirtoryPath , L"\\Data");
	Dlg.m_ofn.lpstrInitialDir = szDirtoryPath ;

	if( Dlg.DoModal() == IDCANCEL )
		return;

	list<Engine::CGameObject*>* ObjList = m_pMainFrm->m_pMainView->GetList(L"ToolObject");

	if(ObjList == NULL)
		return;

	LISTOBJECT::iterator iter = ObjList->begin();
	LISTOBJECT::iterator iter_end = ObjList->end();


	DWORD		dwByte = 0;

	HANDLE hFile = CreateFile(Dlg.GetPathName() , GENERIC_WRITE , 0 , NULL , CREATE_ALWAYS
		, FILE_ATTRIBUTE_NORMAL , NULL);

 	OBJINFO tObjList;
 	ZeroMemory(&tObjList , sizeof(OBJINFO));
 
 	for( ; iter != iter_end; ++iter)
 	{
 		ZeroMemory(&tObjList , sizeof(OBJINFO));
 		
 		lstrcpy(tObjList.szName , ((CObjectTool*)(*iter))->GetObjInfo()->szName);
 		tObjList.vAngle = ((CObjectTool*)(*iter))->GetInfo()->m_fAngle;
 		tObjList.vPos = ((CObjectTool*)(*iter))->GetInfo()->m_vPosition;
 		tObjList.vScale = ((CObjectTool*)(*iter))->GetInfo()->m_vScale;
 		tObjList.isInstancing = ((CObjectTool*)(*iter))->GetObjInfo()->isInstancing;
		tObjList.isNeedUV= ((CObjectTool*)(*iter))->GetObjInfo()->isNeedUV;

 		WriteFile(hFile , &tObjList , sizeof(OBJINFO) , &dwByte , NULL);
 	}

	CloseHandle(hFile);
	UpdateData(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectPage::OnBnClickedLoad()															//������Ʈ Load
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	CFileDialog		Dlg(TRUE, L"dat", L"*.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT
		, L"*.dat", this);

	if(Dlg.DoModal() == IDCANCEL)
		return;
	int a = 0;

	HANDLE		hFile = CreateFile(Dlg.GetPathName() , GENERIC_READ , 0 , NULL , OPEN_EXISTING , FILE_ATTRIBUTE_NORMAL , NULL);
	DWORD		dwByte = 0;

	while(true)
	{
		OBJINFO*	pObject = new OBJINFO;
		ReadFile(hFile , pObject , sizeof(OBJINFO) , &dwByte , NULL);

		if(dwByte == 0)
		{
			::safe_delete(pObject);
			break;
		}

		if(!lstrcmp(pObject->szName , L"Flag13"))
		{
			::safe_delete(pObject);
			continue;
		}
		if(FAILED(Ready_EnvironmentLayer( pObject )))
			return;

	}

	CloseHandle(hFile);
	UpdateData(FALSE);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void CObjectPage::OnBnClickedPreview()									//Preview ���� ��ư
{
	UpdateData(TRUE);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if(m_Preview.GetSafeHwnd() == NULL)
	{
		m_Preview.Create(IDD_PREVIEW);
	}
	m_Preview.ShowWindow(SW_SHOW);
	
	UpdateData(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectPage::Release(void)
{
	//�̿��̸� SaveData���� ����� ���ڿ��� ������

	OBJLIST::iterator iter_list = m_ObjList.begin();
	OBJLIST::iterator iter_list_end = m_ObjList.end();

	for( ; iter_list != iter_list_end; ++iter_list)
	{
		::safe_delete_array(*iter_list);
	}
	m_ObjList.clear();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectPage::Load_MeshList(void)
{			//											����
	Find_MeshFilePath(L"\\Tool\\Bin\\Resource\\Mesh\\Object",        L"\\Data\\Path\\ObjectPath.txt",      m_ObjectPathList);      // Character List

	Set_MeshTagList( &m_ObjectPathList, &m_MeshPathMap);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectPage::Find_MeshFilePath(const TCHAR* pFilePath , const TCHAR* pSavetxtpath , PATHLIST& rPathList)
{
	TCHAR      szDirtoryPath[MAX_PATH];
	GetCurrentDirectory(sizeof(szDirtoryPath), szDirtoryPath);
	SetCurrentDirectory(szDirtoryPath);

	PathRemoveFileSpec(szDirtoryPath);
	PathRemoveFileSpec(szDirtoryPath);
	lstrcat(szDirtoryPath, pFilePath);

	CFileInfo      FileInfo;
	FileInfo.DirtoryInfoExtraction(szDirtoryPath, rPathList);

	Save_MeshPath(pSavetxtpath, rPathList);

}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectPage::Save_MeshPath(const TCHAR* pSavetxtPath , PATHLIST& rPathList)
{
	TCHAR      szDirtoryPath[MAX_PATH];
	GetCurrentDirectory(sizeof(szDirtoryPath), szDirtoryPath);
	SetCurrentDirectory(szDirtoryPath);

	PathRemoveFileSpec(szDirtoryPath);
	PathRemoveFileSpec(szDirtoryPath);
	lstrcat(szDirtoryPath, pSavetxtPath);

	wofstream      SaveFile;

	wstring wstrPathName = szDirtoryPath;
	SaveFile.open(wstrPathName.c_str(), ios::out);

	int      iCount = 0;
 	for(list<MESHPATH*>::iterator iter = rPathList.begin();
 		iter != rPathList.end(); ++iter)
 	{
 		++iCount;
 		SaveFile << (*iter)->wstrFileTag << L"|";
 		SaveFile << (*iter)->wstrObjTag << L"|";
 
 		if(iCount == rPathList.size())
 			SaveFile << (*iter)->wstrFullPath << flush;
 		else
 			SaveFile << (*iter)->wstrFullPath << endl;
 	}

	SaveFile.close();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectPage::Set_MeshTagList(PATHLIST* pPathList , MAPLIST* pMapTag)	//�ʱ�ȭ �Ҷ� �޽�����Ʈ
{
	PATHLIST::iterator      iter    = pPathList->begin();
	PATHLIST::iterator      iter_end = pPathList->end();

	for( ; iter != iter_end; ++iter)
	{
		MAPLIST::iterator   Tagiter = find_if( pMapTag->begin(), pMapTag->end(), Engine::CTag_Finder( (*iter)->wstrGroupTag.c_str() ) );
		if(Tagiter == pMapTag->end())
		{
			list<const TCHAR*>      ObjTagList;
			ObjTagList.push_back( (*iter)->wstrObjTag.c_str() );
			pMapTag->insert(MAPLIST::value_type( (*iter)->wstrGroupTag.c_str(), ObjTagList) );
		}
		else
			Tagiter->second.push_back( (*iter)->wstrObjTag.c_str() );

		LoadMesh((*iter)->wstrObjTag.c_str() , (*iter)->wstrFullPath.c_str() , (*iter)->wstrFileTag.c_str() );
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

list<const TCHAR*>* CObjectPage::Find_MeshTag(const TCHAR* pGroupTag)		//
{
	MAPLIST::iterator   Tagiter = find_if( m_MeshPathMap.begin(), m_MeshPathMap.end(), Engine::CTag_Finder( pGroupTag ) );

	if( Tagiter == m_MeshPathMap.end() )
		return NULL;

	return &Tagiter->second;
}

void CObjectPage::Set_MeshList(list<const TCHAR*>* pPathList)
{
	LISTPATH::iterator      iter     = pPathList->begin();
	LISTPATH::iterator      iter_end = pPathList->end();

	for( ; iter != iter_end; ++iter)
		m_ListBox.AddString( (*iter) );
}
void CObjectPage::OnBnClickedObjectWireFrame()
{
	UpdateData(TRUE);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	list<Engine::CGameObject*>* ObjList = m_pMainFrm->m_pMainView->GetList(L"ToolObject");

	if(ObjList == NULL)
		return;

	LISTOBJECT::iterator iter = ObjList->begin();
	LISTOBJECT::iterator iter_end = ObjList->end();

	for( ; iter != iter_end ; ++iter)
	{
		if(m_ctrObjectWireFrame.GetCheck() == 1)
			((CObjectTool*)(*iter))->SetFrameType(2);
		else									//������ �ڽ��� �ȴ����� �޽�
			((CObjectTool*)(*iter))->SetFrameType(3);
	}
	UpdateData(FALSE);
}
void CObjectPage::OnBnClickedBillBorad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(TRUE);
	if(m_ctrBillBorad.GetCheck() == 1)
		m_isInstancing = true;
	else
		m_isInstancing = false;
	UpdateData(FALSE);
}

void CObjectPage::OnBnClickedNeedUV()
{
	UpdateData(TRUE);
	if(m_ctrNeedUV.GetCheck() == 1)
		m_isNeedUV = true;
	else
		m_isNeedUV = false;
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	UpdateData(FALSE);
}
