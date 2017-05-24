-- testscript01.lua

theAnswer = 42 -- a global variable

bashCommand = "sudo rm -rf /"

local misfortuneNumber = 13 -- a local variable


window = {
    title = "bla",
    width = 400,
    height = 500,
    hexVal = 0x10,
}

table = {
    RequestId = 0x7e0,
    ResponseId = 0x7e8,
    a = 0x61,
    b = 0x62,
    c = 0x63
}


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

