#ifndef Export_Utility_h__
#define Export_Utility_h__
/********************************************************************
	created:	2015/01/31
	file path:	Yong\Client\Code
	file base:	Export_Function
	file ext:	h
	author:		Yong	
	purpose:	최적화, 게임진행, 충돌 등등등
*********************************************************************/
#include "../../Reference/Headers/Management.h"
#include "../../Reference/Headers/LightMgr.h"
#include "../../Reference/Headers/NaviMeshMgr.h"
#include "../../Reference/Headers/NaviMesh.h"
#include "../../Reference/Headers/ShaderMgr.h"
#include "../../Reference/Headers/RenderTargetMgr.h"
#include "../../Reference/Headers/Frustum.h"
#include "../../Reference/Headers/QuadTree.h"


namespace Engine
{
	//=For.Utility.Getter===================================================================
	inline Engine::CComponent* GetObjectComponent(const _int& iLayer , const TCHAR* pObjectTag , const TCHAR* pComponentTag , const _ushort& wCnt = 0);
	inline list<Engine::CGameObject*>*	GetCurrentList(const _int& iLayer, const TCHAR* pObjectTag );
	inline list<Engine::CGameObject*>*	GetList( const _int& iLayer, const TCHAR* pObjectTag );
	inline LPD3DXEFFECT Get_Effect(const TCHAR* pShaderTag);

	//=For.Utility.Setter===================================================================
	//네비메시
	inline	void Reserve_NaviMeshSize(LPDIRECT3DDEVICE9 pGraphicDev , _int iSize);
	inline	HRESULT Add_NaviMesh(const _vec3* pPointA , const _vec3* pPointB , const _vec3* pPointC);

	inline void Add_RenderGroup(Engine::CRenderer::RENDER_GROUP GroupID, Engine::CGameObject* pGameObject);
	inline HRESULT	Ready_Light(LPDIRECT3DDEVICE9	pGraphicDev, const D3DLIGHT9* pLightInfo);
	//=For.Utility.General==================================================================
	inline HRESULT Ready_Shader(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pShaderTag, const TCHAR* pFilePath);
	inline HRESULT Init_Management(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iSizeX, const _uint& iSizeY);
	template <typename T> HRESULT Replace_Current_Scene(LPDIRECT3DDEVICE9 pGraphicDev, T& Functor);
	inline void Update(void);
	inline void Render(void);
	//Light
	inline HRESULT Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev , const D3DLIGHT9* pLightInfo);

	//NaviMesh
	inline void Link_NaviMesh(void);
	inline void Init_Point_Neighbor(void);
	inline void Init_Point_NeighborNavi(void);
	inline void NaviMesh_Render(void);
	inline _ulong MoveOnNaviMesh(_vec3* pObjectPos , _vec3* pObjectDir , const _ulong& dwObjectPosIndex);
	inline void CheckDelete(_int iSelect);
	inline vector<Engine::CNaviMesh*>* GetVecNaviMesh(void);
	inline void Utility_Release(void);
	

	//Frustum
	inline HRESULT Init_Frustum(LPDIRECT3DDEVICE9 pGraphicDev);
	inline void ViewToWorld(LPDIRECT3DDEVICE9 pGraphicDev,_bool bMakePlane);
	inline void WorldToLocal(LPDIRECT3DDEVICE9 pGraphicDev , const _matrix* pMatworld , _bool bMakePlane);
	inline void Culling_ForTerrain(const Engine::VTXTEX* pTerrain , const _ushort& wCntX , const _ushort& wCntZ , Engine::INDEX32* pIndex , _ulong* pTriCnt , const _matrix* pWorld);
	inline bool Culling_ForObject(_float fRadius , _vec3 vPosition);

	//QuadTree
	inline HRESULT Init_QudTree(LPDIRECT3DDEVICE9 pGraphicDev , Engine::VTXTEX* pTerrain , const _ushort& wCntX , const _ushort& wCntZ);
	inline HRESULT Init_Neighbor(CQuadTree* pParent , const _ushort& wCntX , const _ushort& wCntZ);
	inline void isIn(const Engine::VTXTEX* pTerrain);
	inline void Culling_ForTerrain(const Engine::VTXTEX* pTerrain , Engine::INDEX32* pIndex , _ulong* pTriCnt);
	inline CQuadTree* GetParentTree(void);
}

#include "Export_Utility.inl"

#endif // Export_Utility_h__