/*!
 * \class CShaderMgr
 *
 * \ingroup GroupName
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note 
 *
 * \author Administrator
 *
 * \version 1.0
 *
 * \date 4¿ù 2015
 *
 * Contact: user@company.com
 *
 */
#ifndef ShaderMgr_h__
#define ShaderMgr_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class CShader;
class ENGINE_DLL CShaderMgr
{
	DECLARE_SINGLETON(CShaderMgr)
public:
	explicit CShaderMgr(void);
	~CShaderMgr(void);
public: // Getter
	LPD3DXEFFECT Get_Effect(const TCHAR* pShaderTag);
	CShader* Find_Shader(const TCHAR* pShaderTag);
public:
	HRESULT Ready_Shader(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pShaderTag, const TCHAR* pFilePath);
private:
	map<const TCHAR*, CShader*>				m_mapShader;
	typedef map<const TCHAR*, CShader*>		MAPSHADER;
private:
	void Release(void);
};

END

#endif // ShaderMgr_h__
