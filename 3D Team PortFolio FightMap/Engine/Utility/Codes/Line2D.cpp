#include "Line2D.h"

using namespace Engine;

//---------------------------------------------------------------------------------------------------------------------------------------
Engine::CLine2D::CLine2D(void)
{

}

//---------------------------------------------------------------------------------------------------------------------------------------

Engine::CLine2D::~CLine2D(void)
{
	
}

//---------------------------------------------------------------------------------------------------------------------------------------

HRESULT Engine::CLine2D::Init_Line(const _vec2* pStart , const _vec2* pEnd)
{
	m_vStart = *pStart;
	m_vEnd	= *pEnd;

	_vec2		vDir = m_vEnd - m_vStart;
	m_vNormal = _vec2(-vDir.y , vDir.x);

	D3DXVec2Normalize(&m_vNormal , &m_vNormal);

	return S_OK;
}

const _vec2* Engine::CLine2D::Get_Line_Start(void)
{
	return &m_vStart;
}

const _vec2* Engine::CLine2D::Get_Line_End(void)
{
	return &m_vEnd;
}

//---------------------------------------------------------------------------------------------------------------------------------------
_bool CLine2D::isIn(const _vec2* pPosition , const _vec2* pDirection)
{
	_vec2		vGoal = (*pPosition + *pDirection);
	_vec2		vProj = vGoal - m_vStart;
	_float		fResult = D3DXVec2Dot(&vProj , &m_vNormal);

	if(fResult >= 0)
		return false;

	return true;
}


void CLine2D::MakeSlide(const _vec2* pDirection, _vec3* pSlide)
{
	_vec2 vLine = m_vEnd - m_vStart;
	D3DXVec2Normalize(&vLine, &vLine);

	_float fDist = D3DXVec2Dot(pDirection, &vLine);

	_vec2 vResult = vLine * fDist;
	D3DXVec2Normalize(&vResult, &vResult);

	pSlide->x = vResult.x;
	pSlide->z = vResult.y;

	(*pSlide) *= 0.05f;
}
//---------------------------------------------------------------------------------------------------------------------------------------

CLine2D * CLine2D::Create(const _vec2* pStart , const _vec2* pEnd )
{
	CLine2D*		pLine = new CLine2D;
	
	if(FAILED(pLine->Init_Line(pStart , pEnd)))
	{
		Engine::safe_delete(pLine);
		return pLine;
	}
	return pLine;
}
//---------------------------------------------------------------------------------------------------------------------------------------

_vec3 CLine2D::GetNormal(void)
{
	return _vec3(m_vNormal.x , 0.f , m_vNormal.y);
}