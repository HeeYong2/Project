#include "Frustum.h"

using namespace Engine;

IMPLEMENT_SINGLETON(CFrustum)
//------------------------------------------------------------------------------------------------------------------------

CFrustum::CFrustum(void)
{
	ZeroMemory(&m_vOriginal_Point , sizeof(_vec3) * 8);
	ZeroMemory(&m_vConvert_Point , sizeof(_vec3) * 8);
}
//------------------------------------------------------------------------------------------------------------------------

CFrustum::~CFrustum(void)
{

}
//------------------------------------------------------------------------------------------------------------------------

HRESULT CFrustum::Init_Frustum(LPDIRECT3DDEVICE9 pGraphicDev)									//8개의 평면 생성과 영역 변환
{
	m_pGraphicDev = pGraphicDev;
	m_vOriginal_Point[0] = _vec3(-1.f , 1.f , 0.f);
	m_vOriginal_Point[1] = _vec3(1.f , 1.f , 0.f);
	m_vOriginal_Point[2] = _vec3(1.f , -1.f , 0.f);
	m_vOriginal_Point[3] = _vec3(-1.f , -1.f , 0.f);
	m_vOriginal_Point[4] = _vec3(-1.f , 1.f , 1.f);
	m_vOriginal_Point[5] = _vec3(1.f , 1.f , 1.f);
	m_vOriginal_Point[6] = _vec3(1.f , -1.f , 1.f);
	m_vOriginal_Point[7] = _vec3(-1.f , -1.f , 1.f); 

	_matrix		matProj;
	pGraphicDev->GetTransform(D3DTS_PROJECTION , &matProj);

	D3DXMatrixInverse(&matProj , NULL , &matProj);

	for(_int i = 0 ; i < 8; ++i)
		D3DXVec3TransformCoord(&m_vOriginal_Point[i] , &m_vOriginal_Point[i] , &matProj);

	return NULL;
}
//------------------------------------------------------------------------------------------------------------------------

void CFrustum::ViewToWorld(LPDIRECT3DDEVICE9 pGraphicDev , _bool bMakPlane)						//영역 변환
{
	memcpy(m_vConvert_Point, m_vOriginal_Point, sizeof(_vec3) * 8);

	_matrix				matView;
	pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	for (_int i = 0; i < 8; ++i)	
		D3DXVec3TransformCoord(&m_vConvert_Point[i], &m_vConvert_Point[i], &matView);

	if(bMakPlane == true)
		Make_Plane();

}
//------------------------------------------------------------------------------------------------------------------------
void CFrustum::WorldToLocal(LPDIRECT3DDEVICE9 pGraphicDev , const _matrix* pMatWorld , _bool bMakePlane)	
{
	_matrix		matWorldInv;
	D3DXMatrixInverse(&matWorldInv , NULL , pMatWorld);

	for(_int i = 0 ; i < 8 ; ++i)
		D3DXVec3TransformCoord(&m_vConvert_Point[i] , &m_vConvert_Point[i] , &matWorldInv);

	if(bMakePlane == true)
		Make_Plane();
}

//------------------------------------------------------------------------------------------------------------------------

void CFrustum::Culling_ForTerrain(const Engine::VTXTEX* pTerrainVtxInfo, const _ushort& wCntX
								  , const _ushort& wCntZ, Engine::INDEX32* pIndex, _ulong* pTriCnt, const _matrix* pWorld)
{
	if(NULL == pIndex)
		return;

	*pTriCnt = 0;

	_int		iIndex = 0;
	_bool		isIn[4] = {false};

	for	(_int z = 0; z < wCntZ - 1; ++z)
	{
		for (_int x = 0; x < wCntX - 1; ++x)
		{
			iIndex = z * wCntX + x;

			isIn[0] = VertexInFrustum(&pTerrainVtxInfo[iIndex + wCntX].vPosition);
			isIn[1] = VertexInFrustum(&pTerrainVtxInfo[iIndex + wCntX + 1].vPosition);
			isIn[2] = VertexInFrustum(&pTerrainVtxInfo[iIndex + 1].vPosition);
			isIn[3] = VertexInFrustum(&pTerrainVtxInfo[iIndex].vPosition);

			if(true == isIn[0]
			|| true == isIn[1]
			|| true == isIn[2])
			{
				pIndex[*pTriCnt]._1 = iIndex + wCntX;
				pIndex[*pTriCnt]._2 = iIndex + wCntX + 1;
				pIndex[*pTriCnt]._3 = iIndex + 1;
				++(*pTriCnt);
			}

			if(true == isIn[0]
			|| true == isIn[2]
			|| true == isIn[3])
			{
				pIndex[*pTriCnt]._1 = iIndex + wCntX;
				pIndex[*pTriCnt]._2 = iIndex + 1;
				pIndex[*pTriCnt]._3 = iIndex;
				++(*pTriCnt);
			}			
		}
	}
}

bool Engine::CFrustum::Culling_ForObject(_float fRadius , _vec3 vPosition)
{
	bool isIn = false;
	
	isIn = ObjectInSphere(&vPosition , fRadius);		//절두체 컬링이 되면

	return isIn;
}
//------------------------------------------------------------------------------------------------------------------------

void CFrustum::Make_Plane(void)
{
	D3DXPlaneFromPoints(&m_Plane[0], &m_vConvert_Point[1], &m_vConvert_Point[5], &m_vConvert_Point[6]);
	D3DXPlaneFromPoints(&m_Plane[1], &m_vConvert_Point[4], &m_vConvert_Point[0], &m_vConvert_Point[3]);
	D3DXPlaneFromPoints(&m_Plane[2], &m_vConvert_Point[4], &m_vConvert_Point[5], &m_vConvert_Point[1]);
	D3DXPlaneFromPoints(&m_Plane[3], &m_vConvert_Point[3], &m_vConvert_Point[2], &m_vConvert_Point[6]);
	D3DXPlaneFromPoints(&m_Plane[4], &m_vConvert_Point[7], &m_vConvert_Point[6], &m_vConvert_Point[5]);
	D3DXPlaneFromPoints(&m_Plane[5], &m_vConvert_Point[0], &m_vConvert_Point[1], &m_vConvert_Point[2]);
}

//------------------------------------------------------------------------------------------------------------------------

bool CFrustum::VertexInFrustum(const _vec3* pVtxPos)
{
	for(_int Plane_i = 0; Plane_i < 6; ++Plane_i)
	{
		_float fDistance = D3DXPlaneDotCoord(&m_Plane[Plane_i] , pVtxPos);

		if(fDistance > 0.f) //절두체에 포함이 안되어 있다 -> 즉 출력하지 않아야 한다
			return false;
	}
	return true;
}
//------------------------------------------------------------------------------------------------------------------------

bool CFrustum::SphereInFrustum(const _vec3* pCenterPos , const float& fRadius)
{
	for(_int Plane_i = 0 ; Plane_i < 6; ++Plane_i)
	{
		_float fDistance = D3DXPlaneDotCoord(&m_Plane[Plane_i] , pCenterPos);

		if(fDistance > fRadius)	//절두체에 포함이 안되어 있다 -> 즉 출력하지 않아야 한다
			return false;
	}
	return true;
}

bool CFrustum::ObjectInSphere(const _vec3* pCenterPos , const float& fRadius)
{
	for(_int Plane_i = 0 ; Plane_i < 6; ++Plane_i)
	{
		_float fDistance = D3DXPlaneDotCoord(&m_Plane[Plane_i] , pCenterPos);

		if(fDistance > fRadius)	//절두체에 포함이 안되어 있다 -> 즉 출력하지 않아야 한다
			return false;
	}
	return true;
}