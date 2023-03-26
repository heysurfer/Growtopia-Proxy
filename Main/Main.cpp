#if ((defined(_MSVC_LANG) && _MSVC_LANG != 202002L))
# error Required C++ 20
#endif

#include "pch.h"

int main()
{
	auto isAdministrator = [=]() {
#ifdef _WIN32

		BOOL isAdmin = FALSE;
		HANDLE hToken = NULL;
		if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
			TOKEN_ELEVATION elevation;
			DWORD size = sizeof(TOKEN_ELEVATION);
			if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &size)) {
				isAdmin = elevation.TokenIsElevated;
			}
		}
		if (hToken) {
			CloseHandle(hToken);
		}
		return isAdmin;
#else
		return (geteuid() == 0);
#endif
	};
	if (!isAdministrator())
	{
		printf("Error %s\n","Proxy failed to start. Admin permissions are required to write to the host file and start the HTTPS server");
		std::this_thread::sleep_for(std::chrono::milliseconds(3500));
		std::exit(EXIT_FAILURE);
	}
	m_Info->captchaSolverKey = "";//secret code, you can buy at  https://surferwallet.net/SurferShop
	m_Info->init();
	while (true)
	{
		serverHandler->poll();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return EXIT_FAILURE;
}