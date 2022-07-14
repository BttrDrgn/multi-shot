#include <sdk/SexySDK.hpp>
#include <callbacks/callbacks.hpp>

int max_shots = 3;
int shots = 0;
bool end_turn = false;

void init()
{
	callbacks::on(callbacks::type::beginturn2, []()
	{
		if(end_turn)
		{
			shots = 0;
			end_turn = false;
		}
	});

	callbacks::after_begin_shot([](auto logic_mgr, auto do_get_replay_shots)
	{
		if (shots < (max_shots - 1) && !end_turn)
		{
			++shots;
			Sexy::LogicMgr::BeginTurn2();
		}
		else if (shots >= (max_shots - 1))
		{
			end_turn = true;
		}
	});
}

DWORD WINAPI OnAttachImpl(LPVOID lpParameter)
{
	init();
	return 0;
}

DWORD WINAPI OnAttach(LPVOID lpParameter)
{
	__try
	{
		return OnAttachImpl(lpParameter);
	}
	__except (0)
	{
		FreeLibraryAndExitThread((HMODULE)lpParameter, 0xDECEA5ED);
	}

	return 0;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
		CreateThread(nullptr, 0, OnAttach, hModule, 0, nullptr);
		return true;
	}

	return false;
}
