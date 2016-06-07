#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef struct tagMesh_Derived : public D3DXMESHCONTAINER
	{
		LPD3DXMESH					pOriMesh;	// 변형되지않은 원형메시의 정보
		D3DXATTRIBUTERANGE*			pAttributeTable; // 메시내에서 사용되는 서브셋들의 정보를 담기위한 구조체
		vector<D3DMATERIAL9>		vecMtrls; // 현재 메시의 재질집합
		vector<LPDIRECT3DTEXTURE9>	vecTextures;// 현재 메시의 텍스쳐집합
		D3DXMATRIX*					pBoneoffsetMatrix; // 메시에 영향을 미치는 본들의 행렬배열의 가장앞주소
		D3DXMATRIX*					pResultMatrix; // 최종적으로 렌더링 되야할 메시의 행렬
		D3DXMATRIX**				ppBoneMatrix; // 최종적으로 렌더링 되야할 메시의 행렬
	}D3DXMESHCONTAINER_DERIVED;

	typedef struct tagObjectInfo
	{
		_vec3		vPos;
		_vec3		vScale;	
		_int		iType;					// Resource들이 Layer안에 어떤 그룹안에 들어있는지??? 예) Monster -> 중분류
		TCHAR		szObjTag[32];			// Resource 들의 지정된이름 예) Juliet -> 소분류
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
		D3DXVECTOR3		vPosition;	//위치
		DWORD			dwColor;//색상
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

//Frame을 돌리자
	typedef struct tagFrame
	{
		float		fFrame;
		float		fCnt;
		float		fMax;
	}FRAME;
}

#endif // Engine_Struct_h__
