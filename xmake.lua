add_rules("mode.debug")

function add_xf_log() 
    add_files("src/*.c")
    add_files("src/xf_printf/mpaland_printf/*.c")
    add_includedirs("src/")
end

target("xf_log")
    set_kind("binary")
    add_cflags("-Wall")
    add_cflags("-std=gnu99")
    add_files("examples/*.c")
    add_includedirs("examples/")
    add_xf_log()