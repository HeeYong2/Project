#ifndef StaticMesh_h__
#define StaticMesh_h__
/********************************************************************
	created:	2015/02/25
	file path:	Yong\Engine\Resource\Code
	file base:	StaticMesh
	file ext:	h
	author:		Yong
	purpose:	애니메이션을 포함하지않은 메시
*********************************************************************/

#include "Mesh.h"

namespace Engine
{
	class _declspec(dllexport) CStaticMesh
		:	public CMesh
	{
	private:
		explicit CStaticMesh(void);

	public:
		~CStaticMesh(void);

	public:
		virtual HRESULT Ready_Mesh(LPDIRECT3DDEVICE9 pGraphicDev , const TCHAR* pPath , const TCHAR* pFileName);
		virtual void Render(LPDIRECT3DDEVICE9 pGraphicDev , const _matrix* pWorldMatrix);
		virtual void Render(LPDIRECT3DDEVICE9 pGraphicDev, LPD3DXEFFECT pEffect);
		virtual CResource* Clone_Resource(void);

	public:
		static CStaticMesh* Create(LPDIRECT3DDEVICE9 pGraphicDev , const TCHAR* pPath , const TCHAR* pFileName);
		LPD3DXMESH GetMesh(void);
		DWORD GetVtxNum(void);
		DWORD GetVtxByte(void);

	private:
		_ulong				m_dwNumMtrl;

	private:
		LPDIRECT3DTEXTURE9*	m_ppTextures;
		LPDIRECT3DTEXTURE9*	m_ppNormalTex;
		LPDIRECT3DTEXTURE9* m_ppSpecularTex;
		D3DMATERIAL9*		m_pMaterial;

		LPD3DXBUFFER		m_pSubset;
		LPD3DXMESH			m_pMesh;

	private:
		void Release(void);

	};
}
#endif // StaticMesh_h__
