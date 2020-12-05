	public:
		CPythonNetworkStream();
		virtual ~CPythonNetworkStream();
		bool SendSpecial(int nLen, void * pvBuf);
		void StartGame();
		void Warp(LONG lGlobalX, LONG lGlobalY);
		void NotifyHack(const char* c_szMsg);
		void SetWaitFlag();
#ifdef ENABLE_DISCORD_RPC
		void Discord_Start();
		void Discord_Close();
		void Discord_Update(const bool login, const bool select, const bool loading);
		const char * GetStaffRank();
		const char * GetRaceIcon();
		const char * GetRaceName();
		const char * GetEmpireIcon();
		const char * GetEmpireName();
#endif