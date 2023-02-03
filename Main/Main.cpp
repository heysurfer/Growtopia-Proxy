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