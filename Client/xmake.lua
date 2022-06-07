includes("../packages.lua")

--Config: 
local packages = {
	"lua",

	"libwebsockets",

	-- "libsdl",
	-- "opengl", 
	"glew",

	"managedc"
}

local sanitizers = { "address", "undefined" }

local cflags = {
	release = {},
	debug = {
		"-Wno-unused-function", "-Wno-unused-parameter", "-Wno-unused-variable",
	},
	regular = {
		"-Wall", "-Wextra", "-Werror",
		"-fms-extensions", "-Wno-microsoft"
	}
}

local ldflags = {
	release = {},
	debug = {
		"--emrun"
	},
	regular = {
		"-pthreads"
	}
}

add_rules("mode.debug", "mode.release")

set_plat("wasm")
set_arch("wasm32")

add_requires(packages)

target("WurstLink-Client")
do
	set_kind("binary")
	add_packages(packages)

	add_files("src/**.c")
	add_headerfiles("src/**.h")

	add_includedirs("src/", "src/include")
	add_includedirs("/usr/local/Cellar/emscripten/3.1.12/libexec/system/include/")

	add_cflags(cflags.regular)
	add_ldflags(ldflags.regular)

	if is_mode("debug") then
		add_cflags(cflags.debug)
		add_ldflags(ldflags.debug, { force = true })

		for _, v in ipairs(sanitizers) do
			add_cxflags("-fsanitize=" .. v)
			add_ldflags("-fsanitize=" .. v)
		end

		add_defines("PROJECT_DEBUG")
	elseif is_mode("release") then
		add_cflags(cflags.release)
		add_ldflags(ldflags.release)
	end

	on_run(function (target) 
		-- import("lib.detect.find_tool")
		-- local emrun = find_tool("emrun")
		local emrun = { program = "/usr/local/bin/emrun" }
		os.vrunv(emrun.program, {
			"--browser", "firefox",
			"--verbose",
			"--serve_after_exit",
			target:targetfile()
		})
	end)
end
target_end()
