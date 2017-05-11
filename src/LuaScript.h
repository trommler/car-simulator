/** 
 * @file LuaScript.h
 * @author Florian Bauer
 *
 */
#ifndef LUASCRIPT_H
#define LUASCRIPT_H

#include <lua.hpp>
#include <string>
#include <stdexcept>
#include <iostream>
#include <memory>

class LuaScript
{
public:
    LuaScript(const LuaScript& orig) = delete;
    LuaScript(const LuaScript&& orig);
    virtual ~LuaScript();
    static LuaScript import_script(const std::string& scriptFile);
    int executeScript() noexcept;
    void simpleLuaInterpreter() noexcept;
    void dumpStack() noexcept;
    int getGlobalInt(const std::string& varName) noexcept;
    void close() noexcept;
private:
    lua_State* L = nullptr;
    LuaScript();
    int loadScript(const std::string& fileName) noexcept;
};

#endif /* LUASCRIPT_H */
