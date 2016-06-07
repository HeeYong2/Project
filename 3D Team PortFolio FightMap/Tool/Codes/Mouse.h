#ifndef Mouse_h__
#define Mouse_h__

/********************************************************************
	created:	2015/04/15
	file path:	Yong\Tool\Code
	file base:	Mouse
	file ext:	h
	author:		Yong	
	purpose:	Mouse
*********************************************************************/
#include "Tool_Defines.h"
#include "Engine_Defines.h"
#include "Layer.h"

class CMouse
{
	DECLARE_SINGLETON(CMouse)

public:
	explicit CMouse(void);
	~CMouse();

public: // picking
	_int PickTerrain(_vec3* pOut );
	_bool PickObject(_matrix* pMatrix , const Engine::VTXTEX* pVtxTex);

public:	//메시 피킹
	_bool PickMesh(_matrix* pmatWorld, LPD3DXBASEMESH pMesh, _ulong dwWinCX, _ulong dwWinCY);			//메시피킹
	_bool GetPickMeshPos(_vec3* pPickPos, _matrix* pmatWorld, LPD3DXBASEMESH pMesh, _ulong dwWinCX, _ulong dwWinCY);	//메시의 로컬좌표를 월드좌표로 변환

public:
	void GetTerrainInfo(_vec3* pOut );
	void GetMousePos_ViewPort(POINT*	pPoint);
	void Translation_ViewSpace(const POINT* pPoint);
	void Translation_Local(const D3DXMATRIX*	pMatrix);

public: //Vxt Info
	void SetVtx(_int iVtxSizeX , _int iVtxSizeZ , _int iVtxItv);
	void SetVtxTex(Engine::VTXTEX* pVtxTex );

protected:
	_vec3		m_vPivotPos;
	_vec3		m_vRayDir;
	_int		m_iVtxSizeX;
	_int		m_iVtxSizeZ;
	_int		m_iVtxItv;

protected:
	Engine::VTXTEX*	m_pVtxTex;
	Engine::VTXTEX* m_pOriVtxTex;

};

#endif // Mouse_h__