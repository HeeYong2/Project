// AnimationView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "AnimationPage.h"
#include "AnimationView.h"
#include "FileInfo.h"
#include "Export_Function.h"


// CAnimationView 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAnimationPage, CPropertyPage)

CAnimationPage::CAnimationPage()
	: CPropertyPage(CAnimationPage::IDD)
{
	m_strSelectName = L"";
}

CAnimationPage::~CAnimationPage()
{
}

void CAnimationPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ListBox);
	DDX_Control(pDX, IDC_COMBO1, m_ObjectType);
}


BEGIN_MESSAGE_MAP(CAnimationPage, CPropertyPage)
	ON_LBN_SELCHANGE(IDC_LIST1, &CAnimationPage::OnLbnSelchangeList)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CAnimationPage::OnCbnSelchangeObjectType)
END_MESSAGE_MAP()


// CAnimationView 메시지 처리기입니다.

BOOL CAnimationPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_pAnimationView = new CAnimationView;

	m_pAnimationView->Create(NULL , L"AnimationView" , WS_CHILD | WS_BORDER | WS_VISIBLE
		, CRect(10 , 150 , 400 , 600) , this , 0);

	Load_MeshList();

	MAPLIST::iterator		iter = m_MeshPathMap.begin();
	MAPLIST::iterator		iter_end = m_MeshPathMap.end();

	for( ; iter != iter_end; ++iter)
		m_ObjectType.AddString(iter->first);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CAnimationPage::OnLbnSelchangeList()
{
	UpdateData(TRUE);

	int iSelectIndex = m_ListBox.GetCurSel();

	if(iSelectIndex < 0)
		return;

	m_ListBox.GetText(iSelectIndex , m_strSelectName);

	TCHAR	szTemp[MAX_PATH];
	lstrcpy(szTemp , m_strSelectName);
	
	m_pAnimationView->SetName(szTemp);

	UpdateData(FALSE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CAnimationPage::OnCbnSelchangeObjectType()
{
	UpdateData(TRUE);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString	strTemp;
	m_iSelectComboNum = m_ObjectType.GetCurSel();

	m_ObjectType.GetLBText(m_iSelectComboNum, strTemp);

	m_ListBox.ResetContent();

	TCHAR      szGroupTag[64] = L"";
	lstrcpy(szGroupTag, (LPWSTR)strTemp.operator LPCWSTR());   

	Set_MeshList( Find_MeshTag(szGroupTag) );

	UpdateData(FALSE);
}

void CAnimationPage::Set_MeshList(list<const TCHAR*>* pPathList)
{
	LISTPATH::iterator      iter     = pPathList->begin();
	LISTPATH::iterator      iter_end = pPathList->end();

	for( ; iter != iter_end; ++iter)
		m_ListBox.AddString( (*iter) );
}

list<const TCHAR*>* CAnimationPage::Find_MeshTag(const TCHAR* pGroupTag)		//
{
	MAPLIST::iterator   Tagiter = find_if( m_MeshPathMap.begin(), m_MeshPathMap.end(), Engine::CTag_Finder( pGroupTag ) );

	if( Tagiter == m_MeshPathMap.end() )
		return NULL;

	return &Tagiter->second;
}

void CAnimationPage::Load_MeshList(void)
{			//											저장
	Find_MeshFilePath(L"\\Tool\\Bin\\Resource\\DynamicMesh\\Object",        L"\\Data\\Path\\DynamicPath.txt",      m_ObjectPathList);      // Character List
//	Find_MeshFilePath(L"\\Tool\\Bin\\Resource\\Mesh\\Object",        L"\\Data\\Path\\ObjectPath.txt",      m_ObjectPathList);      // Character List

	
	Set_MeshTagList( &m_ObjectPathList, &m_MeshPathMap);
}

void CAnimationPage::Find_MeshFilePath(const TCHAR* pFilePath , const TCHAR* pSavetxtpath , PATHLIST& rPathList)
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

void CAnimationPage::Set_MeshTagList(PATHLIST* pPathList , MAPLIST* pMapTag)	//초기화 할때 메쉬리스트
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


void CAnimationPage::Save_MeshPath(const TCHAR* pSavetxtPath , PATHLIST& rPathList)
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

void CAnimationPage::LoadMesh(const TCHAR* pMeshTag, const TCHAR* pFullPath ,const TCHAR* pFileTag)						//Mesh Load
{
	if(FAILED(Engine::Ready_Mesh(Engine::GetGraphicDev() , RESOURCE_STAGE, Engine::CMesh::TYPE_DYNAMIC , pMeshTag, pFullPath ,pFileTag )))
	{
		::msg_box("Ready_Mesh FAILED");
		return;
	}
}