
#ifndef AnimationController_h__
#define AnimationController_h__
/********************************************************************
	created:	2015/03/13
	file path:	Yong\Engine\Resource\Code
	file base:	AnimationController
	file ext:	h
	author:		Yong
	purpose:	Animation Controller
*********************************************************************/
#include "Engine_Defines.h"

namespace Engine
{
	class _declspec(dllexport) CAnimationController
	{
	public:
		explicit CAnimationController(void);
		explicit CAnimationController(const CAnimationController* pAniCtrl);
		~CAnimationController(void);
	public: // Getter
		LPD3DXANIMATIONCONTROLLER* Get_AniCtrl(void) {
			return &m_pAniCtrl;}
		_bool GetEndPeriod(void);

	public: // Setter
		void SetAnimationSet(_uint iAniIdx);
		void FrameMove(_float fTime);

	public:
		LPD3DXANIMATIONCONTROLLER			m_pAniCtrl;
		_uint								m_iMaxAniSet;
		_ulong								m_dwCurrentTrack;
		_ulong								m_dwNewTrack;
		_float								m_fCurrentTime;

	private:
		double								m_Period;
		LPD3DXTRACK_DESC					m_pTrackInfo;
	
	};
}

#endif // AnimationController_h__