//Script DLL example. Put the 'pluginsdk' directory from the snapshot in the same directory as this file and compile as a DLL.

#include "main.h"

using namespace Script;

extern "C" __declspec(dllexport) void AsyncStart()
{
    auto addr = DbgValFromString("debugger.pyd:$D000");
    struct PyMethodDef
    {
        char* ml_name;
        duint ml_meth;
        int ml_flags;
        char* ml_doc;
    } method;

    auto count = 0;
    while (true)
    {
        memset(&method, 0, sizeof(method));
        Memory::Read(addr, &method, sizeof(method), nullptr);
        addr += sizeof(PyMethodDef);
        if (!method.ml_name)
            break;
        count++;
        char ml_name[256] = "";
        Memory::Read(duint(method.ml_name), ml_name, sizeof(ml_name), nullptr);
        _plugin_logprintf("%p:%s\n", method.ml_meth, ml_name);
        Label::Set(method.ml_meth, ml_name);
    }
    _plugin_logprintf("%d methods found!\n", count);
}

//uncomment this to get called synchronously
//extern "C" __declspec(dllexport) void Start() {}