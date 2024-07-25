{
    depfiles_gcc = "shader.o: src/shader.cpp include/glad/glad.h include/KHR/khrplatform.h  src/shader.h\
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
        "src/shader.cpp"
    }
}