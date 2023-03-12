## Growtopia-Proxy
Growtopia proxy that allows modification and viewing of packets, and creation of new ones.

## Supported Platforms
- Winodows
- Linux

## Build For Windows
- Install <a href="https://visualstudio.microsoft.com/downloads/"> Visual Studio  </a>  Required C++20 <a href="https://learn.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=msvc-170"> Visual Studio Installation</a> 
- Install VCPKG For c++ Libraries <a href ="https://www.youtube.com/watch?v=F4TY_nV5cn8">Install cpp libraries using vcpkg and including them in visual studio</a>
- openssl installation command : vcpkg install openssl:x64-windows

## Build For Linux

```bash
sudo apt update
sudo apt-get install build-essential
sudo apt install gcc
sudo apt-get install libssl-dev
chmod +x LinuxBuild.sh
sudo ./LinuxBuild.sh
```

## Example Custom Command
Example Command: 
```c++
Commands["test"] = [=](std::string Text){
    m_Info->send_log("Detected 'Test' Command");
};
```
```c++
Commands["convertwl"] = [=](std::string Text){
    GameUpdatePacket packet{ };
    packet.type = PACKET_ITEM_ACTIVATE_REQUEST;
    packet.int_data = 242; //world lock id
    m_Info->ENetManager->sendPacket(packet, getType::Growtopia);
};
```
```c++
Commands["warp"] = [=](std::string Text) {
	m_Info->ENetManager->sendPacket("action|join_request\nname|" + Text+"\ninvitedWorld|0", getType::Growtopia, NET_MESSAGE_GAME_MESSAGE);
    m_Info->send_log("Warping to " + Text);
};
```
## Example Callback

Example CallBack for OnCallFunction : 
```c
callbackStruct _callbackStruct;
_callbackStruct.target_Case = ("OnDialogRequest");
_callbackStruct.target_String = ("The Growtopia Gazette");
_callbackStruct.target_Function = [=](std::string &Dialog) 
{
    utils::ReplaceAll(Dialog, "The Growtopia Gazette", "the Proxy Gazette");
    // Replaces the string on varlist and sends the packet to the client.
    return false;
};
_callbackStruct.infinity = true;
callBack_CALL_FUNCTION[fnv32("replace_Gazette")] = _callbackStruct;
```

Example CallBack for OnCallFunction : 
```c
callbackStruct _callbackStruct;
_callbackStruct.target_Case = ("OnDialogRequest");
_callbackStruct.target_String = ("The Growtopia Gazette");
_callbackStruct.target_Function = [=](std::string Dialog) 
{
    m_Info->send_log("Blocked Growtopia Gazette");
    return true;//block dialog 
};
_callbackStruct.infinity = true;
callBack_CALL_FUNCTION[fnv32("block_Gazette")] = _callbackStruct;
```
