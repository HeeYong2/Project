/*!
 * \class CShader
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
 * \date 4월 2015
 *
 * Contact: user@company.com
 *
 */
#ifndef Shader_h__
#define Shader_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CShader : public Engine::CBase
{
public:
	explicit CShader(LPDIRECT3DDEVICE9 pGraphicDev);
	~CShader(void);
public: // Getter
	LPD3DXEFFECT Get_Effect(void) {
		return m_pEffect;}
public:
	HRESULT Init_Shader(const TCHAR* pFilePath);
	_ulong Release(void);
public:
	static CShader* Create(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pFilePath);
private:
	LPD3DXEFFECT					m_pEffect; // 셰이더파일하나를 대표하는 객체
	LPD3DXBUFFER					m_pErrBuffer;
	LPDIRECT3DDEVICE9				m_pGraphicDev;
};

END

#endif // Shader_h__