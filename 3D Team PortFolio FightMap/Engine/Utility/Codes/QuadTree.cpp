#include "QuadTree.h"
#include "Frustum.h"

USING(Engine)

IMPLEMENT_SINGLETON(CQuadTree)

//------------------------------------------------------------------------------------------------------------------------
Engine::CQuadTree::CQuadTree(void)
: m_iCenter(0)	
, m_fRadius(0.f)
, m_isIn(false)
, m_pGraphicDev(NULL)
{
	ZeroMemory(m_iCorner, sizeof(_int) * CORNER_END);
	ZeroMemory(m_pChild, sizeof(CQuadTree*) * CHILD_END);
	ZeroMemory(m_pNeighbor, sizeof(CQuadTree*) * NEIGHBOR_END);
}
//------------------------------------------------------------------------------------------------------------------------
Engine::CQuadTree::~CQuadTree(void)
{
	Release();
}

//------------------------------------------------------------------------------------------------------------------------
HRESULT CQuadTree::Init_QuadTree(LPDIRECT3DDEVICE9 pGraphicDev, Engine::VTXTEX* pTerrainVtxInfo, const _ushort& wCntX, const _ushort& wCntZ)
{
	//3 x 3의 지형이라면 아래쪽을 Top으로 보고 윗쪽을 Bottom으로 구성
	m_iCorner[CORNER_LT] = 0;							//0		
	m_iCorner[CORNER_RT] = wCntX - 1;					//2
	m_iCorner[CORNER_LB] = wCntX * (wCntZ - 1);			//6
	m_iCorner[CORNER_RB] = wCntX * wCntZ - 1;			//8

	for (_int i = 0; i < CORNER_END; ++i)				//각 코너와 센터를 합
		m_iCenter += m_iCorner[i];

	m_iCenter = m_iCenter >> 2;							//최종 Center에는 모든 Center의 합 (이 합을 /4 하면 정확히 중점이 나옴)
	Make_Child(pGraphicDev, pTerrainVtxInfo);

	return S_OK;
}

//------------------------------------------------------------------------------------------------------------------------
HRESULT CQuadTree::Init_Neighbor(CQuadTree* pParent, const _ushort& wCntX, const _ushort& wCntZ)
{
	_int			iLT, iRT, iLB, iRB, iCenter;

	for (_int i = 0; i < NEIGHBOR_END; ++i)
	{
		bool isExist = Add_Neighbor_Corner(NEIGHBOR(i), &iLT, &iRT, &iLB, &iRB, wCntX, wCntZ);

		if(isExist == true)
		{
			iCenter = (iLT + iRT + iLB + iRB) >> 2;
			m_pNeighbor[i] = pParent->Find_Neighbor(&iLT, &iRT, &iLB, &iRB, &iCenter);
		}
	}	

	for (_int i = 0; i < CHILD_END; ++i)
	{
		if(m_pChild[i] != NULL )
			m_pChild[i]->Init_Neighbor(pParent, wCntX, wCntZ);
	}

	return S_OK;
}

//------------------------------------------------------------------------------------------------------------------------
void CQuadTree::isIn(Engine::CFrustum* pFrustum, const Engine::VTXTEX* pTerrainVtxInfo)
{
	if(m_pChild[CHILD_LT] == NULL)
	{
		m_isIn = true;
		return;
	}

	m_isIn = pFrustum->SphereInFrustum(&pTerrainVtxInfo[m_iCenter].vPosition, m_fRadius);

	if(true == m_isIn)
	{
		for (_int i = 0; i < CHILD_END; ++i)
		{
			if( m_pChild[i] != NULL )
				m_pChild[i]->isIn(pFrustum, pTerrainVtxInfo);
		}		
	}
}

