#include "NaviMeshMgr.h"

using namespace Engine;
IMPLEMENT_SINGLETON(CNaviMeshMgr)

//------------------------------------------------------------------------------------------------------------------------------------------------

CNaviMeshMgr::CNaviMeshMgr(void)
: m_bReserved(false) 
, m_Line3D(NULL)
{
	
}

//------------------------------------------------------------------------------------------------------------------------------------------------
CNaviMeshMgr::~CNaviMeshMgr(void)
{
	Release();
}

//------------------------------------------------------------------------------------------------------------------------------------------------
void CNaviMeshMgr::Reserve_NaviMeshSize(LPDIRECT3DDEVICE9 pGraphicDev , _int iSize)
{
	m_pGraphicDev = pGraphicDev;
	m_bReserved	= true;
	m_vecNaviMesh.reserve(iSize);
	D3DXCreateLine(m_pGraphicDev , &m_Line3D);
}

void CNaviMeshMgr::Init_Normal(void)
{

}

//------------------------------------------------------------------------------------------------------------------------------------------------
HRESULT CNaviMeshMgr::Add_NaviMesh(const _vec3* pPointA , const _vec3* pPointB , const _vec3* pPointC)
{
	if(m_bReserved == false)
		return E_FAIL;

	_vec3 vTempA = *pPointA ;
	_vec3 vTempB = *pPointB ;
	_vec3 vTempC = *pPointC ;

	if( (pPointA->x == pPointB->x && pPointA->y == pPointB->y && pPointA->z == pPointB->z)
		 || (pPointA->x == pPointC->x && pPointA->y == pPointC->y && pPointA->z == pPointC->z)
		 || (pPointB->x == pPointC->x && pPointB->y == pPointC->y && pPointB->z == pPointC->z)
		)
	{
		msg_box("값오류로 인해 삭제 !");
		return S_OK;
	}
	CNaviMesh*		pNaviMesh = CNaviMesh::Create(&vTempA , &vTempB , &vTempC , m_vecNaviMesh.size());

	if(pNaviMesh == NULL)
		return E_FAIL;
	
	m_vecNaviMesh.push_back(pNaviMesh);

	return S_OK;
}
//------------------------------------------------------------------------------------------------------------------------------------------------
void CNaviMeshMgr::Link_NaViMesh(void)
{
	for( _uint i = 0 ; i < m_vecNaviMesh.size() ; ++i)
	{
		for( _uint j = 0 ; j < m_vecNaviMesh.size() ; ++j)
		{
			if(m_vecNaviMesh[i] == m_vecNaviMesh[j])
				continue;

			if(m_vecNaviMesh[j]->Compare_Point(
				m_vecNaviMesh[i]->GetPoint(CNaviMesh::POINT_A)
			,	m_vecNaviMesh[i]->GetPoint(CNaviMesh::POINT_B)
			,   m_vecNaviMesh[i]))
			{
				m_vecNaviMesh[i]->SetNeighbor(CNaviMesh::NEIGHBOR_AB , m_vecNaviMesh[j]);
			}

			if(m_vecNaviMesh[j]->Compare_Point(
				m_vecNaviMesh[i]->GetPoint(CNaviMesh::POINT_B)
				,	m_vecNaviMesh[i]->GetPoint(CNaviMesh::POINT_C)
				,   m_vecNaviMesh[i]))
			{
				m_vecNaviMesh[i]->SetNeighbor(CNaviMesh::NEIGHBOR_BC , m_vecNaviMesh[j]);
			}

			if(m_vecNaviMesh[j]->Compare_Point(
				m_vecNaviMesh[i]->GetPoint(CNaviMesh::POINT_C)
				,	m_vecNaviMesh[i]->GetPoint(CNaviMesh::POINT_A)
				,   m_vecNaviMesh[i]))
			{
				m_vecNaviMesh[i]->SetNeighbor(CNaviMesh::NEIGHBOR_CA , m_vecNaviMesh[j]);
			}
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------------------------------

void CNaviMeshMgr::NaviMesh_Render(void)
{
	mVECNAVI::iterator iter = m_vecNaviMesh.begin();
	mVECNAVI::iterator iter_end = m_vecNaviMesh.end();

	_matrix matworld ;
	D3DXMatrixIdentity(&matworld);
	for(; iter != iter_end; ++iter)
		(*iter)->Render(m_pGraphicDev , &matworld);

}
//------------------------------------------------------------------------------------------------------------------------------------------------
_ulong CNaviMeshMgr::MoveOnNaviMesh(_vec3* pObjectPos , _vec3* pObjectDir , const _ulong& dwObjectPosIndex)
{
	m_dwObjectPosIndex = dwObjectPosIndex;

	m_pObjectPos = pObjectPos;
	m_pObjectDir = pObjectDir;

	CNaviMesh::RESULT	eResult = m_vecNaviMesh[m_dwObjectPosIndex]->CheckPass(m_pObjectPos , m_pObjectDir , m_dwObjectPosIndex);

	int iNumCheck = 0;
	switch( eResult )
	{
	case CNaviMesh::RESULT_ON:
		*m_pObjectPos += *m_pObjectDir;
		break;

	case CNaviMesh::RESULT_NEIGHBOR:
		*m_pObjectPos += *m_pObjectDir;
		break;

	case CNaviMesh::RESULT_STOP:
		{
			//바뀐걸 말해주자
			_vec3	vOriSliding ,vResultSlide , vNormal , vDir;

			vDir = *m_pObjectDir;
			vNormal = m_vecNaviMesh[m_dwObjectPosIndex]->GetNormal();
			vOriSliding = vDir - vNormal * D3DXVec3Dot(&vDir , &vNormal);

			_ulong dwPosIdx = m_dwObjectPosIndex;

			_int	iResult = m_vecNaviMesh[m_dwObjectPosIndex]->CheckPass(m_pObjectPos , &vOriSliding , m_dwObjectPosIndex );

			
			_float		fDistance[3];
			_float		fCheckDist;

			for(int i = 0 ; i < 3; ++i)
				fDistance[i] = D3DXVec3Length( &(m_vecNaviMesh[m_dwObjectPosIndex]->GetInfo().tVtxCol[i].vPosition - *pObjectPos) );

			fCheckDist = min(fDistance[0] , fDistance[1]);

			if(fCheckDist == fDistance[0])
				iNumCheck = 0;
			else
				iNumCheck = 1;

			fCheckDist = min(fDistance[iNumCheck] , fDistance[2] );

			if(fCheckDist == fDistance[2])
				iNumCheck = 2;

			switch(iResult)
			{
			case CNaviMesh::RESULT_NEIGHBOR:
				if(NULL == m_vecNaviMesh[dwPosIdx]->Get_PointLine(CNaviMesh::POINT(iNumCheck)))
					break;
				vNormal = m_vecNaviMesh[dwPosIdx]->Get_PointLine(CNaviMesh::POINT(iNumCheck))->GetNormal();
				vResultSlide = vOriSliding - (vNormal * D3DXVec3Dot(&vDir , &vNormal));
				*m_pObjectPos += vResultSlide;
				break;

			case CNaviMesh::RESULT_ON:
				*m_pObjectPos += vOriSliding;				
				break;

			case CNaviMesh::RESULT_STOP:
				if(NULL == m_vecNaviMesh[dwPosIdx]->Get_PointLine(CNaviMesh::POINT(iNumCheck)))
					break;

				vNormal = m_vecNaviMesh[dwPosIdx]->Get_PointLine(CNaviMesh::POINT(iNumCheck))->GetNormal();
				vResultSlide = vOriSliding - (vNormal * D3DXVec3Dot(&vDir , &vNormal));
				*m_pObjectPos += vResultSlide;
				break;
			}
		}
		break;
	}

	m_pObjectPos->y = m_vecNaviMesh[m_dwObjectPosIndex]->GetHeight(m_pObjectPos->x , m_pObjectPos->z);
	return m_dwObjectPosIndex;
}

//------------------------------------------------------------------------------------------------------------------------------------------------
void CNaviMeshMgr::Release(void)
{
	for_each(m_vecNaviMesh.begin() , m_vecNaviMesh.end() , Engine::CDelete_Single());

	vector<CNaviMesh*>			VecTmp;
	m_vecNaviMesh.swap(VecTmp);
	VecTmp.clear();

	Engine::safe_release(m_Line3D);
}
//------------------------------------------------------------------------------------------------------------------------------------------------

void CNaviMeshMgr::CheckDelete(_int iSelect)
{
	if(iSelect == 0)
		m_vecNaviMesh.pop_back();
	else
	{
		Release();		
		Reserve_NaviMeshSize(m_pGraphicDev, 50);		
	}
}
//------------------------------------------------------------------------------------------------------------------------------------------------

vector<CNaviMesh*>* Engine::CNaviMeshMgr::GetVecNaviMesh(void)
{
	return &m_vecNaviMesh;
}

void Engine::CNaviMeshMgr::Init_Point_Neighbor(void)
{
	for ( _uint i = 0; i < m_vecNaviMesh.size(); ++i )
	{
		for( _uint j = 0; j < m_vecNaviMesh.size(); ++j)
		{
			if(m_vecNaviMesh[i] == m_vecNaviMesh[j])
				continue;

			m_vecNaviMesh[i]->Init_Neighbor_Point(CNaviMesh::POINT_A, m_vecNaviMesh[j]->GetPoint(CNaviMesh::POINT_A), 
				m_vecNaviMesh[j]->GetPoint(CNaviMesh::POINT_B), m_vecNaviMesh[j]->GetPoint(CNaviMesh::POINT_C));

			m_vecNaviMesh[i]->Init_Neighbor_Point(CNaviMesh::POINT_B, m_vecNaviMesh[j]->GetPoint(CNaviMesh::POINT_A), 
				m_vecNaviMesh[j]->GetPoint(CNaviMesh::POINT_B), m_vecNaviMesh[j]->GetPoint(CNaviMesh::POINT_C));

			m_vecNaviMesh[i]->Init_Neighbor_Point(CNaviMesh::POINT_C, m_vecNaviMesh[j]->GetPoint(CNaviMesh::POINT_A), 
				m_vecNaviMesh[j]->GetPoint(CNaviMesh::POINT_B), m_vecNaviMesh[j]->GetPoint(CNaviMesh::POINT_C));
		}
	}
}	

void Engine::CNaviMeshMgr::Init_Point_NeighborNavi(void)
{
	_uint      iMaxCnt = m_vecNaviMesh.size();

	for( _uint i = 0; i < iMaxCnt; ++i )
	{
		for( _uint j = 0; j < iMaxCnt; ++j )
		{
			if(m_vecNaviMesh[i] == m_vecNaviMesh[j])
				continue;

			for(_uint y = 0; y < Engine::CNaviMesh::POINT_END; ++y)
			{
				const _vec3*   pSourPoint   = m_vecNaviMesh[i]->Get_Neighbor_Point(Engine::CNaviMesh::POINT(y));

				if( NULL != pSourPoint )
				{
					for(_uint z = 0; z < Engine::CNaviMesh::POINT_END; ++z)
					{
						const _vec3* pDestPoint = m_vecNaviMesh[j]->Get_Neighbor_Point(Engine::CNaviMesh::POINT(z));

						if( NULL != pDestPoint )
						{
							// 만약 나의 점과 상대방의 점이 같다면, 비교한다.
							if( *pSourPoint == *pDestPoint )
							{
								int      iLineNum = z;

								CLine2D*      pLine2D = m_vecNaviMesh[j]->Get_Line(CNaviMesh::LINE(iLineNum));

								const _vec2*      vStart   = pLine2D->Get_Line_Start();
								const _vec2*      vEnd   = pLine2D->Get_Line_End();

								_vec2      v2SourPoint = _vec2(pSourPoint->x, pSourPoint->z);

								if( v2SourPoint == *vStart
									|| v2SourPoint == *vEnd )
								{
									if( NULL == m_vecNaviMesh[j]->Get_Neighbor(CNaviMesh::NEIGHBOR(iLineNum)) ) 
									{
										m_vecNaviMesh[i]->Set_Point_Line(CNaviMesh::POINT(y), m_vecNaviMesh[j]->Get_Line(CNaviMesh::LINE(iLineNum)));
									}
								}

								iLineNum -= 1;
								if(iLineNum < 0)
									iLineNum = 2;

								pLine2D = m_vecNaviMesh[j]->Get_Line(CNaviMesh::LINE(iLineNum));

								vStart   = pLine2D->Get_Line_Start();
								vEnd   = pLine2D->Get_Line_End();


								if( v2SourPoint == *vStart 
									|| v2SourPoint == *vEnd )
								{
									if( NULL == m_vecNaviMesh[j]->Get_Neighbor(CNaviMesh::NEIGHBOR(iLineNum)) )
									{
										m_vecNaviMesh[i]->Set_Point_Line(CNaviMesh::POINT(y), m_vecNaviMesh[j]->Get_Line(CNaviMesh::LINE(iLineNum)));
									}
								}
							}
						}
					}
				}
			}         
		}
	}
}
