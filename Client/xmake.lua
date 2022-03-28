target("client")
do
    set_kind("binary")
    set_plat("wasm")

    add_files("src/**.c")
    add_files("src/**.cpp")

    add_headerfiles("src/**.h")
    add_headerfiles("src/**.hpp")

    --set_configvar("TEST_SHADER", "io.readfile(\"xxx.txt\")")
    add_configfiles("src/include/shader.h.in")

    add_includedirs("src/", "src/include/")
    add_includedirs("/usr/local/Cellar/emscripten/3.1.7/libexec/system/include/")

    add_cxflags("-Wall", "-Wextra", "-Werror")
    if is_mode("debug") then add_cxflags("-fsanitize=address", "-fno-omit-frame-pointer", "-g") end

    add_ldflags("--use-preload-plugins", "--preload-file", "Client/assets/",
                "-s USE_GLFW=3", { force = true })
    add_links("GLESv2", "glfw3", "glfw")

    
end
target_end()
