target("client")
do
    set_kind("binary")
    set_plat("wasm")

    add_files("src/**.c")
    add_files("src/**.cpp")

    add_headerfiles("src/**.h")
    add_headerfiles("src/**.hpp")

    add_includedirs("src/", "src/include/")

    add_cxflags("-Wall", "-Wextra", "-Werror")
    if is_mode("debug") then add_cxflags("-fsanitize=address", "-fno-omit-frame-pointer", "-g") end
    add_ldflags("--use-preload-plugins", "--preload-file", "Client/assets/", { force = true })
end
target_end()
