#include <iostream>
#include <Windows.h>
#include <string>
#include "mem.h"
#include "bools.h"

bool_int dwLocalPlayer = {
	{
		{ false, false, false, false, false, false, false, false },
		{ true, true, false, true, true, false, true, true },
		{ false, true, true, false, false, true, false, true },
		{ true, true, true, false, true, true, false, false }
	}
};

bool_int fFlags = {
	{
		{ false, false, false, false, false, false, false, false },
		{ false, false, false, false, false, false, false, false },
		{ false, false, false, false, false, false, false, true },
		{ false, false, false, false, false, true, false, false },
	}
};

bool_int dwForceJump = {
	{
		{ false, false, false, false, false, true, false, true },
		{ false, false, true, false, false, true, true, true },
		{ true, false, true, true, true, true, false, false },
		{ true, false, false, true, true, false, false, false }
	}
};

bool_int dwEntityList = {
	{
		{ false, false, false, false, false, true, false, false },
		{ true, true, false, true, true, true, false, true },
		{ false, false, false, true, true, true, true, false },
		{ false, false, false, true, true, true, false, false }
	}
};

bool_int iTeamNum = {
	{
		{ false, false, false, false, false, false, false, false },
		{ false, false, false, false, false, false, false, false },
		{ false, false, false, false, false, false, false, false },
		{ true, true, true, true, false, true, false, false }
	}
};

bool_int bSpotted = {
	{
		{ false, false, false, false, false, false, false, false },
		{ false, false, false, false, false, false, false, false },
		{ false, false, false, false, true, false, false, true },
		{ false, false, true, true, true, true, false, true }
	}
};

int main() {
	const Memory csgo = Memory(L"csgo.exe");

	std::cout << "csgo is started" << std::endl;
	
	bool_int client = encode(csgo.GetModuleAddress("client.dll"));
	std::cout << "client: 0x" << std::hex << client.decode() << std::endl;

	while (true) {
		bool_int localPlayer = encode(csgo.Read<std::uintptr_t>(client.decode() + dwLocalPlayer.decode()));
		bool_int localPlayerFlags = encode(csgo.Read<std::uintptr_t>(localPlayer.decode() + fFlags.decode()));

		if (!localPlayer.is())
			continue;

		// bhop
		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
			csgo.Write<std::uintptr_t>(client.decode() + dwForceJump.decode(), localPlayerFlags.decode() & (true << false) ? true + true + true + true + true + true : true + true + true + true);

		bool_int localTeam = encode(csgo.Read<std::uintptr_t>(localPlayer.decode() + iTeamNum.decode()));

		// radar
		for (int i = 1; i <= 64; i++) {
			bool_int entity = encode(csgo.Read<std::uintptr_t>(client.decode() + dwEntityList.decode() + (i * 0x10)));
			bool_int entityTeam = encode(csgo.Read<std::uintptr_t>(entity.decode() + iTeamNum.decode()));

 			if (entityTeam.decode() != localTeam.decode())
				csgo.Write<bool>(entity.decode() + bSpotted.decode(), true);
		}
		Sleep(1);
	}
}