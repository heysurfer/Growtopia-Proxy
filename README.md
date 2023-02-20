## Growtopia-Proxy
Growtopia proxy that allows modification and viewing of packets, and creation of new ones.

## Supported Platforms
Currently Only Winodows.

## Build
- Install <a href="https://visualstudio.microsoft.com/downloads/"> Visual Studio  </a>  Required C++20 <a href="https://learn.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=msvc-170"> Visual Studio Installation</a> 
- Install VCPKG For c++ Libraries <a href ="https://www.youtube.com/watch?v=F4TY_nV5cn8">Install cpp libraries using vcpkg and including them in visual studio</a>
- openssl installation command : vcpkg install openssl:x64-windows

## Custom Command
Example Command: 
```c++
Commands["test"] = [=](std::string Text){
    m_Info->send_log("Detected 'Test' Command");
};
```
Example CallBack for OnCallFunction : 
```c
callbackStruct _callbackStruct;
_callbackStruct.target_Case = ("OnDialogRequest");
_callbackStruct.target_String = ("The Growtopia Gazette");
_callbackStruct.target_Function = [=](std::string &Dialog) 
{
utils::ReplaceAll(Dialog, "The Growtopia Gazette", "the Proxy Gazette");
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