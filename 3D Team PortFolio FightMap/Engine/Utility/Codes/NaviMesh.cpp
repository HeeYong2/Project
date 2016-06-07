#include "NaviMesh.h"

using namespace Engine;

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Engine::CNaviMesh::CNaviMesh(void)
: m_dwIndex(0)
{
	ZeroMemory(m_vPoint , sizeof(_vec3) * POINT_END);
	ZeroMemory(m_pNeighbor , sizeof(CNaviMesh*) * NEIGHBOR_END);
	ZeroMemory(m_pLine , sizeof(CLine2D*) * LINE_END);

	ZeroMemory(m_pNeighborPoint , sizeof(_vec3*) * POINT_END);
	ZeroMemory(m_pPointLine , sizeof(CLine2D*) * POINT_END);
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Engine::CNaviMesh::~CNaviMesh(void)
{
	Release();
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
_float Engine::CNaviMesh::GetHeight(_float fX , _float fZ)
{
	return (-m_Plane.a * fX - m_Plane.c * fZ - m_Plane.d) / m_Plane.b;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT Engine::CNaviMesh::Init_Mesh(const _vec3* pPointA , const _vec3* pPointB , const _vec3* pPointC , _uint dwIndex)
{
 	m_vPoint[POINT_A] = *pPointA;
 	m_vPoint[POINT_B] = *pPointB;
 	m_vPoint[POINT_C] = *pPointC;
 
 	m_pLine[LINE_AB] = CLine2D::Create(&_vec2(m_vPoint[POINT_A].x , m_vPoint[POINT_A].z) , &_vec2(m_vPoint[POINT_B].x , m_vPoint[POINT_B].z) );
 	m_pLine[LINE_BC] = CLine2D::Create(&_vec2(m_vPoint[POINT_B].x , m_vPoint[POINT_B].z) , &_vec2(m_vPoint[POINT_C].x , m_vPoint[POINT_C].z) );
 	m_pLine[LINE_CA] = CLine2D::Create(&_vec2(m_vPoint[POINT_C].x , m_vPoint[POINT_C].z) , &_vec2(m_vPoint[POINT_A].x , m_vPoint[POINT_A].z) );

	D3DXPlaneFromPoints(&m_Plane , &m_vPoint[POINT_A] , &m_vPoint[POINT_B] , &m_vPoint[POINT_C]);

	m_tNaviInfo.tVtxCol[POINT_A].vPosition = *pPointA;
	m_tNaviInfo.tVtxCol[POINT_B].vPosition = *pPointB;
	m_tNaviInfo.tVtxCol[POINT_C].vPosition = *pPointC;


	m_tNaviInfo.tVtxCol[0].dwColor = D3DCOLOR_ARGB(126, 100, 0, 0);
	m_tNaviInfo.tVtxCol[1].dwColor = D3DCOLOR_ARGB(126, 0, 100, 0);
	m_tNaviInfo.tVtxCol[2].dwColor = D3DCOLOR_ARGB(126, 0, 0, 100);

	m_tNaviInfo.dwVtxSize = sizeof(Engine::VTXCOL);
	m_tNaviInfo.dwVtxFVF = Engine::VTXFVF_COL;
	m_tNaviInfo.dwVtxCnt = 3;
	m_tNaviInfo.dwTriCnt = 1;
	m_tNaviInfo.dwIdx = dwIndex;

	m_dwIndex = dwIndex;


	if(m_tNaviInfo.tVtxCol[POINT_A].vPosition.x == m_tNaviInfo.tVtxCol[POINT_B].vPosition.x
		&& m_tNaviInfo.tVtxCol[POINT_A].vPosition.y == m_tNaviInfo.tVtxCol[POINT_B].vPosition.y
		&& m_tNaviInfo.tVtxCol[POINT_A].vPosition.z == m_tNaviInfo.tVtxCol[POINT_B].vPosition.z)
	{
		
		if(m_tNaviInfo.tVtxCol[POINT_A].vPosition.x == m_tNaviInfo.tVtxCol[POINT_C].vPosition.x
			&& m_tNaviInfo.tVtxCol[POINT_A].vPosition.y == m_tNaviInfo.tVtxCol[POINT_C].vPosition.y
			&& m_tNaviInfo.tVtxCol[POINT_A].vPosition.z == m_tNaviInfo.tVtxCol[POINT_C].vPosition.z)
		{
			
		}
	}

	return S_OK;
}

void Engine::CNaviMesh::Init_Normal(void)
{
	_vec3		vTemp, vTempNormal;
	vTempNormal.y = 0.f;
	vTemp = m_tNaviInfo.tVtxCol[POINT_B].vPosition - m_tNaviInfo.tVtxCol[POINT_A].vPosition;
	vTempNormal.x = -vTemp.z;
	vTempNormal.z = vTemp.x;
	D3DXVec3Normalize(&vTempNormal, &vTempNormal);
	m_vtagNormal[NORMAL_AB] = vTempNormal;

	vTemp = m_tNaviInfo.tVtxCol[POINT_C].vPosition - m_tNaviInfo.tVtxCol[POINT_B].vPosition;
	vTempNormal.x = -vTemp.z;
	vTempNormal.z = vTemp.x;
	D3DXVec3Normalize(&vTempNormal, &vTempNormal);
	m_vtagNormal[NORMAL_BC] = vTempNormal;

	vTemp = m_tNaviInfo.tVtxCol[POINT_A].vPosition - m_tNaviInfo.tVtxCol[POINT_C].vPosition;
	vTempNormal.x = -vTemp.z;
	vTempNormal.z = vTemp.x;
	D3DXVec3Normalize(&vTempNormal, &vTempNormal);
	m_vtagNormal[NORMAL_CA] = vTempNormal;
}

_bool Engine::CNaviMesh::Init_Neighbor_Point(POINT ePoint , const _vec3* pPointA , const _vec3* pPointB , const _vec3* pPointC)
{
	if( m_tNaviInfo.tVtxCol[ePoint].vPosition == *pPointA)
	{
		m_pNeighborPoint[ePoint] = const_cast<_vec3*>(pPointA);
		return true;
	}
	if( m_tNaviInfo.tVtxCol[ePoint].vPosition == *pPointB)
	{
		m_pNeighborPoint[ePoint] = const_cast<_vec3*>(pPointB);
		return true;
	}
	if( m_tNaviInfo.tVtxCol[ePoint].vPosition == *pPointC)
	{
		m_pNeighborPoint[ePoint] = const_cast<_vec3*>(pPointC);
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Engine::CNaviMesh::Compare_Point(const _vec3* pPointA , const _vec3* pPointB , CNaviMesh* pNeighbor)
{
	if((*pPointA ) == m_vPoint[POINT_A])
	{
		if((*pPointB) == m_vPoint[POINT_B])
		{
			m_pNeighbor[NEIGHBOR_AB] = pNeighbor;
			return true;
		}
		else if((*pPointB) == m_vPoint[POINT_C])
		{
			m_pNeighbor[NEIGHBOR_CA] = pNeighbor;
			return true;
		}
	}
	else if((*pPointA ) == m_vPoint[POINT_B])
	{
		if((*pPointB) == m_vPoint[POINT_A])
		{
			m_pNeighbor[NEIGHBOR_AB] = pNeighbor;
			return true;
		}
		else if((*pPointB) == m_vPoint[POINT_C])
		{
			m_pNeighbor[NEIGHBOR_BC] = pNeighbor;
			return true;
		}
	}
	else if((*pPointA ) == m_vPoint[POINT_C])
	{
		if((*pPointB) == m_vPoint[POINT_A])
		{
			m_pNeighbor[NEIGHBOR_CA] = pNeighbor;
			return true;
		}
		else if((*pPointB) == m_vPoint[POINT_B])
		{
			m_pNeighbor[NEIGHBOR_BC] = pNeighbor;
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CNaviMesh::RESULT CNaviMesh::CheckPass(const _vec3* pPointion , const _vec3* pDirection , _ulong& dwIndex)
{
	_int	iSumCnt = 0;
	_int	i		= 0;

	for(i = 0; i < LINE_END ; ++i)
	{
		if(m_pLine[i]->isIn(&_vec2(pPointion->x , pPointion->z)
			, &_vec2(pDirection->x , pDirection->z)) == true)
		{
			++iSumCnt;
		}
		else
		{
			//삼각형에 있는지 판단한다
			if(m_pNeighbor[i] == NULL)
			{
				m_vNormal = m_pLine[i]->GetNormal();
				return RESULT_STOP;
			}

			break;
		}
	}
	if(iSumCnt == 3)
	{
		return RESULT_ON;
	}
	
	dwIndex = m_pNeighbor[i]->m_dwIndex;
	return RESULT_NEIGHBOR;
}

void CNaviMesh::Make_Slide(const _vec3* pPosition, const _vec3* pDirection, _vec3* pSlide)
{
	_vec3 vSlide = _vec3(0.f, 0.f, 0.f);
	for(int i = 0; i < LINE_END; ++i)
	{
		if(false ==(m_pLine[i]->isIn(&_vec2(pPosition->x, pPosition->z)
			, &_vec2(pDirection->x, pDirection->z))))
		{
			if(NULL != m_pNeighbor[i])
				continue;

			m_pLine[i]->MakeSlide(&_vec2(pDirection->x, pDirection->z), pSlide);
			break;
		}
	}	
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
CNaviMesh* CNaviMesh::Create(const _vec3* pPointA , const _vec3* pPointB , const _vec3* pPointC , _uint dwIndex)
{
	CNaviMesh*			pNaviMesh = new CNaviMesh;

	if(FAILED(pNaviMesh->Init_Mesh(pPointA , pPointB ,pPointC , dwIndex)))
	{
		Engine::safe_delete(pNaviMesh);
		return NULL;
	}
	pNaviMesh->Init_Normal();
	return pNaviMesh;
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Engine::CNaviMesh::Release(void)
{
	if(NULL != m_pLine[0])
	{
		for(int i = 0 ; i < LINE_END; ++i)
		{
			safe_delete(m_pLine[i]);
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void Engine::CNaviMesh::ReSetNaviPoint(POINT NaviPoint , const _vec3* pPoint)
{
	_vec3 vTemp = *pPoint + _vec3(0.f , 0.01f , 0.f);
	m_vPoint[NaviPoint] = vTemp;
	m_tNaviInfo.tVtxCol[NaviPoint].vPosition = vTemp;
}

void Engine::CNaviMesh::UpDownPoint(POINT NaviPoint , const _vec3* pPoint)
{
	_vec3 vTemp = *pPoint ;
	m_vPoint[NaviPoint] = vTemp;
	m_tNaviInfo.tVtxCol[NaviPoint].vPosition = vTemp;
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void Engine::CNaviMesh::Render(LPDIRECT3DDEVICE9 pGraphicDev, const _matrix* pmatWorld)
{
	pGraphicDev->SetTexture(0, NULL);
	pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	pGraphicDev->SetTransform(D3DTS_WORLD, pmatWorld);
	pGraphicDev->SetFVF( m_tNaviInfo.dwVtxFVF );
	pGraphicDev->DrawPrimitiveUP( D3DPT_TRIANGLELIST, 1, m_tNaviInfo.tVtxCol, m_tNaviInfo.dwVtxSize );
	pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

_vec3 Engine::CNaviMesh::GetNormal(void)
{
	return m_vNormal;
}

bool Engine::CNaviMesh::isInNavi(_vec3* pPos)
{
	_vec3		vTemp = *pPos - m_tNaviInfo.tVtxCol[POINT_A].vPosition;
	//D3DXVec3Normalize(&vTemp, &vTemp);
	float		fTemp = D3DXVec3Dot(&vTemp, &m_vtagNormal[NORMAL_AB]);

	if(fTemp > 0)
		return false;

	vTemp = *pPos - m_tNaviInfo.tVtxCol[POINT_B].vPosition;
	//D3DXVec3Normalize(&vTemp, &vTemp);
	fTemp = D3DXVec3Dot(&vTemp, &m_vtagNormal[NORMAL_BC]);

	if(fTemp > 0)
		return false;

	vTemp = *pPos - m_tNaviInfo.tVtxCol[POINT_C].vPosition;
	//D3DXVec3Normalize(&vTemp, &vTemp);
	fTemp = D3DXVec3Dot(&vTemp, &m_vtagNormal[NORMAL_CA]);

	if(fTemp > 0)
		return false;

	return true;
}

Engine::NAVIMESHINFO Engine::CNaviMesh::GetInfo(void)
{
	return m_tNaviInfo;
}

const Engine::CNaviMesh* Engine::CNaviMesh::Get_Neighbor(CNaviMesh::NEIGHBOR eNeighbor)
{
	return m_pNeighbor[eNeighbor];
}

CLine2D* Engine::CNaviMesh::Get_PointLine(POINT ePoint)
{
	return m_pPointLine[ePoint];
}

const _vec3* Engine::CNaviMesh::Get_Neighbor_Point(POINT ePoint)
{
	return m_pNeighborPoint[ePoint];
}

CLine2D* Engine::CNaviMesh::Get_Line(LINE eLine)
{
	return m_pLine[eLine];
}

void Engine::CNaviMesh::Set_Point_Neighbor(POINT ePoint , _vec3* pPoint)
{
	m_pNeighborPoint[ePoint] = pPoint;
}

void Engine::CNaviMesh::Set_Point_Line(CNaviMesh::POINT ePoint , CLine2D* pPointLine)
{
	m_pPointLine[ePoint] = pPointLine;
}