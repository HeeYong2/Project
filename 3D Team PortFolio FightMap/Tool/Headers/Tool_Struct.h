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
	_vec3	vPos;			//Object�� ��ġ
	_vec3	vScale;			//Object�� ũ��
	_vec3	vAngle;			//Object�� ȸ��
	_bool	isInstancing;
	_bool	isNeedUV;
	TCHAR	szName[32];		//Object���� ���� �̸�(Monster��� Soldier�� Obstacle�̶�� tree , bush ���)
}OBJINFO;
		//����� ����ü

typedef struct tagMeshPath // Mesh
{
	wstring		wstrGroupTag;			//Bush , Castle����� �׷� �̸�
	wstring		wstrFileTag;			//.X�� ���Ե� ������Ʈ �̸�
	wstring		wstrFullPath;			//��ü ��θ� �˾Ƴ���
	wstring		wstrObjTag;				//������Ʈ �̸�
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
