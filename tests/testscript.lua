-- testscript.lua
print("-- start lua script!")

theAnswer = 42 -- a global variable

local misfortuneNumber = 13 -- a local variable

-- 
function printHelloWorld ()
    print("Hello World from Lua!")
end

-- defines a factorial function
function fact (n)
    if n == 0 then
        return 1
    else
        return n * fact(n-1)
    end
end

-- dubles the input
function double (x)
    return 2 * x
end

print("-- end lua script!")
