#include "AnimationController.h"

USING(Engine)

Engine::CAnimationController::CAnimationController(void)
: m_pAniCtrl(NULL)
, m_iMaxAniSet(0)
, m_dwCurrentTrack(0)
, m_dwNewTrack(1)
, m_fCurrentTime(0.f)
, m_pTrackInfo(new D3DXTRACK_DESC)

{

}
//---------------------------------------------------------------------------------------------------------------------------------------

Engine::CAnimationController::CAnimationController(const CAnimationController* pAniCtrl)
: m_pAniCtrl(NULL)
, m_iMaxAniSet(0)
, m_dwCurrentTrack(0)
, m_dwNewTrack(1)
, m_fCurrentTime(0.f)
, m_pTrackInfo(new D3DXTRACK_DESC)

{
	pAniCtrl->m_pAniCtrl->CloneAnimationController(pAniCtrl->m_pAniCtrl->GetMaxNumAnimationOutputs(),
		pAniCtrl->m_pAniCtrl->GetMaxNumAnimationSets()
		, pAniCtrl->m_pAniCtrl->GetMaxNumTracks()
		, pAniCtrl->m_pAniCtrl->GetMaxNumEvents()
		, &m_pAniCtrl);
	m_iMaxAniSet = pAniCtrl->m_pAniCtrl->GetMaxNumAnimationSets();
}
//---------------------------------------------------------------------------------------------------------------------------------------

Engine::CAnimationController::~CAnimationController(void)
{
	
}
//---------------------------------------------------------------------------------------------------------------------------------------

void Engine::CAnimationController::SetAnimationSet(_uint iAniIdx)
{
	if(m_iMaxAniSet <= iAniIdx)
		return;

	if(m_dwCurrentTrack == 0)
		m_dwNewTrack = 1;
	else
		m_dwNewTrack = 0;

	LPD3DXANIMATIONSET				pAniSet = NULL;

	m_pAniCtrl->GetAnimationSet(iAniIdx, &pAniSet);
	m_pAniCtrl->SetTrackAnimationSet(m_dwNewTrack, pAniSet);

	m_Period = pAniSet->GetPeriod();
	pAniSet->Release();

	m_pAniCtrl->UnkeyAllTrackEvents(m_dwCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_dwNewTrack);

	m_pAniCtrl->KeyTrackEnable(m_dwCurrentTrack, FALSE, m_fCurrentTime + 0.25f);
	m_pAniCtrl->KeyTrackSpeed(m_dwCurrentTrack, 1.f, m_fCurrentTime, 0.25f, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_dwCurrentTrack, 0.f, m_fCurrentTime, 0.25f, D3DXTRANSITION_LINEAR);

	m_pAniCtrl->SetTrackEnable(m_dwNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_dwNewTrack, 1.f, m_fCurrentTime, 0.25f, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_dwNewTrack, 1.f, m_fCurrentTime, 0.25f, D3DXTRANSITION_LINEAR);

	m_pAniCtrl->SetTrackPosition(m_dwNewTrack, 0.0);

	m_pAniCtrl->ResetTime();

	m_dwCurrentTrack = m_dwNewTrack;
	m_fCurrentTime = 0.f;
}
//---------------------------------------------------------------------------------------------------------------------------------------

_bool Engine::CAnimationController::GetEndPeriod(void)
{
	m_pAniCtrl->GetTrackDesc(m_dwCurrentTrack, m_pTrackInfo);	
	if(m_Period <= m_pTrackInfo->Position)
		return true;

	return false;
}
//---------------------------------------------------------------------------------------------------------------------------------------

void CAnimationController::FrameMove(_float fTime)
{
	m_pAniCtrl->AdvanceTime(fTime, NULL);
	
	m_fCurrentTime += fTime;
}