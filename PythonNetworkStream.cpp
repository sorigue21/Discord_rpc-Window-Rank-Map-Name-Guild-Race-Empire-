bool CPythonNetworkStream::RecvPhasePacket()
{
	TPacketGCPhase packet_phase;

	if (!Recv(sizeof(TPacketGCPhase), &packet_phase))
		return false;

	switch (packet_phase.phase)
	{
		case PHASE_CLOSE:
			ClosePhase();
			break;

		case PHASE_HANDSHAKE:
			SetHandShakePhase();
			break;

		case PHASE_LOGIN:
			SetLoginPhase();
#ifdef ENABLE_DISCORD_RPC
			Discord_Update(true, false, false);
#endif
			break;

		case PHASE_SELECT:
			SetSelectPhase();
#ifdef ENABLE_DISCORD_RPC
			Discord_Update(false, true, false);
#endif
			__DownloadMark();
			break;

		case PHASE_LOADING:
			SetLoadingPhase();
#ifdef ENABLE_DISCORD_RPC
			Discord_Update(false, false, true);
#endif
			break;

		case PHASE_GAME:
			SetGamePhase();
#ifdef ENABLE_DISCORD_RPC
			Discord_Update(false, false, false);
#endif
			break;

		case PHASE_DEAD:
			break;
	}

	return true;
}

void CPythonNetworkStream::SetOffLinePhase()
{
	if ("OffLine" != m_strPhase)
		m_phaseLeaveFunc.Run();

	m_strPhase = "OffLine";
	Tracen("");
	Tracen("## Network - OffLine Phase ##");
	Tracen("");
#ifdef ENABLE_DISCORD_RPC
	Discord_Update(true, false, false);
#endif
	m_dwChangingPhaseTime = ELTimer_GetMSec();
	m_phaseProcessFunc.Set(this, &CPythonNetworkStream::OffLinePhase);
	m_phaseLeaveFunc.Set(this, &CPythonNetworkStream::__LeaveOfflinePhase);
	SetGameOffline();
	m_dwSelectedCharacterIndex = 0;
	__DirectEnterMode_Initialize();
	__BettingGuildWar_Initialize();
}