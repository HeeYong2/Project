#ifndef ObjectTool_h__
#define ObjectTool_h__
/********************************************************************
	created:	2015/04/16
	file path:	Yong\Tool\Code
	file base:	ObjectTool
	file ext:	h
	author:		Yong	
	purpose:	ObjectTool
*********************************************************************/
#include "GameObject.h"
#include "Tool_Defines.h"

class CObjectTool
	: public Engine::CGameObject
{
public:
	enum LAYERID {LAYER_ENVIRONMENT , LAYER_GAMELOGIC , LAYER_UI};

	explicit CObjectTool(void);
	~CObjectTool(void);

public:
	virtual HRESULT Initialize(OBJINFO* tObjectInfo );
	virtual void Update(void);
	virtual	void Render(void);

public:
	static CObjectTool* Create(OBJINFO* tObjectInfo );
	void Render_Mesh(void);
	void Render_Texture(void);
	void Render_Buffer(void);
	void Render_Collider(void);
	
public:
	Engine::CComponent* GetMeshCom(void);

	//툴에서 넘어오는 Render관련
public:
	void SetPickingRender(_bool isPickingRender);
	void SetColliderRender(_bool isColliderRender);
	Engine::VTXTEX*	GetVtxTex(void);			//Vtx정보 가져오기

	//툴에서 넘어오는 Render수정 변수
private:
	_float					m_fPickingSizeX;
	_float					m_fPickingSizeZ;
	_float					m_fColliderSizeX;
	_float					m_fColliderSizeY;
	_float					m_fColliderSizeZ;

private:
	_ulong Release(void);
	HRESULT AddComponent(OBJINFO* ObjectInfo );

private:
	Engine::CTransform*		m_pInfo;
	Engine::CComponent*		m_pMeshCom;
	OBJINFO					m_ObjectInfo;
	
private:
	list<OBJINFO*>			m_ObjectList;
	typedef list<OBJINFO*>	LISTOBJECT;
	Engine::CGameObject*	m_pPickingBox;

private:
	Engine::CComponent*		m_pBufferCom;
	Engine::CComponent*		m_pTexBlueCom;
	Engine::CComponent*		m_pColliderCom;

private:
	_bool					m_isPickingRender;
	_bool					m_isColliderRender;

private:
	_bool					m_isCollision;

public:
	void					SetCollision(_bool	isCollision);
	_vec3					GetPos(void);

public:
	void					SetInfo(Engine::CTransform*	pTransForm);
	_matrix					GetBoxWorld(void);
	Engine::CTransform*		GetInfo(void);
	OBJINFO*				GetObjInfo(void);
	void					SetFrameType(_int iFrameNum);			//WireFrame 설정

private:
	Engine::VTXTEX*			m_pVtxInfo;
	_matrix					m_BoxmatWorld;
	_int					m_iFrameNum;

private:
	LPD3DXEFFECT			m_pEffect;
private:
	void SetConstantTable(void);

private:
	_vec3					m_vCenter;
	_float					m_fRadius;
	DWORD					m_dwVtxNum;
	DWORD					m_dwVtxByte;

private:
	_ulong					m_iTriCnt;
	_vec3					m_vTrans;
	_float					m_fLength;
	_float					m_fBig;

private:
	_bool					m_isFrustum;
	TCHAR					m_szBuffer[32];

};

#endif // ObjectTool_h__