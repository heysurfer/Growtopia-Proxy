#if ((defined(_MSVC_LANG) && _MSVC_LANG != 202002L))
# error Required C++ 20
#endif

#include "pch.h"

int main()
{
	m_Info->init();
	while (true)
	{
		serverHandler->poll();
		Sleep(1);
	}

}