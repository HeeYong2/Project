

template <typename T> HRESULT Engine::CManagement::Replace_Current_Scene(LPDIRECT3DDEVICE9 pGraphicDev, T& Functor)
{
	Engine::safe_delete(m_pCurrentScene);

	HRESULT			hr = 0;

	hr = Functor(pGraphicDev, &m_pCurrentScene);

	if(SUCCEEDED(hr))
	{
		m_pRenderer->Receive_CurrentScene(m_pCurrentScene);
	}

	return hr;
}