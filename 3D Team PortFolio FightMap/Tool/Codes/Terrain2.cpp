#include "stdafx.h"
#include "Terrain2.h"
#include "Mouse.h"
#include "VIBuffer.h"
#include "Export_Function.h"

CTerrain2::CTerrain2(void)
: m_pVtxInfo(NULL)
, m_pIndex(NULL)
, m_iFrameNum(2)
, m_iTriCnt(0)
, m_wResult(0)
, m_iTextureNum(1)
, m_fUVRatio(1.f)
, m_isCreateSplatting(false)
{
	ZeroMemory(m_pAlphaTexture , sizeof(LPDIRECT3DTEXTURE9) * 4);
	memset(&m_tTexInfo , 1 , sizeof(TEXINFO));
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

CTerrain2::~CTerrain2(void)
{
	Release();	//릴리즈 호출을 안한다
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

HRESULT CTerrain2::Initialize(const _uint& wCntX, const _uint& wCntZ, const float& fItv)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_wResult = wCntX * wCntZ;
	m_pVtxInfo = new Engine::VTXTEX[wCntX * wCntZ];
	m_pIndex = new Engine::INDEX32[(wCntX - 1) * (wCntZ - 1) * 2];
	m_VtxSizeX = wCntX;
	m_VtxSizeZ = wCntZ;
	m_fVtxItv = fItv;
	Engine::Throw_Vertex_Info(RESOURCE_STAGE, L"Buffer_Terrain2", m_pVtxInfo);
//	Engine::Init_QudTree(Engine::GetGraphicDev() , m_pVtxInfo , wCntX , wCntZ );
	Engine::Init_Neighbor(Engine::GetParentTree() ,wCntX , wCntZ);

	m_pEffect = Engine::Get_Effect(L"Shader_Terrain");

	for(int i = 0 ; i < 4; ++i)
	{
		m_tTexInfo.iTextureNum[i] = 1;
		m_tTexInfo.fAlpahUV[i] = 1.f;
	}
	return S_OK;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CTerrain2::Update(void)
{
// 	Engine::WorldToLocal(Engine::GetGraphicDev() , &m_pInfo->m_matWorld , true);
// 	Engine::isIn(m_pVtxInfo);
// 	m_iTriCnt = 0;
// 	Engine::Culling_ForTerrain(m_pVtxInfo , m_pIndex , &m_iTriCnt);
	Engine::CGameObject::Update();
	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_DEFERRED, this);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CTerrain2::Render(void)
{
	Set_ConstantTable();

	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);

	Render_Buffer();

	m_pEffect->EndPass();
	m_pEffect->End();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

CTerrain2* CTerrain2::Create(const _uint& wCntX, const _uint& wCntZ, const float& fItv)
{
	CTerrain2* pTerrain2 = new CTerrain2;

	if(FAILED(pTerrain2->Initialize(wCntX , wCntZ , float(fItv))))
	{
		//		safe_delete(pTerrain2Tool);
		msg_box("pTerrain2Tool Create Failed");
		return NULL;
	}
	return pTerrain2;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

HRESULT CTerrain2::Add_Component(void)
{
	Engine::CComponent*		pComponent = NULL;
	//= For.Transform =======================================================================
	pComponent = m_pInfo = Engine::CTransform::Create(&g_vLook);
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOM::value_type(L"Transform", pComponent));

	//= For.Texture =======================================================================
	pComponent = m_pTexture = Engine::Clone_Resource(RESOURCE_STAGE, L"Tool_Texture_Terrain");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOM::value_type(L"Texture", pComponent));

	//= For.Buffer =======================================================================
	pComponent = m_pBuffer = Engine::Clone_Resource(RESOURCE_STAGE, L"Buffer_Terrain2");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOM::value_type(L"Buffer", pComponent));
	return S_OK;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CTerrain2::Render_Texture(void)
{
	m_pTexture->Render(Engine::GetGraphicDev(), m_iTextureNum);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CTerrain2::Render_Buffer(void)
{
	//	((Engine::CVIBuffer*)m_pBuffer)->Receive_Index_Info(m_pIndex, &m_iTriCnt);
	Engine::GetGraphicDev()->SetRenderState(D3DRS_FILLMODE, m_iFrameNum);
	//  D3DFILL_POINT               = 1,
	//  D3DFILL_WIREFRAME           = 2,
	//  D3DFILL_SOLID               = 3,
	//((Engine::CVIBuffer*)m_pBuffer)->Receive_Index_Info(m_pIndex, &m_iTriCnt);
	m_pBuffer->Render(Engine::GetGraphicDev());
	Engine::GetGraphicDev()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}
//------------------------------------------------------------------------------------------------------------------------------
void CTerrain2::Set_ConstantTable(void)
{
	if(NULL == m_pEffect)
		return;

	m_pEffect->SetMatrix("g_matWorld", &m_pInfo->m_matWorld);

	_matrix			matView, matProj;
	Engine::GetGraphicDev()->GetTransform(D3DTS_VIEW, &matView);
	Engine::GetGraphicDev()->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &matProj);

	IDirect3DBaseTexture9*				pTexture = NULL;

	pTexture = ((Engine::CTexture*)m_pTexture)->Get_Texture(m_iTextureNum);				//베이스 텍스처
	m_pEffect->SetTexture("g_BaseSplatting", pTexture);

	pTexture = ((Engine::CTexture*)m_pTexture)->Get_Texture(m_tTexInfo.iTextureNum[0]);				//섞을재료 1
	m_pEffect->SetTexture("g_Mix1Texture", pTexture);

	pTexture = ((Engine::CTexture*)m_pTexture)->Get_Texture(m_tTexInfo.iTextureNum[1]);				//섞을재료 2
	m_pEffect->SetTexture("g_Mix2Texture", pTexture);

	pTexture = ((Engine::CTexture*)m_pTexture)->Get_Texture(m_tTexInfo.iTextureNum[2]);				//섞을재료 3
	m_pEffect->SetTexture("g_Mix3Texture", pTexture);

	pTexture = ((Engine::CTexture*)m_pTexture)->Get_Texture(m_tTexInfo.iTextureNum[3]);				//섞을재료 3
	m_pEffect->SetTexture("g_Mix4Texture", pTexture);

	pTexture = ((Engine::CTexture*)m_pTexture)->Get_Texture(0);				
	m_pEffect->SetTexture("g_Mix1AlphaTexture", m_pAlphaTexture[0]);

	pTexture = ((Engine::CTexture*)m_pTexture)->Get_Texture(0);
	m_pEffect->SetTexture("g_Mix2AlphaTexture", m_pAlphaTexture[1]);

	pTexture = ((Engine::CTexture*)m_pTexture)->Get_Texture(0);
	m_pEffect->SetTexture("g_Mix3AlphaTexture", m_pAlphaTexture[2]);

	pTexture = ((Engine::CTexture*)m_pTexture)->Get_Texture(0);
	m_pEffect->SetTexture("g_Mix4AlphaTexture", m_pAlphaTexture[3]);

	m_pEffect->SetFloat("g_Mix1Ratio" , m_tTexInfo.fAlpahUV[0]);
	m_pEffect->SetFloat("g_Mix2Ratio" , m_tTexInfo.fAlpahUV[1]);
	m_pEffect->SetFloat("g_Mix3Ratio" , m_tTexInfo.fAlpahUV[2]);
	m_pEffect->SetFloat("g_Mix4Ratio" , m_tTexInfo.fAlpahUV[3]);

	m_pEffect->SetFloat("g_fDetail" , m_fUVRatio);							//Detail값을 쉐이더에 넘겨줌
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

Engine::VTXTEX* CTerrain2::GetVtxTex(void)
{
	return m_pVtxInfo;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------
Engine::CComponent* CTerrain2::GetBuffer(void)
{
	return m_pBuffer;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CTerrain2::SetFrameType(_int iFrameNum)
{
	m_iFrameNum = iFrameNum;
}

//------------------------------------------------------------------------------------------------------------------------------
_ulong CTerrain2::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		::safe_delete_array(m_pVtxInfo);
		::safe_delete_array(m_pIndex);
		delete this;
	}
	return dwRefCnt;	
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------

void CTerrain2::SetVerTex(_vec3 vPos)
{
	for(int i = 0 ; i < m_wResult ; ++ i)
		m_pVtxInfo[i].vPosition += vPos;

	((Engine::CVIBuffer*)m_pBuffer)->SetVtxInfo(m_pVtxInfo);
}

WORD CTerrain2::GetVtxSizeX(void)
{
	return m_VtxSizeX;
}

WORD CTerrain2::GetVtxSizeZ(void)
{
	return m_VtxSizeZ;
}

_float CTerrain2::GetVtxItv(void)
{
	return m_fVtxItv;
}

void CTerrain2::SetLoadVerTex(_vec3 vPos)
{
	((Engine::CVIBuffer*)m_pBuffer)->SetVtxInfo(m_pVtxInfo);
}

void CTerrain2::SetTextureNum(_int iTextureNum)
{
	m_iTextureNum = iTextureNum;
}

void CTerrain2::SetUVRatio(_float UVRatio)
{
	m_fUVRatio = UVRatio;
}

void CTerrain2::CreateAlphaMap(void)				//버튼을 눌렀을때 알파맵이 생성되게 한다
{
	if(m_isCreateSplatting == true)
		return;

	m_isCreateSplatting = true;
	for(int iAlpahSize =  0 ; iAlpahSize < 4; ++iAlpahSize)				//일단 4개의 알파맵을 생각하자
	{
		if(FAILED(D3DXCreateTexture( Engine::GetGraphicDev() , m_VtxSizeX , m_VtxSizeZ , 1 , 0 , D3DFMT_A8B8G8R8 , D3DPOOL_MANAGED , &m_pAlphaTexture[iAlpahSize])))
			return ;											//CreateTexture를 이용해서 알파맵을 생성하자

		D3DLOCKED_RECT		Rect;								//RECT변수를 이용 하여 LockRect과 UnlockRect을  사용한다
		m_pAlphaTexture[iAlpahSize]->LockRect(0 , &Rect , NULL , 0);

		for(_int i = 0; i < m_VtxSizeX ; ++i)
		{
			BYTE*	pByte = (BYTE*)Rect.pBits;					//pBits는 void*형이다 그러니까 사용할때 변환해서 사용하자

			pByte = pByte + i * Rect.Pitch;						//Pitch는 int형

			for(_int j = 0; j < m_VtxSizeZ; ++j)
			{
				DWORD*		pPixel	=	(DWORD*)pByte + j;		

				*pPixel = D3DXCOLOR(0.f , 0.f , 0.f , 0.f);		//픽셀의 색을 0으로 초기화하자
			}
		}


		m_pAlphaTexture[iAlpahSize]->UnlockRect(0);
	}
}

LPDIRECT3DTEXTURE9 CTerrain2::GetTexture(_int iNum)
{
	return m_pAlphaTexture[iNum];
}

void CTerrain2::SetMix1num(_int iMix1Num)
{
	m_tTexInfo.iTextureNum[0] = iMix1Num;
}

void CTerrain2::SetMix2num(_int iMix2Num)
{
	m_tTexInfo.iTextureNum[1] = iMix2Num;
}

void CTerrain2::SetMix3num(_int iMix3Num)
{
	m_tTexInfo.iTextureNum[2] = iMix3Num;
}

void CTerrain2::SetMix4num(_int iMix4Num)
{
	m_tTexInfo.iTextureNum[3] = iMix4Num;
}

void CTerrain2::SetMix1Ratio(_float fMix1Ratio)
{
	m_tTexInfo.fAlpahUV[0] = fMix1Ratio;
}

void CTerrain2::SetMix2Ratio(_float fMix2Ratio)
{
	m_tTexInfo.fAlpahUV[1] = fMix2Ratio;
}

void CTerrain2::SetMix3Ratio(_float fMix3Ratio)
{
	m_tTexInfo.fAlpahUV[2] = fMix3Ratio;
}

void CTerrain2::SetMix4Ratio(_float fMix4Ratio)
{
	m_tTexInfo.fAlpahUV[3] = fMix4Ratio;
}

void CTerrain2::SetAlphaTexture(LPDIRECT3DTEXTURE9* ppLoadTexture)
{
	for(int i = 0 ; i < 4 ; ++i)
		m_pAlphaTexture[i] = ppLoadTexture[i];
	m_isCreateSplatting = true;
}

TEXINFO CTerrain2::GetTexInfo(void)
{
	return m_tTexInfo;
}

void CTerrain2::SetTexInfo(TEXINFO tTexInfo)
{
	m_tTexInfo = tTexInfo;
}