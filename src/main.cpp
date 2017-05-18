/** 
 * @file main.cpp
 * @author Florian Bauer
 *
 * This file contains the `main()`-function with a simple Lua script test.
 */

#include <LuaBridge.h>
#include <lua.hpp>
#include "Utilities.h"
#include <cstring>
#include <iostream>

using namespace luabridge;

/**
 * The main application only for testing purposes. 
 * 
 * @param argc: the number of arguments
 * @param argv: the argument list
 * @return 0 on success, otherwise a negative value
 */
int main(int argc, char** argv)
{
    const std::string testScript = "tests/testscript01.lua";

    if (utils::existsFile(testScript))
    {
        lua_State* L = luaL_newstate();
        luaL_dofile(L, testScript.c_str());
        luaL_openlibs(L);
        lua_pcall(L, 0, 0, 0);
        LuaRef s = getGlobal(L, "bashCommand");
        LuaRef n = getGlobal(L, "theAnswer");

        std::string luaString = s.cast<std::string>();
        std::cout << "The Bash command everyone should try out: \"" << luaString << "\"\n";
        int answer = n.cast<int>();
        std::cout << "The Answer: " << answer << '\n';
    }
}
