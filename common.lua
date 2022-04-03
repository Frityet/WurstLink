function string.split(s, sep)
    if sep == nil then sep = "\n" end

    local t = {}
    for str in string.gmatch(s, "([^" .. sep .. "]+)") do table.insert(t, str) end
    return t
end

---@param dst   string
---@param src   string
---@param ...   string[]
---@return      string
function concat(dst, src, ...)
    local args = {...}
    if #args > 0 then for i, v in ipairs(args) do src = src .. v end end
    dst = dst .. src
    return dst
end

---@param dst   string
---@param src   string
---@param ...   string[]
---@return      string
function string.concat(dst, src, ...) return concat(dst, src, ...) end


---@param strs  string[]
---@param sep   string
function table.merge(strs, sep)
    local s = ""
    for i = 1, #strs do
        s = s .. strs[i]
        if i ~= #strs then s = s .. sep end
    end
    return s
end
