#ifndef Frustum_h__
#define Frustum_h__
/*!
 * \class CFrustum
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
 * \date 26ÀÏ 6¿ù 2015
 *
 * Contact: user@company.com
 *
 */
#include "Engine_Defines.h"

BEGIN(Engine)
class ENGINE_DLL CFrustum
{
	DECLARE_SINGLETON(CFrustum)
public:
	explicit CFrustum(void);
	virtual ~CFrustum(void);

public:
	HRESULT Init_Frustum(LPDIRECT3DDEVICE9 pGraphicDev);
	void ViewToWorld(LPDIRECT3DDEVICE9 pGraphicDev, _bool bMakPlane);
	void WorldToLocal(LPDIRECT3DDEVICE9 pGraphicDev , const _matrix* pMatWorld , _bool bMakePlane);
	void Culling_ForTerrain(const Engine::VTXTEX* pTerrainVtxInfo, const _ushort& wCntX
		, const _ushort& wCntZ, Engine::INDEX32* pIndex, _ulong* pTriCnt, const _matrix* pWorld);
	bool Culling_ForObject(_float fRadius , _vec3 vCenter);
	bool VertexInFrustum(const _vec3* pVtxPos);
	bool SphereInFrustum(const _vec3* pCenterPos , const float& fRadius );
	bool ObjectInSphere(const _vec3* pCenterPos , const float& fRadius );

private:
	_vec3			m_vOriginal_Point[8];
	_vec3			m_vConvert_Point[8];
	D3DXPLANE		m_Plane[6];

private:
	D3DXMATRIX		m_matWorld;

private:
	void Make_Plane(void);

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
};
END

#endif // Frustum_h__
