#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef struct tagMesh_Derived : public D3DXMESHCONTAINER
	{
		LPD3DXMESH					pOriMesh;	// ������������ �����޽��� ����
		D3DXATTRIBUTERANGE*			pAttributeTable; // �޽ó����� ���Ǵ� ����µ��� ������ ������� ����ü
		vector<D3DMATERIAL9>		vecMtrls; // ���� �޽��� ��������
		vector<LPDIRECT3DTEXTURE9>	vecTextures;// ���� �޽��� �ؽ�������
		D3DXMATRIX*					pBoneoffsetMatrix; // �޽ÿ� ������ ��ġ�� ������ ��Ĺ迭�� ������ּ�
		D3DXMATRIX*					pResultMatrix; // ���������� ������ �Ǿ��� �޽��� ���
		D3DXMATRIX**				ppBoneMatrix; // ���������� ������ �Ǿ��� �޽��� ���
	}D3DXMESHCONTAINER_DERIVED;

	typedef struct tagObjectInfo
	{
		_vec3		vPos;
		_vec3		vScale;	
		_int		iType;					// Resource���� Layer�ȿ� � �׷�ȿ� ����ִ���??? ��) Monster -> �ߺз�
		TCHAR		szObjTag[32];			// Resource ���� �������̸� ��) Juliet -> �Һз�
	}OBJECT;

	typedef struct tagBone : public D3DXFRAME
	{
		D3DXMATRIX					CombinedTransformMatrix;
	}DERIVED_D3DXFRAME;

	typedef struct tagCollider
	{
		D3DXVECTOR3			vSize;
		D3DXMATRIX			matWorld;
	}COLLIDER;

	typedef struct tagVertex_Screen
	{
		D3DXVECTOR4				vPosition;					
		D3DXVECTOR2				vTexUV;
	}VTXSCREEN;

	const _ulong VTXSCREEN_FVF = D3DFVF_XYZRHW | D3DFVF_TEX1;

	typedef struct tagVtxColor
	{
		D3DXVECTOR3		vPosition;	//��ġ
		DWORD			dwColor;//����
	}VTXCOL;

	typedef struct tagNaviMesh
	{
		VTXCOL		tVtxCol[3];
		DWORD		dwNaviIdx;
		DWORD		dwVtxSize;
		DWORD		dwVtxFVF;
		DWORD		dwVtxCnt;
		DWORD		dwTriCnt;
		DWORD		dwIdx;
		DWORD		dwNaviType;

	}NAVIMESHINFO;

	typedef struct tagVertex_Texture
	{
		D3DXVECTOR3				vPosition;			
		D3DXVECTOR3				vNormal;
		D3DXVECTOR2				vTexUV;	
	}VTXTEX;

	typedef struct tagVtxCube
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vNormal;
		D3DXVECTOR3		vTexUV;
	}VTXCUBE;

	typedef struct NavigationCell
	{
		D3DXVECTOR3		vPosition[3];
	}NAVICELL;

	const _ulong VTXTEX_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
	const _ulong VTXFVF_CUBE = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE3(0);
	const _ulong VTXFVF_COL = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX0;

	typedef struct tagIndex16
	{
		WORD	_1, _2, _3;
	}INDEX16;

	typedef struct tagIndex32
	{
		DWORD	_1, _2, _3;
	}INDEX32;

//Frame�� ������
	typedef struct tagFrame
	{
		float		fFrame;
		float		fCnt;
		float		fMax;
	}FRAME;
}

#endif // Engine_Struct_h__
