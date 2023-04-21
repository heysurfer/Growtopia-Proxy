#pragma once
#ifndef PCH_CLASS
#define PCH_CLASS
#define _CRT_SECURE_NO_WARNINGS
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <string>
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tchar.h>
#include <excpt.h>
#include <filesystem>
#else
#include <cstdint>
#include <stdint.h>
#include <experimental/filesystem>
#endif
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string_view>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <stdint.h>
#include <functional>
#include <map>
#include <string>
#include <memory>
#include <chrono>
#include <ctime>
#include <unordered_set>
#include <list>
#include <queue>
#include <stack>
#include <thread>
#include <set>

#include <sstream>
#include <fstream>

#define BOOST_ASSERT_MSG(expr, msg) assert((expr)&&(msg))

#ifndef _WIN32
#include "../library/enet/callbacks.c"
#include "../library/enet/compress.c"
#include "../library/enet/host.c"
#include "../library/enet/list.c"
#include "../library/enet/packet.c"
#include "../library/enet/peer.c"
#include "../library/enet/protocol.c"
#include "../library/enet/unix.c"
#endif

#include "../library/enet/include/enet.h"
#include "../library/binaryReader/binaryReader.h"
#include "../library/GrowtopiaHelper/GrowtopiaHelper.h"

#include "../struct/Player.h"

enum getType
{
	Local = 0,
	Growtopia,
};
struct _ConnnectInformation
{
	std::string IP = "";
	uint32_t port = 0;
	getType type = getType::Growtopia;
	int type2 = false;
};
typedef _ConnnectInformation ConnnectInformation;

#include "../library/httplib/httplib.h"
#include "../struct/World.h"

std::string FormatStr(const char* fmt, ...)
{
	char buf[8192];
	va_list arg;
	va_start(arg, fmt);
	vsnprintf(buf, sizeof(buf) - 1, fmt, arg);
	va_end(arg);
	return std::string(buf);
}
void Print(const char* fmt, ...)
{
	char buf[8192];
	va_list arg;
	va_start(arg, fmt);
	vsnprintf(buf, sizeof(buf) - 1, fmt, arg);
	va_end(arg);
	printf("%s\n", std::string(buf).c_str());
}
#include "../Class/EnetManager.h" /*peer & host manager*/
#include "../Class/HttpServer.h" /*Local HTTP(S) Server*/
#include "../library/captchaSolver/CaptchaSolver.h"

#include  "../Class/Information.h"
info* m_Info = new info();
#include "GrowtopiaProxy.h"/*Functions*/
#include "../Class/ServerHandle.h"/*Handle Server*/
serverHandle* serverHandler = new serverHandle();

#endif