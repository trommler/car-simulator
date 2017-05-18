-- testscript01.lua

theAnswer = 42 -- a global variable

bashCommand = "sudo rm -rf /"

window = {
    title = "bla",
    width = 400,
    height = 500
}

local misfortuneNumber = 13 -- a local variable

table = {
    RequestId = 0x7e0,
    ResponseId = 0x7e8,

    ReadDataByIdentifier = {
        ["f190"] = 0911,
        ["f124"] = "HPLA-12345-AB",
        ["fabc"] = nil,
        ["1e23"] = "231132"
    },

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

