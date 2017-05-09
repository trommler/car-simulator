#include <iostream>
#include <string.h>
#include "lua.hpp"

// reads a lua command from stdin and executes it
// exit with CTRL-D
void simple_lua_interpreter(lua_State *L)
{
    char buf[256];
    int error;

    while (fgets(buf, sizeof(buf), stdin) != NULL)
    {
        error = luaL_loadbuffer(L, buf, strlen(buf), "line") || lua_pcall(L, 0, 0, 0);
        if (error)
        {
            fprintf(stderr, "%s", lua_tostring(L, -1));
            lua_pop(L, 1);          /* pop error message from the stack */
        }
    }
}

// print the contents of the lua stack to stdout
void stack_dump(lua_State *L, const char *msg = "")
{
    int top = lua_gettop(L); // number of stack elements
    printf("stack %s:\n", msg);

    for (int i = 1; i <= top; ++i)
    {
        int t = lua_type(L, i);
        switch(t)
        {
            case LUA_TNIL:
                printf("nil");
                break;
            case LUA_TSTRING:
                printf("'%s'", lua_tostring(L, i));
                break;
            case LUA_TBOOLEAN:
                printf(lua_toboolean(L, i) ? "true" : "false");
                break;
            case LUA_TNUMBER:
                printf("%g", lua_tonumber(L, i));
                break;
            default:
                printf("%s", lua_typename(L, t));
                break;
        }
        printf("   ");
    }
    printf("\n");
}

// load a variable named 'var_name' from lua, the lua file has to be loaded and compiled before
int get_global_int(lua_State *L, const char *var_name)
{
    lua_getglobal(L, var_name);
    if (!lua_isnumber(L, -1))
    {
        fprintf(stderr, "get_global_int: not a number\n");
        stack_dump(L, "[error in get_global_int]");
        exit(-1);
    }
    int value = (int)lua_tonumber(L, -1);
    lua_pop(L, 1);
    return value;
}


int main(int argc, char **argv)
{
    int error;
    lua_State *L = lua_open();      /* open lua */
    luaL_openlibs(L);               /* open lua librarys */
    error = lua_checkstack(L, 50);  /* ensure the lua stack can hold 50 elements, it's LUA_MINSTACK by default */
    if (error == 0)
    {
        fprintf(stderr, "stack cannot hold 50 elements\n");
        return -1;
    }

    char buf1[3] = {'a', 'b', 'c'}; // not null-terminated string
    const char *buf2 = "abc";       // null-terminated string

    // lua uses a stack to exchange values between lua an c/c++
    //
    // push some values onto the lua stack
    // indices from the bottom start at 1,
    // if you index from the top of the stack down, it's -1, -2, ...
    // => -1 is the index of the element that was pushed last
    lua_pushnil(L);                             // push nil
    lua_pushboolean(L, 1);                      // bool in c is int
    lua_pushnumber(L, 33.3);                    // push a double
    lua_pushlstring(L, buf1, sizeof(buf1));     // not null-terminated string
    lua_pushstring(L, buf2);                    // null-terminated string

    // dump stack
    stack_dump(L, "after pushing some values");

    // these check if a value can be converted into the queried type
    printf("index 1 is number? : %d\n", lua_isnumber(L, 1));
    printf("index 3 is number? : %d\n", lua_isnumber(L, 3));
    printf("index 3 is string? : %d\n", lua_isstring(L, 3));
    printf("index 5 is string? : %d\n", lua_isstring(L, 5));

    // query values from the stack, without poping them
    printf("get at index 1, convert to bool: %d\n", lua_toboolean(L, 1));
    printf("get at index 2, convert to bool: %d\n", lua_toboolean(L, 2));

    printf("num elements on the stack: %d\n", lua_gettop(L));
    lua_pop(L, 2);                              // discard the top n elements (here n=2)
    printf("num elements on the stack: %d\n", lua_gettop(L));
    lua_settop(L, 7);                           // fills the stack with nil values if necessary
    printf("num elements on the stack: %d\n", lua_gettop(L));
    // btw.: lua_pop(L, n) is the same as lua_settop(L, -(n)-1)

    // dump stack
    stack_dump(L, "after some manipulations");

    // there are also:
    // lua_remove(L, index)         -- remove element at the given idex
    // lua_insert(L, index)         -- move the top element to position index, shift other values up
    // lua_replace(L, index)        -- pop value from top, and replace value at index with it
    // lua_pushvalue(L, index)      -- copy value at index and push on top


    //simple_lua_interpreter(L);        // start the simple interpreter (CTRL-D to exit)


    // lua config file example
    //
    const char *filename = "extension.lua";

    // load and compile the lua file
    if (luaL_loadfile(L, filename) || lua_pcall(L, 0, 0, 0))
        fprintf(stderr, "cannot load file: %s", lua_tostring(L, -1));

    // load two global vars from the lua file
    int width  = get_global_int(L, "width");
    int height = get_global_int(L, "height");
    printf("width=%d, height=%d\n", width, height);


    // how to call a function defined in lua
    lua_getglobal(L, "my_pow");         // put function on the stack
    lua_pushnumber(L, 2);               // push 1st argument
    lua_pushnumber(L, 3);               // push 2nd argument
    stack_dump(L, "after pushing function and params");

    // call the function with 2 arguments and put 1 result on the stack
    if (lua_pcall(L, 2, 1, 0))
    {
        fprintf(stderr, "error in calling my_pow\n");
        return -1;
    }
    stack_dump(L, "after function call");

    // fetch result of function call from the stack
    int result = lua_tonumber(L, -1);
    lua_pop(L, 1);
    printf("result=%d\n", result);
    stack_dump(L, "after result retrieval");

    lua_close(L);
    return 0;
}


