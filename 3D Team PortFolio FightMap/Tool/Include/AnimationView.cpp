// AnimationView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "AnimationView.h"
#include "Export_Function.h"
#include "DynamicMesh.h"

// CAnimationView

IMPLEMENT_DYNCREATE(CAnimationView, CView)

CAnimationView::CAnimationView()
{
	D3DXMatrixIdentity(&m_matWorld);
	m_isOk = false;
	m_isPass = false;
	m_pMeshCom = NULL;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

CAnimationView::~CAnimationView()
{
	Release();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CAnimationView, CView)
END_MESSAGE_MAP()


// CAnimationView �׸����Դϴ�.

//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CAnimationView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CAnimationView::OnDraw(CDC* pDC)
{
	if(m_isPass = false)
		return;

	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
	
	Render();
	Update();
	Invalidate(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------


// CAnimationView �����Դϴ�.

#ifdef _DEBUG
void CAnimationView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CAnimationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

//--------------------------------------------------------------------------------------------------------------------------------------------------------

// CAnimationView �޽��� ó�����Դϴ�.


void CAnimationView::Render(void)
{
	//if(m_isOk == false)
	//	return;
	//AddComponent(m_szName);

	Engine::GetGraphicDev()->Clear(0 , NULL , D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL ,D3DCOLOR_ARGB(255 , 163 , 204 , 163) , 1.f , 0);
	Engine::GetGraphicDev()->BeginScene();

	_matrix matView , matProj , matOriView;

	Engine::GetGraphicDev() ->GetTransform(D3DTS_VIEW , &matView);
	matOriView = matView;
	D3DXMatrixInverse(&matView , NULL , &matView);
	D3DXMatrixLookAtLH(&matView, &_vec3(3.f, 3.f, -3.f), &_vec3(0.f, 0.0f, 0.0f), &_vec3(0.f, 1.f, 0.f));
	//Projection�� �ؾ߉�
	Engine::GetGraphicDev() ->SetTransform(D3DTS_VIEW , &matView);

	/*float iDat = Engine::GetTime();*/
	if(NULL != m_pMeshCom)
	{
		Engine::SetRenderState(D3DRS_CULLMODE , D3DCULL_NONE);
		((Engine::CDynamicMesh*)m_pMeshCom)->FrameMove(0.0015);
		((Engine::CMesh*)m_pMeshCom)->Render(Engine::GetGraphicDev(), &m_pInfo->m_matWorld);
	}

	Engine::GetGraphicDev()->EndScene();
	Engine::GetGraphicDev()->Present(NULL , NULL , m_hWnd, NULL);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CAnimationView::Update(void)
{
	_matrix			matScale, matRotX, matRotY, matRotZ, matTrans;

	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, m_vScale.z);
	D3DXMatrixRotationX(&matRotX, m_fAngle[ANGLE_X]);
	D3DXMatrixRotationY(&matRotY, m_fAngle[ANGLE_Y]);
	D3DXMatrixRotationZ(&matRotZ, m_fAngle[ANGLE_Z]);

	D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, m_vPosition.z);

	m_matWorld = matScale * matRotX * matRotY * matRotZ * matTrans;
	Engine::GetGraphicDev() ->SetTransform(D3DTS_WORLD , &m_matWorld);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

HRESULT	CAnimationView::AddComponent(TCHAR*		szName)
{
	Engine::CComponent*		pComponent = NULL;
	//= For.Transform =======================================================================
	pComponent = m_pInfo = Engine::CTransform::Create(&g_vLook);
	if(NULL == pComponent)
		return E_FAIL;

	//= For.Mesh =======================================================================
	pComponent = m_pMeshCom = Engine::Clone_Resource(RESOURCE_STAGE, szName);
	if(NULL == pComponent)
		return E_FAIL;

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CAnimationView::SetName(TCHAR* szpName, CListBox* pListBox)
{
	TCHAR	szTemp[MAX_PATH] = L"";

	if(!lstrcmp(szTemp , szpName))		//�� ���ڿ��� ������ 0�� ��ȯ
		return;
	else
	{
		lstrcpy(m_szName ,szpName);
		AddComponent(szpName);


		pListBox->ResetContent();
		_uint iMaxNum = ((Engine::CDynamicMesh*)m_pMeshCom)->GetAniMaxCnt();


		for(_uint i = 0; i < iMaxNum; ++i)
		{
			CString Tempstr;					// ���� - ���� ��
			Tempstr.Format(_T("%d"), i);
			pListBox->AddString(Tempstr);
		}
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CAnimationView::Release(void)
{
	if(!m_pBufferCom)
		Engine::safe_delete(m_pBufferCom);
}