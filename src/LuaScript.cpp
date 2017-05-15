/** 
 * @file LuaScript.cpp
 * @author Florian Bauer
 * 
 * This class serves as a wrapper for the Lua libraries to encapsulate and control the
 * corresponding Lua scripts on a higher abstraction level.
 */
#include "LuaScript.h"
#include <iostream>
#include <cstring>

using namespace std;

/**
 * Destructor. Closes the Lua script.
 */
LuaScript::~LuaScript()
{
    lua_close(m_L);
}

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
    script.m_L = luaL_newstate();
    luaL_openlibs(script.m_L); // load the Lua libraries
    if (script.loadScript(scriptFile))
    {
        throw;
    }
    return script;
}

int LuaScript::loadScript(const string& fileName) noexcept
{
    // load but don't run the Lua script 
    int retVal = luaL_loadfilex(m_L, fileName.c_str(), nullptr);
    if (retVal)
    {
        cerr << "luaL_loadfiles() failed\n";
    }
    return retVal;
}

/**
 * Executes the given Lua script.
 * 
 * @return 0 on success, otherwise a negative value
 */
int LuaScript::executeScript() noexcept
{
    int retVal = lua_pcallk(m_L, 0, 0, 0, 0, nullptr); // run the loaded Lua script
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
        error = luaL_loadbuffer(m_L, buf, strlen(buf), "line") || lua_pcall(m_L, 0, 0, 0);
        if (error)
        {
            cerr << lua_tostring(m_L, -1);
            lua_pop(m_L, 1); // pop error message from the stack
        }
    }
}

/**
 * Prints the actual Lua stack to `stdout`.
 */
void LuaScript::dumpStack() noexcept
{
    const int top = lua_gettop(m_L); // number of stack elements
    int type;
    for (int i = 1; i <= top; i++)
    {
        type = lua_type(m_L, i);
        switch (type)
        {
            case LUA_TNIL:
                cout << "nil";
                break;
            case LUA_TSTRING:
                cout << lua_tostring(m_L, i);
                break;
            case LUA_TBOOLEAN:
                cout << boolalpha << lua_toboolean(m_L, i);
                break;
            case LUA_TNUMBER:
                cout << lua_tonumber(m_L, i);
                break;
            default:
                cout << lua_typename(m_L, type);
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
    lua_getglobal(m_L, varName.c_str());
    if (!lua_isnumber(m_L, -1))
    {
        cerr << "get_global_int: not a number\n";
        dumpStack();
        return -1;
    }

    int retVal = static_cast<int> (lua_tonumber(m_L, -1));
    lua_pop(m_L, 1);
    return retVal;
}
