{
    files = {
        "src/main.cpp"
    },
    depfiles_gcc = "main.o: src/main.cpp include/glad/glad.h include/KHR/khrplatform.h  include/stb_image.h src/callbacks.h src/inputs.h src/shader.h\
",
    values = {
        "/usr/bin/gcc",
        {
            "-m64",
            "-Iinclude",
            "-DROOT=\"/home/kommy/projects/dwarf_miner\""
        }
    }
}