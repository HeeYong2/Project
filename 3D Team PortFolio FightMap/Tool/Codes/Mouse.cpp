#include "stdafx.h"
#include "Mouse.h"
#include "ToolView.h"
#include "Export_Function.h"

IMPLEMENT_SINGLETON(CMouse)

CMouse::CMouse(void)
: m_iVtxSizeX(0)
, m_iVtxSizeZ(0)
, m_iVtxItv(1)
{

}
//------------------------------------------------------------------------------------------------------------------------------

CMouse::~CMouse(void)
{

}
//------------------------------------------------------------------------------------------------------------------------------

_int CMouse::PickTerrain(D3DXVECTOR3* pOut )
{
	POINT		ptMouse;
	GetMousePos_ViewPort(&ptMouse);
	Translation_ViewSpace(&ptMouse);

	_matrix		matLocal;
	D3DXMatrixIdentity(&matLocal);
	Translation_Local(&matLocal);

	if(m_pVtxTex == NULL)
		return NULL;

	int		iIndex = 0;

	float	fU, fV, fDist;

	for(int z = 0; z < m_iVtxSizeZ - 1; ++z)
	{
		for(int x = 0; x < m_iVtxSizeX - 1; ++x)
		{
			iIndex = z * m_iVtxSizeX + x;

			if(D3DXIntersectTri(&m_pVtxTex[iIndex + m_iVtxSizeX + 1].vPosition, &m_pVtxTex[iIndex + m_iVtxSizeX].vPosition
				, &m_pVtxTex[iIndex + 1].vPosition, &m_vPivotPos, &m_vRayDir, &fU, &fV, &fDist))
			{
				*pOut = m_pVtxTex[iIndex + m_iVtxSizeX + 1].vPosition
					+ (m_pVtxTex[iIndex + m_iVtxSizeX].vPosition - m_pVtxTex[iIndex + m_iVtxSizeX + 1].vPosition) * fU
					+ (m_pVtxTex[iIndex + 1].vPosition - m_pVtxTex[iIndex + m_iVtxSizeX + 1].vPosition) * fV;

				return iIndex;
			}

			else if(D3DXIntersectTri(&m_pVtxTex[iIndex].vPosition, &m_pVtxTex[iIndex + 1].vPosition
				, &m_pVtxTex[iIndex + m_iVtxSizeX].vPosition, &m_vPivotPos, &m_vRayDir, &fU, &fV, &fDist))
			{
				*pOut = m_pVtxTex[iIndex].vPosition
					+ (m_pVtxTex[iIndex + 1].vPosition - m_pVtxTex[iIndex].vPosition) * fU
					+ (m_pVtxTex[iIndex + m_iVtxSizeX].vPosition - m_pVtxTex[iIndex].vPosition) * fV;

				return iIndex;;
			}
		}
	}
	return -1;
}

//------------------------------------------------------------------------------------------------------------------------------

