{
    depfiles_gcc = "callbacks.o: src/callbacks.cpp include/glad/glad.h  include/KHR/khrplatform.h\
",
    values = {
        "/usr/bin/gcc",
        {
            "-m64",
            "-Iinclude",
            "-DROOT=\"/home/kommy/projects/dwarf_miner\""
        }
    },
    files = {
        "src/callbacks.cpp"
    }
}