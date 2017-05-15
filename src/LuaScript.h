/** 
 * @file LuaScript.h
 * @author Florian Bauer
 *
 */
#ifndef LUASCRIPT_H
#define LUASCRIPT_H

#include <lua.hpp>
#include <string>

class LuaScript
{
public:
    LuaScript(LuaScript& orig) = delete; // make non-copyable
    LuaScript& operator=(LuaScript& orig) = delete; // make non-copyable
    LuaScript(LuaScript&& orig) = default; // provide move constructor
    LuaScript& operator=(LuaScript&& orig) = default; // provide move assignment operator
    virtual ~LuaScript();
    static LuaScript import_script(const std::string& scriptFile);
    int executeScript() noexcept;
    void simpleLuaInterpreter() noexcept;
    void dumpStack() noexcept;
    int getGlobalInt(const std::string& varName) noexcept;

private:
    lua_State* m_L = nullptr;
    LuaScript() = default; // private default constructor (used only in `import_script()`)
    int loadScript(const std::string& fileName) noexcept;

};

#endif /* LUASCRIPT_H */
