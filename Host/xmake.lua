rule("csproj")
do
    set_extensions(".csproj")
	on_build_file(function (target, sourcefile)
		os.execv("dotnet", { "build", sourcefile, "-o", target:targetdir() })
        os.cp(path.join(target:targetdir(), "compiled_execute_filename"), target:targetfile())
	end)
end
rule_end()

target("Host")
do
    set_kind("phony")
    add_rules("csproj")

    add_files("WurstLink.Host.csproj")
end
target_end()

