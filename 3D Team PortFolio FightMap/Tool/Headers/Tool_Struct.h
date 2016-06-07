#ifndef Tool_Struct_h__
#define Tool_Struct_h__

typedef struct tagVertex_Texture
{
	D3DXVECTOR3				vPosition;			
	D3DXVECTOR3				vNormal;
	D3DXVECTOR2				vTexUV;	
}VTXTEX;

typedef struct tagVtxData
{
	unsigned int	VTXCNTX;
	unsigned int	VTXCNTZ;
	unsigned int	VTXITV ;
}VTXDATA;

typedef struct tagObjectInfo
{
	_vec3	vPos;			//Object의 위치
	_vec3	vScale;			//Object의 크기
	_vec3	vAngle;			//Object의 회전
	_bool	isInstancing;
	_bool	isNeedUV;
	TCHAR	szName[32];		//Object들의 실제 이름(Monster라면 Soldier나 Obstacle이라면 tree , bush 등등)
}OBJINFO;
		//저장용 구조체

typedef struct tagMeshPath // Mesh
{
	wstring		wstrGroupTag;			//Bush , Castle등등의 그룹 이름
	wstring		wstrFileTag;			//.X가 포함된 오브젝트 이름
	wstring		wstrFullPath;			//전체 경로를 알아내자
	wstring		wstrObjTag;				//오브젝트 이름
}MESHPATH;

typedef struct tagAlphaTexture
{
	_float		fAlpahUV[4];
	_int		iTextureNum[4];
}TEXINFO;

const _ulong VTXTEX_FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

typedef struct tagIndex16
{
	WORD	_1, _2, _3;
}INDEX16;

typedef struct tagIndex32
{
	DWORD	_1, _2, _3;
}INDEX32;


#endif // Tool_Struct_h__
