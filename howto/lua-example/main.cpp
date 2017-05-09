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
void stack_dump(lua_State *L)
{
    int top = lua_gettop(L); // number of stack elements
    printf("stack:\n");

    for (int i = 1; i <= top; ++i)
    {
        int t = lua_type(L, i);
        switch(t)
        {
            case LUA_TNIL:
                printf("nil");
                break;
            case LUA_TSTRING:
                printf("%s", lua_tostring(L, i));
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
    stack_dump(L);

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
    stack_dump(L);

    // there are also:
    // lua_remove(L, index)         -- remove element at the given idex
    // lua_insert(L, index)         -- move the top element to position index, shift other values up
    // lua_replace(L, index)        -- pop value from top, and replace value at index with it
    // lua_pushvalue(L, index)      -- copy value at index and push on top


    //simple_lua_interpreter(L);

    lua_close(L);
    return 0;
}
