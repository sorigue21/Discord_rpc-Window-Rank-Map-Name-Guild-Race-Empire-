#ifdef ENABLE_DISCORD_RPC
#include "discord_rpc.h"
#ifdef _DEBUG
#pragma comment(lib, "discord_rpc_d.lib")
#else
#pragma comment(lib, "discord_rpc_r.lib")
#endif
static int64_t StartTime;
static constexpr auto DiscordClientID = "";
auto szSiteName = "www.tenebris2.international";
auto szIconName = "image1";
void CPythonNetworkStream::Discord_Start()
{
	StartTime = time(0);
	DiscordEventHandlers handlers;
	memset(&handlers, 0, sizeof(handlers));
	Discord_Initialize(DiscordClientID, &handlers, 1, nullptr);
	Discord_Update(true, false, false);
}
void CPythonNetworkStream::Discord_Update(const bool login, const bool select, const bool loading)
{
	DiscordRichPresence discordPresence;
	memset(&discordPresence, 0, sizeof(discordPresence));
	char szBufName[256+1];
	char szBufWarp[256+1];
	char szEmpireId[256+1];
	char szEmpireName[256+1];
	char szRaceName[256+1];
	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetMainInstancePtr();
	
	if (login)
	{
		discordPresence.details = "On Login";
		discordPresence.state = szSiteName;
		discordPresence.largeImageKey = szIconName;
		discordPresence.largeImageText = szSiteName;
	}
	else if (select)
	{
		discordPresence.details = "On Select";
		discordPresence.state = szSiteName;
		discordPresence.largeImageKey = szIconName;
		discordPresence.largeImageText = szSiteName;
	}
	else if (loading)
	{
		discordPresence.details = "On Loading";
		discordPresence.state = szSiteName;
		discordPresence.largeImageKey = szIconName;
		discordPresence.largeImageText = szSiteName;
	}
	else
	{
		std::string GuildName;
		DWORD GuildID = CPythonPlayer::Instance().GetGuildID();
		CPythonGuild::Instance().GetGuildName(GuildID, &GuildName);
		
		if (GuildID != 0)
			_snprintf(szBufName, sizeof(szBufName), "Name: %s - Guild: %s", CPythonPlayer::Instance().GetName(), &GuildName);
		else
			_snprintf(szBufName, sizeof(szBufName), "Name: %s", CPythonPlayer::Instance().GetName());
		
		_snprintf(szBufWarp, sizeof(szBufWarp), "Location: %s", CPythonBackground::Instance().GetMapName());
		_snprintf(szEmpireId, sizeof(szEmpireId), "%s", GetEmpireIcon());
		_snprintf(szEmpireName, sizeof(szEmpireName), "Empire: %s", GetEmpireName());
		_snprintf(szRaceName, sizeof(szRaceName), "Race: %s", GetRaceName());
		std::string szName = pInstance->GetNameString();
		
		if (pInstance->IsGameMaster())
		{
			discordPresence.details = GetStaffRank();
			discordPresence.state = szBufName;
		}
		else if (szName.find("[VET]") != std::string::npos)
		{
			discordPresence.details = "Rank: Veteran";
			discordPresence.state = szBufName;
		}
		else if (szName.find("[GFX]") != std::string::npos)
		{
			discordPresence.details = "Rank: Graphic Designer";
			discordPresence.state = szBufName;
		}
		else if (szName.find("[PM]") != std::string::npos)
		{
			discordPresence.details = "Rank: Promoter";
			discordPresence.state = szBufName;
		}
		else
		{
			discordPresence.details = szBufName;
			discordPresence.state = szBufWarp;
		}
		
		discordPresence.largeImageKey = GetRaceIcon();
		discordPresence.largeImageText = szRaceName;
		discordPresence.smallImageKey = GetEmpireIcon();
		discordPresence.smallImageText = szEmpireName;
	}
	
	discordPresence.startTimestamp = StartTime;
	Discord_UpdatePresence(&discordPresence);
}
void CPythonNetworkStream::Discord_Close()
{
	Discord_Shutdown();
}

const char * CPythonNetworkStream::GetStaffRank()
{
	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetMainInstancePtr();
	std::string szName = pInstance->GetNameString();
	
	if (szName.find("[SA]") != std::string::npos)
		return "Rank: Server Admin";
	else if (szName.find("[TM]") != std::string::npos)
		return "Rank: Team Manager";
	else if (szName.find("[GA]") != std::string::npos)
		return "Rank: Game Admin";
	else if (szName.find("[GM]") != std::string::npos)
		return "Rank: Game Master";
	else if (szName.find("[H]") != std::string::npos)
		return "Rank: Helper";
	else if (szName.find("[MID]") != std::string::npos)
		return "Rank: Middleman";
	else
		return szSiteName;
}

const char * CPythonNetworkStream::GetRaceIcon()
{
	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetMainInstancePtr();
	int raceId = pInstance->GetRace();
	
	if (raceId == 0)
		return "warrior_m";
	else if (raceId == 1)
		return "ninja_w";
	else if (raceId == 2)
		return "sura_m";
	else if (raceId == 3)
		return "shaman_w";
	else if (raceId == 4)
		return "warrior_w";
	else if (raceId == 5)
		return "ninja_m";
	else if (raceId == 6)
		return "sura_w";
	else if (raceId == 7)
		return "shaman_m";
#ifdef ENABLE_WOLFMAN_CHARACTER
	else if (raceId == 8)
		return "wolfman_m";
#endif
	else
		return szIconName;
}

const char * CPythonNetworkStream::GetRaceName()
{
	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetMainInstancePtr();
	int raceId = pInstance->GetRace();
	
	if (raceId == 0 || raceId == 4)
		return "Warrior";
	else if (raceId == 1 || raceId == 5)
		return "Ninja";
	else if (raceId == 2 || raceId == 6)
		return "Sura";
	else if (raceId == 3 || raceId == 7)
		return "Shaman";
#ifdef ENABLE_WOLFMAN_CHARACTER
	else if (raceId == 8)
		return "Wolfman";
#endif
	else
		return szSiteName;
}

const char * CPythonNetworkStream::GetEmpireIcon()
{
	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetMainInstancePtr();
	int empireId = pInstance->GetEmpireID();
	
	if (empireId == 1)
		return "empire_a";
	else if (empireId == 2)
		return "empire_b";
	else if (empireId == 3)
		return "empire_c";
	else
		return szIconName;
}

const char * CPythonNetworkStream::GetEmpireName()
{
	CInstanceBase* pInstance = CPythonCharacterManager::Instance().GetMainInstancePtr();
	int empireId = pInstance->GetEmpireID();
	
	if (empireId == 1)
		return "Shinsoo (Red)";
	else if (empireId == 2)
		return "Chunjo (Yellow)";
	else if (empireId == 3)
		return "Jinno (Blue)";
	else
		return szSiteName;
}
#endif

