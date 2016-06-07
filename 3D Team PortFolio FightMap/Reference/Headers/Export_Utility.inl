
//=For.Utility.Getter===================================================================
inline LPD3DXEFFECT Engine::Get_Effect(const TCHAR* pShaderTag)
{
	return Engine::CShaderMgr::GetInstance()->Get_Effect(pShaderTag);

}

inline Engine::CComponent* Engine::GetObjectComponent(const _int& iLayer , const TCHAR* pObjectTag , const TCHAR* pComponentTag , const _ushort& wCnt /* = 0 */)
{
	return Engine::CManagement::GetInstance()->GetObjectComponent(iLayer , pObjectTag , pComponentTag , wCnt);
}

inline  list<Engine::CGameObject*>* Engine::GetCurrentList(const _int& iLayer, const TCHAR* pObjectTag )
{
	return Engine::CManagement::GetInstance()->GetCurrentList(iLayer , pObjectTag);
}

inline  list<Engine::CGameObject*>* Engine::GetList(const _int& iLayer , const TCHAR* pObjectTag )
{
	return Engine::CManagement::GetInstance()->GetList( iLayer , pObjectTag );
}
//=For.Utility.Setter===================================================================
inline void Engine::Add_RenderGroup(Engine::CRenderer::RENDER_GROUP GroupID, Engine::CGameObject* pGameObject)
{
	Engine::CManagement::GetInstance()->Add_RenderGroup(GroupID, pGameObject);
}
inline void Engine::Reserve_NaviMeshSize(LPDIRECT3DDEVICE9 pGraphicDev , _int iSize)
{
	Engine::CNaviMeshMgr::GetInstance()->Reserve_NaviMeshSize(pGraphicDev , iSize);
}

inline HRESULT Engine::Add_NaviMesh(const _vec3* pPointA , const _vec3* pPointB , const _vec3* pPointC)
{
	return Engine::CNaviMeshMgr::GetInstance()->Add_NaviMesh(pPointA , pPointB ,pPointC );
}

//=For.Utility.General==================================================================
inline HRESULT Engine::Ready_Shader(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pShaderTag, const TCHAR* pFilePath)
{
	return Engine::CShaderMgr::GetInstance()->Ready_Shader(pGraphicDev, pShaderTag, pFilePath);
}
inline HRESULT Engine::Init_Management(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iSizeX, const _uint& iSizeY)
{
	return Engine::CManagement::GetInstance()->Init_Management(pGraphicDev, iSizeX, iSizeY);
}

template <typename T> HRESULT Engine::Replace_Current_Scene(LPDIRECT3DDEVICE9 pGraphicDev, T& Functor)
{
	return Engine::CManagement::GetInstance()->Replace_Current_Scene(pGraphicDev, Functor);
}

inline void Engine::Update(void)
{
	Engine::CManagement::GetInstance()->Update();
}
inline void Engine::Render(void)
{
	Engine::CManagement::GetInstance()->Render();
}

inline HRESULT Engine::Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev , const D3DLIGHT9* pLightInfo)
{
	return Engine::CLightMgr::GetInstance()->Ready_Light(pGraphicDev , pLightInfo);
}

inline void Engine::Link_NaviMesh(void)
{
	Engine::CNaviMeshMgr::GetInstance()->Link_NaViMesh();
}

inline void Engine::Init_Point_Neighbor(void)
{
	Engine::CNaviMeshMgr::GetInstance()->Init_Point_Neighbor();
}

inline void Engine::Init_Point_NeighborNavi(void)
{
	Engine::CNaviMeshMgr::GetInstance()->Init_Point_NeighborNavi();
}

inline void Engine::NaviMesh_Render(void)
{
	Engine::CNaviMeshMgr::GetInstance()->NaviMesh_Render();
}

inline _ulong Engine::MoveOnNaviMesh(_vec3* pObjectPos , _vec3* pObjectDir , const _ulong& dwObjectPosIndex)
{
	return Engine::CNaviMeshMgr::GetInstance()->MoveOnNaviMesh(pObjectPos , pObjectDir , dwObjectPosIndex);
}

inline void Engine::CheckDelete(_int iSelect)
{
	Engine::CNaviMeshMgr::GetInstance()->CheckDelete(iSelect);
}

inline vector<Engine::CNaviMesh*>* Engine::GetVecNaviMesh(void)
{
	return Engine::CNaviMeshMgr::GetInstance()->GetVecNaviMesh();
}

inline HRESULT Engine::Init_Frustum(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return Engine::CFrustum::GetInstance()->Init_Frustum(pGraphicDev);
}

inline void Engine::ViewToWorld(LPDIRECT3DDEVICE9 pGraphicDev,_bool bMakePlane)
{
	Engine::CFrustum::GetInstance()->ViewToWorld(pGraphicDev ,bMakePlane);
}

inline void Engine::WorldToLocal(LPDIRECT3DDEVICE9 pGraphicDev , const _matrix* pMatworld , _bool bMakePlane)
{
	Engine::CFrustum::GetInstance()->WorldToLocal(pGraphicDev ,pMatworld , bMakePlane);
}

inline void Engine::Culling_ForTerrain(const Engine::VTXTEX* pTerrain , const _ushort& wCntX , const _ushort& wCntZ , Engine::INDEX32* pIndex , _ulong* pTriCnt , const _matrix* pWorld)
{
	Engine::CFrustum::GetInstance()->Culling_ForTerrain(pTerrain , wCntX , wCntZ , pIndex , pTriCnt , pWorld);
}
inline bool Engine::Culling_ForObject(_float fRadius , _vec3 vPosition)
{
	return Engine::CFrustum::GetInstance()->Culling_ForObject(fRadius , vPosition);
}

inline HRESULT Engine::Init_QudTree(LPDIRECT3DDEVICE9 pGraphicDev ,Engine::VTXTEX* pTerrain , const _ushort& wCntX , const _ushort& wCntZ)
{
	return Engine::CQuadTree::GetInstance()->Init_QuadTree(pGraphicDev ,pTerrain , wCntX , wCntZ );
}

inline HRESULT Engine::Init_Neighbor(CQuadTree* pParent , const _ushort& wCntX , const _ushort& wCntZ)
{
	return Engine::CQuadTree::GetInstance()->Init_Neighbor(pParent , wCntX , wCntZ );
}

inline void Engine::isIn(const Engine::VTXTEX* pTerrain)
{
	Engine::CQuadTree::GetInstance()->isIn(Engine::CFrustum::GetInstance() , pTerrain);
}

inline void Engine::Culling_ForTerrain(const Engine::VTXTEX* pTerrain , Engine::INDEX32* pIndex , _ulong* pTriCnt)
{
	Engine::CQuadTree::GetInstance()->Culling_ForTerrain(Engine::CFrustum::GetInstance() , pTerrain , pIndex , pTriCnt);
}

inline Engine::CQuadTree* Engine::GetParentTree(void)
{
	return Engine::CQuadTree::GetInstance();
}

inline void Engine::Utility_Release(void)
{
	Engine::CFrustum::GetInstance()->DestroyInstance();
	Engine::CQuadTree::GetInstance()->DestroyInstance();
	Engine::CRenderTargetMgr::GetInstance()->DestroyInstance();
	Engine::CShaderMgr::GetInstance()->DestroyInstance();
	Engine::CNaviMeshMgr::GetInstance()->DestroyInstance();
	Engine::CLightMgr::GetInstance()->DestroyInstance();
	Engine::CManagement::GetInstance()->DestroyInstance();
}