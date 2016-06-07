#ifndef Collider_h__
#define Collider_h__
/********************************************************************
	created:	2015/02/26
	file path:	Yong\Engine\Resource\Code
	file base:	Collider
	file ext:	h
	author:		Yong	
	purpose:	Ãæµ¹
*********************************************************************/
#include "Mesh.h"

namespace Engine
{
	class CCollider 
		: public CMesh
	{

	public:
		typedef struct tagCollider 
		{
			_vec3				m_vMin , m_vMax;
			_matrix				m_matWorld;
		}COLLIDER;

	public:
		explicit CCollider(void);	
		~CCollider(void);
	
	public:	//getter
		const COLLIDER* GetColliderInfo(void) {
			return & m_Collider;}
		void SetRadius(float fRadius);

	public:
		virtual HRESULT Ready_Mesh(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pPath , const TCHAR* pFileName);
		virtual void Render(LPDIRECT3DDEVICE9 pGraphicDev, const _matrix* pWorldMatrix);
		virtual CResource* Clone_Resource(void);

	public:
		static CCollider* Create(LPDIRECT3DDEVICE9 pGraphicDev , const TCHAR* pPath , const TCHAR* pFileName);

	private:
		LPD3DXMESH			m_pMesh;
		COLLIDER			m_Collider;

	private:
		float				m_fRadius;

	private:
		void Release(void);
	};
}

#endif // Collider_h__
