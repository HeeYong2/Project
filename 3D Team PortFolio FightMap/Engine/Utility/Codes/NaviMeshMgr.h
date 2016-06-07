#ifndef NaviMeshMgr_h__
#define NaviMeshMgr_h__

/********************************************************************
	created:	2015/03/04
	file path:	Yong\Engine\Resource\Code
	file base:	NaviMeshMgr
	file ext:	h
	author:		Yong
	purpose:	NaviMeshMgr
*********************************************************************/

#include "Engine_Defines.h"
#include "NaviMesh.h"

namespace Engine
{
	class _declspec(dllexport) CNaviMeshMgr 
	{
		DECLARE_SINGLETON(CNaviMeshMgr)

	public:
		explicit CNaviMeshMgr(void);
	public:
		~CNaviMeshMgr(void);

	public:
		void Reserve_NaviMeshSize(LPDIRECT3DDEVICE9 pGraphicDev , _int iSize);
		void Init_Normal(void);
		HRESULT Add_NaviMesh(const _vec3* pPointA , const _vec3* pPointB , const _vec3* pPointC);
		void Link_NaViMesh(void);
		void NaviMesh_Render(void);
		_ulong MoveOnNaviMesh(_vec3* pObjectPos , _vec3* pObjectDir , const _ulong& dwObjectPosIndex);
		void CheckDelete(_int iSelect );
		vector<CNaviMesh*>* GetVecNaviMesh(void);
		void Render(void);

	public:
		void Init_Point_Neighbor(void);
		void Init_Point_NeighborNavi(void);

	private:
		_bool							m_bReserved;
		vector<CNaviMesh*>				m_vecNaviMesh;
		typedef vector<CNaviMesh*>		mVECNAVI;

	private:
		_vec3*							m_pObjectPos;
		_vec3*							m_pObjectDir;
		_ulong							m_dwObjectPosIndex;
		_vec3*							m_pMousePos;

	private:
		LPD3DXLINE						m_Line3D;
		LPDIRECT3DDEVICE9				m_pGraphicDev;

	private:
		void Release(void);
	};
}
#endif // NaviMeshMgr_h__
