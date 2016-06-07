#include "stdafx.h"
#include "ObjectTool.h"
#include "Mesh.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "StaticMesh.h"
#include "..\..\Engine\Resource\Codes\Collider.h"
#include "Export_Function.h"

CObjectTool::CObjectTool(void)
: m_isPickingRender(false)
, m_isColliderRender(false)
, m_fColliderSizeX(1.f)
, m_fColliderSizeY(1.f)
, m_fColliderSizeZ(1.f)
, m_fPickingSizeX(30.f)
, m_fPickingSizeZ(30.f)
, m_isCollision(0)
, m_vCenter(0.f , 0.f , 0.f)
, m_fRadius(0.f)
, m_dwVtxNum(0)
, m_dwVtxByte(0)
, m_iTriCnt(0)
, m_fBig(0)
, m_iFrameNum(3)
, m_pEffect(NULL)
, m_isFrustum(true)
{
	ZeroMemory(m_szBuffer , sizeof(TCHAR) * 32);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

CObjectTool::~CObjectTool(void)
{
	Release();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CObjectTool::Initialize(OBJINFO* tObjectInfo )
{
	if(FAILED(AddComponent(tObjectInfo)))
		return E_FAIL;
	
	lstrcpy(m_szBuffer, tObjectInfo->szName);

	m_pInfo->m_vPosition = tObjectInfo->vPos;
	m_pInfo->m_vScale = tObjectInfo->vScale;
	m_pInfo->m_fAngle[m_pInfo->ANGLE_X] = tObjectInfo->vAngle.x;
	m_pInfo->m_fAngle[m_pInfo->ANGLE_Y] = tObjectInfo->vAngle.y;
	m_pInfo->m_fAngle[m_pInfo->ANGLE_Z] = tObjectInfo->vAngle.z;


	m_pVtxInfo = new Engine::VTXTEX[4];
	Engine::Throw_Vertex_Info(RESOURCE_STATIC, L"RcTex", m_pVtxInfo);

	m_pEffect = Engine::Get_Effect(L"Shader_Default");
	m_pGraphicDev = Engine::GetGraphicDev();

	if(NULL == m_pEffect)
		return E_FAIL;

	HRESULT hr = 0;
	LPD3DXMESH	pMesh = ((Engine::CStaticMesh*)m_pMeshCom)->GetMesh();
	_vec3*	vPos = NULL;
	pMesh->LockVertexBuffer(0 ,(void**)&vPos);
	
	DWORD vGetVTX = pMesh->GetNumVertices();
	DWORD vGetFVF = D3DXGetFVFVertexSize(pMesh->GetFVF());
	hr = D3DXComputeBoundingSphere(vPos , pMesh->GetNumVertices()
		, D3DXGetFVFVertexSize(pMesh->GetFVF())
		, &m_vCenter , &m_fRadius);														//센터와 반지름

   	m_fBig = max(m_pInfo->m_vScale.z, max(m_pInfo->m_vScale.x , m_pInfo->m_vScale.y));  //가장 큰 값을 넣자
   	m_fRadius = m_fRadius * m_fBig;														//m_fBig을 이용하여 m_fRadius를 연산해주자 (연산을 한 radius를 넘겨주자)
 	m_pInfo->Update();																	//업데이트를 미리돌려서 m_pInfo->m_matWorld를 세팅해주자
 


//  	if(!lstrcmp(tObjectInfo->szName , L"Tree13")
//  		|| !lstrcmp(tObjectInfo->szName , L"Tree15")
//  		|| !lstrcmp(tObjectInfo->szName , L"Tree31")
// 		|| !lstrcmp(tObjectInfo->szName , L"Bush10")
// 		|| !lstrcmp(tObjectInfo->szName , L"Bush50")
//  		)
// 
//  	{d
//  		m_pInfo->m_fAngle[m_pInfo->ANGLE_Y] = rand() % 360;
//  		m_ObjectInfo.isInstancing = true;
//  	}
 		
//  	if(!lstrcmp(tObjectInfo->szName , L"Mountain01")
//  		|| !lstrcmp(tObjectInfo->szName , L"Mountain02")
//  		)
//  	{
//  		m_pInfo->m_vScale = _vec3(7.f , 7.f ,7.f );
//  	}

 	D3DXVec3TransformCoord(&m_vCenter, &m_vCenter , &m_pInfo->m_matWorld);			//변환된 스케일링을 통해 m_vCenter로 넣자

	pMesh->UnlockVertexBuffer();

	if( FAILED(hr) )
		return E_FAIL;

	if(tObjectInfo->isNeedUV == true)
		int a= 0;
	if(tObjectInfo->isInstancing== true)
		int a= 0;

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void CObjectTool::Update(void)
{
	m_isFrustum = Engine::Culling_ForObject(m_fRadius , m_vCenter);					//변형된 반지름과 포지션

	if(m_isFrustum == false)
		return;

	Engine::CGameObject::Update();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void CObjectTool::Render(void)
{
	if(m_isFrustum == false)
		return;

	if(m_isPickingRender == true)
	{
		Render_Texture();
		Render_Buffer();
	}

	if(m_isColliderRender == true)
		Render_Collider();

	if(NULL == m_pEffect)
		return;

	SetConstantTable();
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);
	Render_Mesh();
	m_pEffect->EndPass();
	m_pEffect->End();
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
CObjectTool* CObjectTool::Create(OBJINFO* tObjectInfo)
{
	CObjectTool*	pObject = new CObjectTool;

	if(FAILED(pObject->Initialize(tObjectInfo)))
	{
		AfxMessageBox(L"Object Create Failed");
		return NULL;
	}
	return pObject;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CObjectTool::AddComponent(OBJINFO* tObjectInfo )
{
	lstrcpy(m_ObjectInfo.szName , tObjectInfo->szName);
	m_ObjectInfo.isInstancing = tObjectInfo->isInstancing;
	m_ObjectInfo.isNeedUV = tObjectInfo->isNeedUV;
	Engine::CComponent*		pComponent = NULL;
	//= For.Transform =======================================================================
	pComponent = m_pInfo = Engine::CTransform::Create(&g_vLook);
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOM::value_type(L"Transform", pComponent));

	//= For.Mesh =======================================================================
	pComponent = m_pMeshCom = Engine::Clone_Resource(RESOURCE_STATIC, m_ObjectInfo.szName);
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOM::value_type(m_ObjectInfo.szName, pComponent));

	// For.Buffer ===============================================================================================================================================================================================================================================================================================================================================
	pComponent = m_pBufferCom = Engine::Clone_Resource(RESOURCE_STATIC, L"RcTex");
	if(NULL == pComponent)
		return E_FAIL;

	m_mapComponent.insert(MAPCOM::value_type(L"Buffer_RcTex", pComponent));

	// For.Texture ===============================================================================================================================================================================================================================================================================================================================================
	pComponent = m_pTexBlueCom = Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Blue");
	if(NULL == pComponent)
		return E_FAIL;

	m_mapComponent.insert(MAPCOM::value_type(L"Texture_Blue", pComponent));

	pComponent = m_pColliderCom = Engine::Clone_Resource(RESOURCE_STAGE , L"Mesh_Box");
	if(pComponent == NULL)
		return E_FAIL;

	m_mapComponent.insert(MAPCOM::value_type(L"Mesh_Box" , pComponent));

	return S_OK;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
void CObjectTool::Render_Mesh(void)
{
	Engine::GetGraphicDev()->SetRenderState(D3DRS_FILLMODE, m_iFrameNum);
	((Engine::CMesh*)m_pMeshCom)->Render(Engine::GetGraphicDev() , m_pEffect);
	Engine::GetGraphicDev()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectTool::Render_Buffer(void)
{
	Engine::SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	Engine::SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	Engine::SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	m_pBufferCom->Render(Engine::GetGraphicDev());

	Engine::SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	Engine::SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectTool::Render_Texture(void)
{
	_matrix			matTrans ;
	D3DXMatrixTranslation(&matTrans, m_pInfo->m_vPosition.x, m_pInfo->m_vPosition.y, m_pInfo->m_vPosition.z);
	m_BoxmatWorld = matTrans;

	_matrix				matBill, matView;
	Engine::GetGraphicDev()->GetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixIdentity(&matBill);

 	matBill._11 = matView._11;
 	matBill._13 = matView._13;
 	matBill._31 = matView._31;
 	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, NULL, &matBill);
	m_BoxmatWorld = matBill * m_BoxmatWorld;

	Engine::GetGraphicDev()->SetTransform(D3DTS_WORLD , &m_BoxmatWorld);
	((Engine::CTexture*)m_pTexBlueCom)->Render(Engine::GetGraphicDev());
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectTool::Render_Collider(void)
{
	Engine::GetGraphicDev()->SetRenderState(D3DRS_FILLMODE , D3DFILL_WIREFRAME);

	Engine::CCollider::COLLIDER			Collider;
	const Engine::CCollider::COLLIDER*  pCollider = ((Engine::CCollider*)m_pColliderCom)->GetColliderInfo();
	memcpy(&Collider , pCollider , sizeof(Collider));

	_matrix		matScale , matTrans ;
	D3DXMatrixScaling(&matScale , m_fRadius, m_fRadius , m_fRadius );
	D3DXMatrixTranslation(&matTrans , m_vCenter.x, m_vCenter.y, m_vCenter.z);

	Collider.m_matWorld	= matScale * matTrans ;

	((Engine::CMesh*)m_pColliderCom)->Render(Engine::GetGraphicDev() , &Collider.m_matWorld);
	Engine::GetGraphicDev()->SetRenderState(D3DRS_FILLMODE , D3DFILL_SOLID);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectTool::SetPickingRender(_bool isPickingRender)
{
	m_isPickingRender = isPickingRender;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectTool::SetColliderRender(_bool isColliderRender)
{
	m_isColliderRender = isColliderRender;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CObjectTool::SetCollision(_bool isCollision)
{
	m_isCollision = isCollision;
}

_vec3 CObjectTool::GetPos(void)
{
	return m_pInfo->m_vPosition;
}

Engine::VTXTEX* CObjectTool::GetVtxTex(void)
{
	return m_pVtxInfo;
}

void CObjectTool::SetInfo(Engine::CTransform* pTransForm)
{
	m_pInfo->m_fAngle[m_pInfo->ANGLE_X] = pTransForm->m_fAngle[m_pInfo->ANGLE_X];
	m_pInfo->m_fAngle[m_pInfo->ANGLE_Y] = pTransForm->m_fAngle[m_pInfo->ANGLE_Y];
	m_pInfo->m_fAngle[m_pInfo->ANGLE_Z] = pTransForm->m_fAngle[m_pInfo->ANGLE_Z];
	m_pInfo->m_vPosition = pTransForm->m_vPosition;
	m_pInfo->m_vScale = pTransForm->m_vScale;
}

_matrix CObjectTool::GetBoxWorld(void)
{
	return m_BoxmatWorld;
}

Engine::CTransform* CObjectTool::GetInfo(void)
{
	return m_pInfo;
}

OBJINFO* CObjectTool::GetObjInfo(void)
{
	return &m_ObjectInfo;
}

void CObjectTool::SetFrameType(_int iFrameNum)
{
	m_iFrameNum = iFrameNum;
}

Engine::CComponent* CObjectTool::GetMeshCom(void)
{
	return m_pMeshCom;
}

void CObjectTool::SetConstantTable(void)
{
	_matrix matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	m_pEffect->SetMatrix("g_matWorld", &m_pInfo->m_matWorld);
	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &matProj);
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------

_ulong CObjectTool::Release(void)
{
	Engine::safe_delete_array(m_pVtxInfo);
	return 0;
}