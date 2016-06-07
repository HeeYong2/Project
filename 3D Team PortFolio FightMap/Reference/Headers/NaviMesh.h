#ifndef NaviMesh_h__
#define NaviMesh_h__
/********************************************************************
	created:	2015/03/09
	file path:	Yong\Engine\Resource\Code
	file base:	NaviMesh
	file ext:	h
	author:		Yong
	purpose:	삼각형 하나를 의미한다
*********************************************************************/

#include "Engine_Defines.h"
#include "Line2D.h"

namespace Engine
{
	class _declspec(dllexport) CNaviMesh
	{
	public:
		enum POINT { POINT_A , POINT_B , POINT_C , POINT_END };
		enum NEIGHBOR { NEIGHBOR_AB ,  NEIGHBOR_BC ,  NEIGHBOR_CA ,  NEIGHBOR_END };
		enum LINE	{ LINE_AB ,  LINE_BC , LINE_CA , LINE_END , };
		enum RESULT { RESULT_ON , RESULT_NEIGHBOR , RESULT_STOP } ;
		enum NAVITYPE {TYPE_TERRAIN , TYPE_CAMERA};
		enum NORMAL		{ NORMAL_AB, NORMAL_BC, NORMAL_CA, NORMAL_END };

	public:
		explicit CNaviMesh(void);
		~CNaviMesh(void);

	public:
		const _vec3*		GetPoint(POINT iIndex) {
			return &m_vPoint[iIndex] ; }			

		_float GetHeight(_float fX , _float fZ);
		const Engine::CNaviMesh*	Get_Neighbor(CNaviMesh::NEIGHBOR eNeighbor);
		CLine2D*					Get_PointLine(POINT ePoint);
		const _vec3*				Get_Neighbor_Point(POINT ePoint);
		CLine2D*					Get_Line(LINE eLine);

	public:
		void						Set_Point_Neighbor(POINT ePoint , _vec3* pPoint);
		void						Set_Point_Line(CNaviMesh::POINT ePoint , CLine2D* pPointLine);

	public:
		void ReSetNaviPoint(POINT NaviPoint , const _vec3*	pPoint);
		void UpDownPoint(POINT NaviPoint , const _vec3* pPoint);

	public:
		void SetNeighbor( NEIGHBOR ID , CNaviMesh* pNeighbor ) {
			m_pNeighbor[ID] = pNeighbor; }

	public:
		HRESULT Init_Mesh(const _vec3* pPointA , const _vec3* pPointB , const _vec3* pPointC , _uint dwIndex);
		_bool	Init_Neighbor_Point(POINT ePoint , const _vec3* pPointA , const _vec3* pPointB , const _vec3* pPointC);
		_bool Compare_Point(const _vec3* pPointA , const _vec3* pPointB , CNaviMesh* pNeighbor);
		RESULT CheckPass(const _vec3* pPointion , const _vec3* pDirection , _ulong& dwIndex);
		void Make_Slide(const _vec3* pPosition, const _vec3* pDirection, _vec3* pSlide);
	public:
		_vec3  GetNormal(void);

	public:
		static CNaviMesh*	Create(const _vec3* pPointA , const _vec3* pPointB , const _vec3* pPointC  , _uint dwIndex);
		void Render(LPDIRECT3DDEVICE9 pGraphicDev, const _matrix* pmatWorld);



	public:
		bool			 			isInNavi(_vec3* pPos);
		NAVIMESHINFO	 			GetInfo(void);
		void			 			Init_Normal(void);

	private:
		_vec3						m_vPoint[POINT_END];
		CNaviMesh*					m_pNeighbor[NEIGHBOR_END];
		CLine2D*					m_pLine[LINE_END];
		_ulong						m_dwIndex;
		D3DXPLANE					m_Plane;
		_vec3						m_vNormal;
		_vec3						m_vtagNormal[NORMAL_END];

	private:
		NAVIMESHINFO				m_tNaviInfo;

	private:
		_vec3*						m_pNeighborPoint[POINT_END];				//점의 이웃(점을 알고 있다 -> 인접점을 찾는다)
		CLine2D*					m_pPointLine[POINT_END];					//점과 이어진 놈의 line들의 Normal값들 , 슬라이딩 벡터를 재세팅하는데 이용

	private:
		void Release(void);
	};
}
#endif // NaviMesh_h__