includes("Client")

target("WurstLink")
do
    set_kind("phony")
    add_deps("WurstLink-Client")
end
target_end()
