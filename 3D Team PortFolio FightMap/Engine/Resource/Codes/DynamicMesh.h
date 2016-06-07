#ifndef DynamicMesh_h__
#define DynamicMesh_h__
/********************************************************************
	created:	2015/03/13
	file path:	Yong\Engine\Resource\Code
	file base:	Resource
	file ext:	h
	author:		Yong
	purpose:	Dynamic Mesh
*********************************************************************/
#include "Mesh.h"

namespace Engine
{
	class CAnimationController;
	class CHierachyLoader;
	class _declspec(dllexport) CDynamicMesh : public CMesh
	{
	private:
		explicit CDynamicMesh(void);
		explicit CDynamicMesh(const CDynamicMesh& Mesh);
	public:
		~CDynamicMesh(void);

	public:
		_bool	GetEndPeriod(void);
		const _matrix*	GetFrameMatrix(const char*	pFrameName);

	public: // Setter
		void SetAnimationSet(_uint iAniIdx);
		void FrameMove(_float fTime);
	public:
		virtual HRESULT Ready_Mesh(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pPath, const TCHAR* pFileName);
		virtual void Render(LPDIRECT3DDEVICE9 pGraphicDev, const _matrix* pWorldMatrix);
		void Render(LPDIRECT3DDEVICE9 pGraphicDev, DERIVED_D3DXFRAME* pBone);
		virtual CResource* Clone_Resource(void);
	public:
		void UpdateFrameMatrices(DERIVED_D3DXFRAME* pFrame, const _matrix* pmatParent);
		void SetUpBoneMatrixPointer(DERIVED_D3DXFRAME* pBone);
	public:
		static CDynamicMesh* Create(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pPath, const TCHAR* pFileName);
	private:
		CHierachyLoader*					m_pLoader;
		D3DXFRAME*							m_pRootBone;
		CAnimationController*				m_pAniCtrler;
	private:
		void Release(void);
	};
}

#endif // DynamicMesh_h__
