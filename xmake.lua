set_project("WurstLink")
set_version("1.0.0")
add_rules("mode.debug", "mode.release")

includes("csharp", "common")

includes (
    "Host", "TestHost",
    "Client", "PrototypeClient"
)


target("WurstLink")
do
    set_kind("phony")
end
target_end()