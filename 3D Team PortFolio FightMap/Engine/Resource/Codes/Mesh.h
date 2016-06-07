#ifndef Mesh_h__
#define Mesh_h__
/********************************************************************
	created:	2015/02/25
	file path:	Yong\Engine\Resource\Code
	file base:	Resource
	file ext:	h
	author:		Yong	
	purpose:	Mesh 리소스들의 부모클래스다.
*********************************************************************/
#include "Resource.h"
#include "MeshTextureCtrl.h"

namespace Engine
{
	class _declspec(dllexport) CMesh
		: public CResource
	{
	public:
		enum MESHTYPE	{TYPE_STATIC , TYPE_DYNAMIC , TYPE_COLLIDER};

	public:
		explicit CMesh(void);
		~CMesh(void);

	public:
		virtual HRESULT Ready_Mesh(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pPath , const TCHAR* pFileName)PURE;
		virtual	void Render(LPDIRECT3DDEVICE9 pGraphicDev , const _matrix* pWorldMatrix)PURE;
		virtual void Render(LPDIRECT3DDEVICE9 pGraphicDev , LPD3DXEFFECT pEffect){}

	public:
		virtual CResource* Clone_Resource(void)PURE;

	public:
		void Release(void);
	};
}
#endif // Mesh_h__
