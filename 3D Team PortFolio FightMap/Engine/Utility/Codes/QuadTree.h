#ifndef QuadTree_h__
#define QuadTree_h__
/*!
 * \class CQuadTree
 *
 * \ingroup GroupName
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note 
 *
 * \author Èñ¿ë
 *
 * \version 1.0
 *
 * \date 27ÀÏ 6¿ù 2015
 *
 * Contact: user@company.com
 *
 */
#include "Engine_Defines.h"

BEGIN(Engine)

class CFrustum;
class ENGINE_DLL CQuadTree
{
	DECLARE_SINGLETON(CQuadTree)

public:
	enum CORNER   {CORNER_LT , CORNER_RT , CORNER_LB , CORNER_RB ,CORNER_END};
	enum CHILD	  {CHILD_LT , CHILD_RT , CHILD_LB , CHILD_RB , CHILD_END};
	enum NEIGHBOR {NEIGHBOR_LEFT , NEIGHBOR_TOP , NEIGHBOR_RIGHT , NEIGHBOR_BOTTOM , NEIGHBOR_END};

private:
	explicit CQuadTree(void);
public:
	~CQuadTree(void);

public:
	HRESULT Init_QuadTree(LPDIRECT3DDEVICE9 pGraphicDev, Engine::VTXTEX* pTerrainVtxInfo , const _ushort& wnCntX ,const _ushort& wnCntZ );
	HRESULT Init_Neighbor(CQuadTree* pParent , const _ushort& wCntX , const _ushort& wCntZ);
	void isIn(Engine::CFrustum* pFrustum , const Engine::VTXTEX* pTerrainvtxInfo);
	void Culling_ForTerrain(Engine::CFrustum* pFrustum , const Engine::VTXTEX*  pTerrainVtxInfo
		, Engine::INDEX32* pIndex , _ulong* pTriCnt);

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;

private:
	_int			m_iCorner[CORNER_END];
	_int			m_iCenter;
	_float			m_fRadius;
	CQuadTree*		m_pChild[CHILD_END];
	_bool			m_isIn;
	CQuadTree*		m_pNeighbor[NEIGHBOR_END];
	CQuadTree*		m_pParent;
	VTXTEX*			m_pTerrainVtxInfo;

private:	//For QuadTree
	void Make_Child(LPDIRECT3DDEVICE9 pGraphicDev,Engine::VTXTEX* pTerrainVtxInfo);
	CQuadTree* SetChild(CHILD ChildFlag , const _int* pLC , const _int* pTC , const _int* pRC ,const _int* pBC);
	void SetSideCenterIdx(_int* pLC , _int* pTC , _int* pRC , _int* pBC);

private:	//For Level Of Detail
	CQuadTree* Find_Neighbor(const _int* pLT ,const  _int* pRT ,const  _int* pLB ,const  _int* pRB , const _int* pCenter); 
	bool LevelofDetail(const _vec3* pCenterPos);
	bool Add_Neighbor_Corner(NEIGHBOR Flag , _int* pLT , _int* pRT , _int* pLB , _int* pRB , const _ushort& wCntX , const _ushort& wCntZ);

private:
	void Release(void);

};
END
#endif // QuadTree_h__
