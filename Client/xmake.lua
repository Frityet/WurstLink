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
local function concat(dst, src, ...)
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
local function merge_tostring(strs, sep)
    local s = ""
    for i = 1, #strs do
        s = s .. strs[i]
        if i ~= #strs then s = s .. sep end
    end
    return s
end

local CONFIG<const> = {
    compile = {
        sanitizers = { "address", "leak", "undefined" },
    },
    emrun = {
        port = 7777
    }
}

rule("embed")
do
    on_buildcmd_file(function (target, batch, source, option)
        import("utils.progress")
        import("core.project.depend")
        import("core.tool.compiler")
        import("lib.detect.find_tool")

        batch:mkdir(target:targetdir())

        ---@type string
        local objfpath = target:objectfile(source)
        local tmpc_path = objfpath .. ".c"
        
        table.insert(target:objectfiles(), objfpath)

        local srcf = io.open(source, "rb")
        ---@type string
        local src_contents = srcf:read("a")
        srcf:close()

        local tmpf = io.open(tmpc_path, "w")
        local dirs = path.directory(source):split("/")
        local symname = "$" .. dirs[#dirs] .. "_" .. path.basename(source)

        tmpf:write("#include <stdint.h>\n")
        tmpf:write("struct EmbeddedInfo { const uint64_t length; const uint8_t source[]; };")
        tmpf:write("const struct EmbeddedInfo " .. symname .. " = { .source = {")

        cprintf("\x1b[?25l\r")
        local srcc_len = #src_contents
        for i = 1, srcc_len do
            local c = src_contents:byte(i)
            tmpf:write(string.format("0x%02X", c))
            if i ~= srcc_len then tmpf:write(",") end

            cprintf("${bright green}[ %i%%]:${reset} ${color.build.object}embedding %s into binary ... written byte %i/%i\r",
                            math.floor(((i / srcc_len) * 100) + 0.5), source,                           i, srcc_len)
        end
        printf("\n\x1b[?25h\r")

        batch:show_progress(option.progress, "${color.build.object}embedded %s into binary", source)


        tmpf:write("}, ")
        tmpf:write(".length = ".. srcc_len)
        tmpf:write(" };")

        tmpf:close();

        batch:compile(tmpc_path, objfpath)

        batch:add_depfiles(source)
        batch:set_depcache(target:dependfile(objfpath))
    end)
end
rule_end()


target("client")
do
    set_kind("binary")
    set_plat("wasm")

    add_files("src/**.c")
    --add_files("src/**.cpp")

    add_headerfiles("src/**.h")
    --add_headerfiles("src/**.hpp")

    add_files("src/rendering/shaders/**.glsl", { rule = "embed" })

    add_includedirs("src/", "src/include/")
    add_includedirs("/usr/local/Cellar/emscripten/3.1.7/libexec/system/include/")

    add_cxflags("-Wall", "-Wextra", "-Werror", "-pthread")
    add_ldflags (
            "--use-preload-plugins", "--preload-file", "Client/assets/",
            "-s USE_GLFW=3",
            "-pthread",
            { force = true }
    )

    if is_mode("debug") then
        add_cxflags("-g", "-v")
        add_ldflags("--emrun", "-v")

        if CONFIG.compile.sanitizers ~= nil then
            local sanfl = merge_tostring(CONFIG.compile.sanitizers, ",")
            add_cxflags("-fsanitize=" .. sanfl, "-fno-omit-frame-pointer")
            add_ldflags("-fsanitize=" .. sanfl)
        end
        set_optimize("none")

    else
        set_optimize("aggressive")
    end

    --add_links("glfw3")

    on_run(function (target)
        import("lib.detect.find_program")
        import("lib.detect.find_tool")
        local emrun = find_tool("emrun", {
                pathes = { "/usr/bin", "/usr/local/bin" },
                check = function (program) os.runv(program, { "--browser_info" }) end
        })
        if emrun == nil then cprint("${bright red}error:${clear} \"emrun\" not found!") return end
        --local emrun = { program = "/usr/local/bin/emrun" }

        os.execv(emrun.program, {
                target:targetfile(),
                "--browser", "firefox",
                "--verbose",
                "--port", CONFIG.emrun.port
                --(function() if CONFIG.port == 0 then return "--port", CONFIG.port else return "" end end)()
        })
    end)
end
target_end()
