/** 
 * @file main.cpp
 * @author Florian Bauer
 *
 * This file contains the `main()`-function with a simple Lua script test. See 
 * https://github.com/jeremyong/Selene for usage instructions.
 */

#include "selene.h"
#include <lua.hpp>
#include "Utilities.h"
#include <iostream>

using namespace std;

/**
 * The main application only for testing purposes. 
 * 
 * @param argc: the number of arguments
 * @param argv: the argument list
 * @return 0 on success, otherwise a negative value
 */
int main(int argc, char** argv)
{
    const string testScript = "tests/testscript01.lua";

    if (utils::existsFile(testScript))
    {
        sel::State state;
        state.Load(testScript);

        cout << "The Answer: " << int(state["theAnswer"]) << '\n';
        const string cmd = state["bashCommand"];
        cout << "The Bash command everyone should try out: \"" << cmd << "\"\n";
    }
    
    return 0;
}
