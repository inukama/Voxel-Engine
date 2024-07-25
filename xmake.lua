add_requires("glfw3", "glm", "spdlog")

target("dwarf_miner")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("glfw3", "glm", "spdlog")
    add_includedirs("include")
    add_defines("ROOT=\"$(projectdir)\"")