//------------------------------------------------------------------------------------------------------------------------
void CQuadTree::Culling_ForTerrain(Engine::CFrustum* pFrustum, const Engine::VTXTEX* pTerrainVtxInfo, Engine::INDEX32* pIndex, _ulong* pTriCnt)
{
	if(true == m_isIn)
	{
		if(m_pChild[CHILD_LT] == NULL
			|| LevelofDetail(&pTerrainVtxInfo[m_iCenter].vPosition))
		{

			_bool	bDraw[NEIGHBOR_END] = {1, 1, 1, 1};

			for (_int i = 0; i < NEIGHBOR_END; ++i)
			{
				if( m_pNeighbor[i] != NULL)
					bDraw[i] = m_pNeighbor[i]->LevelofDetail(&pTerrainVtxInfo[m_pNeighbor[i]->m_iCenter].vPosition);
			}

			_bool	isIn[4] = {false};
			for (_int i = 0; i < CORNER_END; ++i)			
				isIn[i] = pFrustum->VertexInFrustum(&pTerrainVtxInfo[m_iCorner[i]].vPosition);

			if(bDraw[NEIGHBOR_LEFT] 
			&& bDraw[NEIGHBOR_TOP]
			&& bDraw[NEIGHBOR_RIGHT]
			&& bDraw[NEIGHBOR_BOTTOM])				
			{
				if(isIn[CORNER_LB] == true
					|| isIn[CORNER_RB] == true
					|| isIn[CORNER_RT] == true)
				{
					pIndex[*pTriCnt]._1 = m_iCorner[CORNER_LB];
					pIndex[*pTriCnt]._2 = m_iCorner[CORNER_RB];
					pIndex[*pTriCnt]._3 = m_iCorner[CORNER_RT];
					++(*pTriCnt);
				}

				if( isIn[CORNER_LB] == true
					||  isIn[CORNER_RT] == true
					|| isIn[CORNER_LT] == true)
				{
					pIndex[*pTriCnt]._1 = m_iCorner[CORNER_LB];
					pIndex[*pTriCnt]._2 = m_iCorner[CORNER_RT];
					pIndex[*pTriCnt]._3 = m_iCorner[CORNER_LT];
					++(*pTriCnt);
				}	
				return;
			}	

			// 더 분열될 가능성이 있는 놈들
			if(bDraw[NEIGHBOR_LEFT] == false) // 왼쪽이웃이 더 분열될때
			{
				_int				iLC = (m_iCorner[CORNER_LT] + m_iCorner[CORNER_LB]) >> 1;

				pIndex[*pTriCnt]._1 = m_iCorner[CORNER_LB];
				pIndex[*pTriCnt]._2 = m_iCenter;
				pIndex[*pTriCnt]._3 = iLC;
				++(*pTriCnt);	

				pIndex[*pTriCnt]._1 = iLC;
				pIndex[*pTriCnt]._2 = m_iCenter;
				pIndex[*pTriCnt]._3 = m_iCorner[CORNER_LT];
				++(*pTriCnt);
			}
			else // 왼쪽이웃은 true 나머지중 분열(false)
			{
				pIndex[*pTriCnt]._1 = m_iCorner[CORNER_LB];
				pIndex[*pTriCnt]._2 = m_iCenter;
				pIndex[*pTriCnt]._3 = m_iCorner[CORNER_LT];
				++(*pTriCnt);
			}			

			if( bDraw[NEIGHBOR_TOP] == false) // 위쪽이웃이 더 분열될때
			{
				_int				iTC = (m_iCorner[CORNER_LT] + m_iCorner[CORNER_RT]) >> 1;

				pIndex[*pTriCnt]._1 = m_iCorner[CORNER_LT];
				pIndex[*pTriCnt]._2 = m_iCenter;
				pIndex[*pTriCnt]._3 = iTC;
				++(*pTriCnt);	

				pIndex[*pTriCnt]._1 = iTC;
				pIndex[*pTriCnt]._2 = m_iCenter;
				pIndex[*pTriCnt]._3 = m_iCorner[CORNER_RT];
				++(*pTriCnt);
			}
			else // 윗이웃은 true 나머지중 분열(false)
			{
				pIndex[*pTriCnt]._1 = m_iCorner[CORNER_LT];
				pIndex[*pTriCnt]._2 = m_iCenter;
				pIndex[*pTriCnt]._3 = m_iCorner[CORNER_RT];
				++(*pTriCnt);
			}	

			if( bDraw[NEIGHBOR_RIGHT] == false) // 오른쪽이웃이 더 분열될때
			{
				_int				iRC = (m_iCorner[CORNER_RT] + m_iCorner[CORNER_RB]) >> 1;

				pIndex[*pTriCnt]._1 = m_iCorner[CORNER_RT];
				pIndex[*pTriCnt]._2 = m_iCenter;
				pIndex[*pTriCnt]._3 = iRC;
				++(*pTriCnt);	

				pIndex[*pTriCnt]._1 = iRC;
				pIndex[*pTriCnt]._2 = m_iCenter;
				pIndex[*pTriCnt]._3 = m_iCorner[CORNER_RB];
				++(*pTriCnt);
			}
			else // 오른이웃은 true 나머지중 분열(false)
			{
				pIndex[*pTriCnt]._1 = m_iCorner[CORNER_RT];
				pIndex[*pTriCnt]._2 = m_iCenter;
				pIndex[*pTriCnt]._3 = m_iCorner[CORNER_RB];
				++(*pTriCnt);
			}	

			if( bDraw[NEIGHBOR_BOTTOM] == false) // 오른쪽이웃이 더 분열될때
			{
				_int				iBC = (m_iCorner[CORNER_LB] + m_iCorner[CORNER_RB]) >> 1;

				pIndex[*pTriCnt]._1 = m_iCorner[CORNER_LB];
				pIndex[*pTriCnt]._2 = iBC;
				pIndex[*pTriCnt]._3 = m_iCenter;
				++(*pTriCnt);	

				pIndex[*pTriCnt]._1 = m_iCenter;
				pIndex[*pTriCnt]._2 = iBC;
				pIndex[*pTriCnt]._3 = m_iCorner[CORNER_RB];
				++(*pTriCnt);
			}
			else // 아래이웃은 true 나머지중 분열(false)
			{
				pIndex[*pTriCnt]._1 = m_iCorner[CORNER_LB];
				pIndex[*pTriCnt]._2 = m_iCorner[CORNER_RB];
				pIndex[*pTriCnt]._3 = m_iCenter;
				++(*pTriCnt);
			}
		}

		else
		{
			for (_int i = 0; i < CHILD_END; ++i)
			{
				m_pChild[i]->Culling_ForTerrain(pFrustum, pTerrainVtxInfo, pIndex, pTriCnt);
			}		
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------
void CQuadTree::Make_Child(LPDIRECT3DDEVICE9 pGraphicDev, Engine::VTXTEX* pTerrainVtxInfo)
{
	m_pTerrainVtxInfo = pTerrainVtxInfo;
	m_pGraphicDev = pGraphicDev;

	if(m_iCorner[CORNER_RT] - m_iCorner[CORNER_LT] == 1)
		return;

	m_fRadius = D3DXVec3Length(&(pTerrainVtxInfo[m_iCorner[CORNER_LT]].vPosition - pTerrainVtxInfo[m_iCenter].vPosition));

	_int		iLC, iTC, iRC, iBC;
	SetSideCenterIdx(&iLC, &iTC, &iRC, &iBC);

	for (_int i = 0; i < CHILD_END; ++i)	
	{
		m_pChild[i] = SetChild(CHILD(i), &iLC, &iTC, &iRC, &iBC);	
		m_pChild[i]->Make_Child(pGraphicDev, pTerrainVtxInfo);
	}
}

//------------------------------------------------------------------------------------------------------------------------
bool CQuadTree::Add_Neighbor_Corner(NEIGHBOR Flag, _int* pLT, _int* pRT, _int* pLB, _int* pRB, const _ushort& wCntX, const _ushort& wCntZ)
{
	_int	iSize = m_iCorner[CORNER_RT] - m_iCorner[CORNER_LT];

	switch(Flag)
	{
	case NEIGHBOR_LEFT:
		*pLT = m_iCorner[CORNER_LT] - iSize;
		*pRT = m_iCorner[CORNER_LT];
		*pLB = m_iCorner[CORNER_LB] - iSize;
		*pRB = m_iCorner[CORNER_LB];

		if(*pLT < 0 || m_iCorner[CORNER_LT] / wCntX != *pLT / wCntX)
			return false;

		break;
	case NEIGHBOR_TOP:
		*pLT = m_iCorner[CORNER_LT] - iSize * wCntX;
		*pRT = m_iCorner[CORNER_RT] - iSize * wCntX;
		*pLB = m_iCorner[CORNER_LT];
		*pRB = m_iCorner[CORNER_RT];

		if(*pLT < 0)
			return false;

		break;

	case NEIGHBOR_RIGHT:
		*pLT = m_iCorner[CORNER_RT];
		*pRT = m_iCorner[CORNER_RT] + iSize;
		*pLB = m_iCorner[CORNER_RB];
		*pRB = m_iCorner[CORNER_RB] + iSize;

		if(m_iCorner[CORNER_RT] / wCntX != *pRT / wCntX)
			return false;

		break;

	case NEIGHBOR_BOTTOM:
		*pLT = m_iCorner[CORNER_LB];
		*pRT = m_iCorner[CORNER_RB];
		*pLB = m_iCorner[CORNER_LB] + iSize * wCntX;
		*pRB = m_iCorner[CORNER_RB] + iSize * wCntX;

		if(*pLB > wCntX * wCntZ - 1)
			return false;
		break;
	}
	return true;
}

//------------------------------------------------------------------------------------------------------------------------
CQuadTree* CQuadTree::Find_Neighbor(const _int* pLT, const _int* pRT, const _int* pLB, const _int* pRB, const _int* pCenter)
{
	if(m_iCorner[CORNER_LT] == *pLT
		&& m_iCorner[CORNER_RT] == *pRT		
		&& m_iCorner[CORNER_LB] == *pLB
		&& m_iCorner[CORNER_RB] == *pRB)
		return this;

	for (_int i = 0; i < CHILD_END; ++i)
	{
		CQuadTree*		pTemp = NULL;

		if( m_pChild[i] != NULL )
		{
			RECT		rcQuad = { long(m_pTerrainVtxInfo[m_pChild[i]->m_iCorner[CORNER_LT]].vPosition.x)
				, long(m_pTerrainVtxInfo[m_pChild[i]->m_iCorner[CORNER_LT]].vPosition.z)
				, long(m_pTerrainVtxInfo[m_pChild[i]->m_iCorner[CORNER_RB]].vPosition.x)
				, long(m_pTerrainVtxInfo[m_pChild[i]->m_iCorner[CORNER_RB]].vPosition.z) };

			POINT		ptCenter = { long(m_pTerrainVtxInfo[*pCenter].vPosition.x)
				, long(m_pTerrainVtxInfo[*pCenter].vPosition.z) };

			if( PtInRect(&rcQuad, ptCenter) == 1)
				pTemp = m_pChild[i]->Find_Neighbor(pLT, pRT, pLB, pRB, pCenter);
		}
		if(NULL == pTemp)
			continue;
		else
			return pTemp;
	}

	return NULL;
}

//------------------------------------------------------------------------------------------------------------------------
CQuadTree* CQuadTree::SetChild(CHILD ChildFlag, const _int* pLC, const _int* pTC, const _int* pRC, const _int* pBC)
{
	CQuadTree*			pChild = new CQuadTree;

	switch(ChildFlag)
	{
	case CHILD_LT:
		pChild->m_iCorner[CORNER_LT] = m_iCorner[CORNER_LT];
		pChild->m_iCorner[CORNER_RT] = *pTC;
		pChild->m_iCorner[CORNER_LB] = *pLC;
		pChild->m_iCorner[CORNER_RB] = m_iCenter;
		break;

	case CHILD_RT:
		pChild->m_iCorner[CORNER_LT] = *pTC;
		pChild->m_iCorner[CORNER_RT] = m_iCorner[CORNER_RT];
		pChild->m_iCorner[CORNER_LB] = m_iCenter;
		pChild->m_iCorner[CORNER_RB] = *pRC;

		break;

	case CHILD_LB:
		pChild->m_iCorner[CORNER_LT] = *pLC;
		pChild->m_iCorner[CORNER_RT] = m_iCenter;
		pChild->m_iCorner[CORNER_LB] = m_iCorner[CORNER_LB];
		pChild->m_iCorner[CORNER_RB] = *pBC;
		break;

	case CHILD_RB:
		pChild->m_iCorner[CORNER_LT] = m_iCenter;
		pChild->m_iCorner[CORNER_RT] = *pRC;
		pChild->m_iCorner[CORNER_LB] = *pBC;
		pChild->m_iCorner[CORNER_RB] = m_iCorner[CORNER_RB];
		break;
	}

	for (_int i = 0; i < CORNER_END; ++i)	
		pChild->m_iCenter += pChild->m_iCorner[i];

	pChild->m_iCenter = pChild->m_iCenter >> 2;	
	pChild->m_pParent = this;

	return pChild;
}

//------------------------------------------------------------------------------------------------------------------------
void CQuadTree::SetSideCenterIdx(_int* pLC, _int* pTC, _int* pRC, _int* pBC)
{
	*pLC = (m_iCorner[CORNER_LT] + m_iCorner[CORNER_LB]) >> 1;
	*pTC = (m_iCorner[CORNER_LT] + m_iCorner[CORNER_RT]) >> 1;
	*pRC = (m_iCorner[CORNER_RT] + m_iCorner[CORNER_RB]) >> 1;
	*pBC = (m_iCorner[CORNER_LB] + m_iCorner[CORNER_RB]) >> 1;
}

//------------------------------------------------------------------------------------------------------------------------
bool CQuadTree::LevelofDetail(const _vec3* pCenterPos)
{
	_matrix				matView;

	if(m_pGraphicDev == NULL)
		return false;	

	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	_vec3				vCamPos;
	memcpy(&vCamPos, &matView.m[3][0], sizeof(_vec3));

	float		fDistance = 0.f;

	_vec3	vTmp = *pCenterPos - vCamPos;

	fDistance = D3DXVec3Length(&vTmp);
	fDistance *= 0.1f;

	if((m_iCorner[CORNER_RT] - m_iCorner[CORNER_LT]) > fDistance)
		return false; 
	else
		return true;	
}

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
void CQuadTree::Release(void)
{
	for (_int i = 0; i < CHILD_END; ++i)	
		Engine::safe_delete(m_pChild[i]);
}
