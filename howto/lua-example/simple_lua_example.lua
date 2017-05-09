-- comment
--[[ block comment until the next ]]


--[[
--defines a factorial function
function fact(n)
    if n == 0 then
        return 1
    else
        return n * fact(n-1)
    end
end

print("enter a number:")
a = io.read("*number")
print(fact(a))
]]


-- print different types
function print_types(a, b, c, d, e, f, g)
    print(a, "has type:", type(a))
    print(b, "has type:", type(b))
    print(c, "has type:", type(c))
    print(d, "has type:", type(d))
    print(e, "has type:", type(e))
    print(f, "has type:", type(f))
    print(g, "has type:", type(g))
end

print_types(4, 3.3e2, "hello", nil, true, print, uninitialized_variable)



-- tables
a = {}                      -- new table, empty
a["k"] = 42                 -- new entry key="k" value=42
print(a["k"])
a["my_print"] = print       -- new entry key="my_print" value=print-function
a.my_print("that's cool")

a = {}                      -- empty table
for i=1,100 do a[i] = 2^i end
for i=1,5 do
    print(a[i])
end

a.x = 2                     -- a.x is equivalent to a["x"] but not to a[x]
if a.x == a["x"] and a.x == 2  then
    print("it's the same")
end
if not (a.x == a[x]) then
    print("not the same")
end

print("the typing in lua is a bit shady :(", a.x, not a.x)