void CMouse::GetTerrainInfo(_vec3* pOut )
{
	POINT		ptMouse;
	GetMousePos_ViewPort(&ptMouse);
	Translation_ViewSpace(&ptMouse);

	_matrix		matLocal;
	D3DXMatrixIdentity(&matLocal);
	Translation_Local(&matLocal);

	int		iIndex = 0;

	float	fU, fV, fDist;

	for(int z = 0; z < m_iVtxSizeZ - 1; ++z)
	{
		for(int x = 0; x < m_iVtxSizeX - 1; ++x)
		{
			iIndex = z * m_iVtxSizeX + x;

			if(D3DXIntersectTri(&m_pVtxTex[iIndex + m_iVtxSizeX + 1].vPosition, &m_pVtxTex[iIndex + m_iVtxSizeX].vPosition
				, &m_pVtxTex[iIndex + 1].vPosition, &m_vPivotPos, &m_vRayDir, &fU, &fV, &fDist))
			{
				*pOut = m_pVtxTex[iIndex + m_iVtxSizeX + 1].vPosition
					+ (m_pVtxTex[iIndex + m_iVtxSizeX].vPosition - m_pVtxTex[iIndex + m_iVtxSizeX + 1].vPosition) * fU
					+ (m_pVtxTex[iIndex + 1].vPosition - m_pVtxTex[iIndex + m_iVtxSizeX + 1].vPosition) * fV;

				return ;
			}

			else if(D3DXIntersectTri(&m_pVtxTex[iIndex].vPosition, &m_pVtxTex[iIndex + 1].vPosition
				, &m_pVtxTex[iIndex + m_iVtxSizeX].vPosition, &m_vPivotPos, &m_vRayDir, &fU, &fV, &fDist))
			{
				*pOut = m_pVtxTex[iIndex].vPosition
					+ (m_pVtxTex[iIndex + 1].vPosition - m_pVtxTex[iIndex].vPosition) * fU
					+ (m_pVtxTex[iIndex + m_iVtxSizeX].vPosition - m_pVtxTex[iIndex].vPosition) * fV;

				return ;
			}
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------------

void CMouse::GetMousePos_ViewPort(POINT* pPoint)
{
	GetCursorPos(pPoint);
	ScreenToClient(g_hWnd, pPoint);
}
//------------------------------------------------------------------------------------------------------------------------------

void CMouse::Translation_ViewSpace(const POINT* pPoint)
{
	D3DXMATRIX		matProj;
	Engine::GetGraphicDev()->GetTransform(D3DTS_PROJECTION , &matProj);

	D3DXVECTOR3		vTemp;
	vTemp.x = (pPoint->x / (WINSIZEX / 2.f) - 1.f) / matProj._11;
	vTemp.y = (-pPoint->y / (WINSIZEY / 2.f) + 1.f) / matProj._22;
	vTemp.z = 1.f;

	m_vPivotPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vRayDir = vTemp - m_vPivotPos;
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
}

//------------------------------------------------------------------------------------------------------------------------------

void CMouse::Translation_Local(const D3DXMATRIX* pMatrix)
{
	D3DXMATRIX		matView;
	Engine::GetGraphicDev()->GetTransform(D3DTS_VIEW , &matView);

	D3DXMatrixInverse(&matView, 0, &matView);

	D3DXVec3TransformCoord(&m_vPivotPos, &m_vPivotPos, &matView);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matView);

	D3DXMATRIX		matWorldInv;

	D3DXMatrixInverse(&matWorldInv, 0, pMatrix);

	D3DXVec3TransformCoord(&m_vPivotPos, &m_vPivotPos, &matWorldInv);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matWorldInv);
}
//------------------------------------------------------------------------------------------------------------------------------

void CMouse::SetVtx(int VtxSizeX, int VtxSizeZ , int VtxItv)
{
	m_iVtxSizeX = VtxSizeX;
	m_iVtxSizeZ = VtxSizeZ;
	m_iVtxItv   = VtxItv;
}
//------------------------------------------------------------------------------------------------------------------------------

void CMouse::SetVtxTex(Engine::VTXTEX* pVtxTex)
{
	m_pVtxTex = pVtxTex;
}
//------------------------------------------------------------------------------------------------------------------------------

_bool CMouse::PickObject(_matrix* pMatrix , const Engine::VTXTEX* pVtxTex)
{
	_matrix matWorld = *pMatrix;

	POINT	ptMouse;
	GetMousePos_ViewPort(&ptMouse);		//Mouse 가져오기
	Translation_ViewSpace(&ptMouse);	//Mouse 변환
	Translation_Local(&matWorld);		//Local 변환

 	_float	fU , fV , fDist;

	if(D3DXIntersectTri(&pVtxTex[0].vPosition , &pVtxTex[1].vPosition , &pVtxTex[2].vPosition
		, &m_vPivotPos , &m_vRayDir , &fU , &fV , &fDist))
	{
		_vec3 vPoint =				  pVtxTex[1].vPosition
			+ (pVtxTex[0].vPosition - pVtxTex[1].vPosition) * fU
			+ (pVtxTex[2].vPosition - pVtxTex[1].vPosition) * fV;
		
		return true;
	}

	if(D3DXIntersectTri(&pVtxTex[0].vPosition , &pVtxTex[2].vPosition , &pVtxTex[3].vPosition
		, &m_vPivotPos , &m_vRayDir , &fU , &fV , &fDist))
	{
		_vec3 vPoint =				  pVtxTex[3].vPosition
			+ (pVtxTex[0].vPosition - pVtxTex[3].vPosition) * fU
			+ (pVtxTex[2].vPosition - pVtxTex[3].vPosition) * fV;

		return true;
	}
	return false;
}

//------------------------------------------------------------------------------------------------------------------------------
_bool CMouse::PickMesh(_matrix* pmatWorld, LPD3DXBASEMESH pMesh, _ulong dwWinCX, _ulong dwWinCY)			//메시피킹
{
	POINT	ptMouse;
	GetMousePos_ViewPort( &ptMouse );
	Translation_ViewSpace( &ptMouse );

	BOOL	bCheckHit = FALSE;
	_ulong	dwFaceIdx = 0;
	_float	fU, fV, fDist;

	Translation_Local(pmatWorld);

	D3DXIntersect(pMesh, &m_vPivotPos, &m_vRayDir, &bCheckHit, &dwFaceIdx, &fU, &fV, &fDist, NULL, NULL);

	if(TRUE == bCheckHit)
		return TRUE;

	return FALSE;
}

//------------------------------------------------------------------------------------------------------------------------------
_bool CMouse::GetPickMeshPos( _vec3* pPickPos, _matrix* pmatWorld, LPD3DXBASEMESH pMesh, _ulong dwWinCX, _ulong dwWinCY)//메시의 로컬좌표를 월드좌표로 변환
{
	POINT	ptMouse;
	GetMousePos_ViewPort( &ptMouse );
	Translation_ViewSpace( &ptMouse );

	BOOL	bCheckHit = FALSE;
	_ulong	dwFaceIdx = 0;
	_float	fU, fV, fDist;

	Translation_Local(pmatWorld);

	D3DXIntersect(pMesh, &m_vPivotPos, &m_vRayDir, &bCheckHit, &dwFaceIdx, &fU, &fV, &fDist, NULL, NULL);

	D3DXVec3TransformCoord(&m_vPivotPos, &m_vPivotPos, pmatWorld);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, pmatWorld);

	*pPickPos = m_vPivotPos + m_vRayDir * fDist;

	pPickPos->y += 0.02f;

	if(TRUE == bCheckHit)
		return TRUE;

	return FALSE;
}
