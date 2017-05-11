/** 
 * @file LuaScript.cpp
 * @author Florian Bauer
 * 
 * This encapsulates the basic functions supported by the Lua libraries into a easy to use class
 * object to control the corresponding script in C++ style.
 */
#include "LuaScript.h"
#include <iostream>
#include <cstring>
#include <locale>

using namespace std;

/**
 * Constructor.
 */
LuaScript::LuaScript() { }

/**
 * Destructor.
 */
LuaScript::~LuaScript() { }

/**
 * Loads the Lua script according to the given name. Do not forget to call `close()` after usage.
 * 
 * @param scriptFile: the scriptFile to load.
 * @return the loaded Lua script
 * @see close()
 */
LuaScript LuaScript::import_script(const std::string& scriptFile)
{
    LuaScript script;
    script.L = luaL_newstate();
    luaL_openlibs(script.L); // load the Lua libraries
    if (script.loadScript(scriptFile))
    {
        throw;
    }
    return script;
}

int LuaScript::loadScript(const string& fileName) noexcept
{
    int retVal = luaL_loadfilex(L, fileName.c_str(), nullptr); // load but don't run the Lua script 
    if (retVal)
    {
        cerr << "luaL_loadfiles() failed\n";
    }
    return retVal;
}

/**
 * Executes the given Lua script.
 * 
 * @return 0 on sucess, otherwise a negative value
 */
int LuaScript::executeScript() noexcept
{
    int retVal = lua_pcallk(L, 0, 0, 0, 0, nullptr); // run the loaded Lua script
    if (retVal)
    {
        cerr << "lua_pcallk() failed\n";
    }
    return retVal;
}

/**
 * Reads a Lua command from `stdin` and executes it. Exit with CTRL-D.
 */
void LuaScript::simpleLuaInterpreter() noexcept
{
    char buf[256];
    int error;

    while (fgets(buf, sizeof(buf), stdin) != NULL)
    {
        error = luaL_loadbuffer(L, buf, strlen(buf), "line") || lua_pcall(L, 0, 0, 0);
        if (error)
        {
            cerr << lua_tostring(L, -1);
            lua_pop(L, 1); // pop error message from the stack
        }
    }
}

/**
 * Prints the actual Lua stack to `stdout`.
 */
void LuaScript::dumpStack() noexcept
{
    int top = lua_gettop(L); // number of stack elements
    int t;
    for (int i = 1; i <= top; i++)
    {
        t = lua_type(L, i);
        switch (t)
        {
            case LUA_TNIL:
                cout << "nil";
                break;
            case LUA_TSTRING:
                cout << lua_tostring(L, i);
                break;
            case LUA_TBOOLEAN:
                cout << boolalpha << lua_toboolean(L, i);
                break;
            case LUA_TNUMBER:
                cout << lua_tonumber(L, i);
                break;
            default:
                cout << lua_typename(L, t);
                break;
        }
        cout << '\t';
    }
    cout << '\n';
}

/**
 * Load a variable named `varName` from Lua, the Lua file has to be loaded and compiled before
 * 
 * @param varName: the variable name
 * @return 0 on success, otherwise a negative value
 */
int LuaScript::getGlobalInt(const string& varName) noexcept
{
    lua_getglobal(L, varName.c_str());
    if (!lua_isnumber(L, -1))
    {
        cerr << "get_global_int: not a number\n";
        dumpStack();
        return -1;
    }

    int retVal = static_cast<int> (lua_tonumber(L, -1));
    lua_pop(L, 1);
    return retVal;
}

/**
 * Closes the Lua script.
 */
void LuaScript::close() noexcept
{
    lua_close(L);
}
