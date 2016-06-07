// ObjectView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Tool.h"
#include "ObjectView.h"
#include "Export_Function.h"


// CObjectView

IMPLEMENT_DYNCREATE(CObjectView, CView)

CObjectView::CObjectView()
: m_iSizeX(300)
, m_iSizeY(300)
, m_isPass(false)
, m_isOk(false)
{
	ZeroMemory(m_szName , sizeof(TCHAR) * MAX_PATH);
	m_vScale = _vec3(0.01f, 0.01f, 0.01f);
	ZeroMemory(&m_vPosition, sizeof(_vec3));
	ZeroMemory(m_fAngle, sizeof(_float ) * ANGLE_END);
	memcpy(&m_vDirection ,  g_vLook , sizeof(_vec3));

	D3DXMatrixIdentity(&m_matWorld);
}

CObjectView::~CObjectView()
{
	Release();
}

BEGIN_MESSAGE_MAP(CObjectView, CView)
END_MESSAGE_MAP()


// CObjectView �׸����Դϴ�.

void CObjectView::OnDraw(CDC* pDC)
{
 	if(m_isPass == false)
 		return;

	CDocument* pDoc = GetDocument();

	Render();
	Update();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
	Invalidate(FALSE);
}


// CObjectView �����Դϴ�.

#ifdef _DEBUG
void CObjectView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CObjectView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CObjectView �޽��� ó�����Դϴ�.

void CObjectView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_isPass = true;

	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectView::Render(void)
{
	if(m_isOk == false)
		return;
	
	AddComponent(m_szName);
	m_isOk = false;
		
	Engine::GetGraphicDev()->Clear(0 , NULL , D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL ,D3DCOLOR_ARGB(255 , 163 , 204 , 163) , 1.f , 0);
	Engine::GetGraphicDev()->BeginScene();

 	_matrix matView , matProj , matOriView;
 
  	Engine::GetGraphicDev() ->GetTransform(D3DTS_VIEW , &matView);
  	matOriView = matView;
  	D3DXMatrixInverse(&matView , NULL , &matView);
 	D3DXMatrixLookAtLH(&matView, &_vec3(0.f, 10.f, -60.f), &_vec3(0.f, 0.0f, 0.0f), &_vec3(0.f, 1.f, 0.f));
	//Projection�� �ؾ߉�
 	Engine::GetGraphicDev() ->SetTransform(D3DTS_VIEW , &matView);
 	((Engine::CMesh*)m_pMeshCom)->Render(Engine::GetGraphicDev() , &m_pInfo->m_matWorld );
 	((Engine::CVIBuffer*)m_pBufferCom)->Render(Engine::GetGraphicDev());


	Engine::GetGraphicDev()->EndScene();
	Engine::GetGraphicDev()->Present(NULL , NULL , m_hWnd, NULL);

	Engine::GetGraphicDev() ->SetTransform(D3DTS_VIEW , &matOriView);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectView::Update(void)
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
HRESULT CObjectView::AddComponent(TCHAR* szName)
{
	Engine::CComponent*		pComponent = NULL;
	//= For.Transform =======================================================================
	pComponent = m_pInfo = Engine::CTransform::Create(&g_vLook);
	if(NULL == pComponent)
		return E_FAIL;

	//= For.Mesh =======================================================================
	pComponent = m_pMeshCom = Engine::Clone_Resource(RESOURCE_STATIC, szName);
	if(NULL == pComponent)
		return E_FAIL;

	// For.Buffer ===============================================================================================================================================================================================================================================================================================================================================
	pComponent = m_pBufferCom = Engine::Clone_Resource(RESOURCE_STATIC, L"RcTex");
	if(NULL == pComponent)
		return E_FAIL;

	return S_OK;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectView::SetName(TCHAR* szpName)
{
	TCHAR	szTemp[MAX_PATH] = L"";

	if(!lstrcmp(szTemp , szpName))		//�� ���ڿ��� ������ 0�� ��ȯ
		m_isOk = false;
	else
	{
		lstrcpy(m_szName ,szpName);
		m_isOk = true;
 	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectView::Release(void)
{
	Engine::safe_delete(m_pBufferCom);
	Engine::safe_delete(m_pMeshCom);
}