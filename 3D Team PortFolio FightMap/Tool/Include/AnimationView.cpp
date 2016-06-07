// AnimationView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Tool.h"
#include "AnimationView.h"
#include "Export_Function.h"

// CAnimationView

IMPLEMENT_DYNCREATE(CAnimationView, CView)

CAnimationView::CAnimationView()
{
	D3DXMatrixIdentity(&m_matWorld);
	m_isOk = false;
	m_isPass = false;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

CAnimationView::~CAnimationView()
{
	Release();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

BEGIN_MESSAGE_MAP(CAnimationView, CView)
END_MESSAGE_MAP()


// CAnimationView 그리기입니다.

//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CAnimationView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CAnimationView::OnDraw(CDC* pDC)
{
	if(m_isPass = false)
		return;

	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
	
	Render();
	Update();
	Invalidate(FALSE);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------


// CAnimationView 진단입니다.

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

// CAnimationView 메시지 처리기입니다.


void CAnimationView::Render(void)
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
	//Projection도 해야됌
	Engine::GetGraphicDev() ->SetTransform(D3DTS_VIEW , &matView);
	((Engine::CMesh*)m_pMeshCom)->Render(Engine::GetGraphicDev() , &m_pInfo->m_matWorld );
	((Engine::CVIBuffer*)m_pBufferCom)->Render(Engine::GetGraphicDev());


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

void CAnimationView::SetName(TCHAR* szpName)
{
	TCHAR	szTemp[MAX_PATH] = L"";

	if(!lstrcmp(szTemp , szpName))		//두 문자열이 같으면 0을 반환
		m_isOk = false;
	else
	{
		lstrcpy(m_szName ,szpName);
		m_isOk = true;
	}
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CAnimationView::Release(void)
{
	if(!m_pBufferCom)
		Engine::safe_delete(m_pBufferCom);
	if(!m_pMeshCom)
		Engine::safe_delete(m_pMeshCom);
}