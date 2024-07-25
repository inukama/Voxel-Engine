{
    files = {
        "src/inputs.cpp"
    },
    depfiles_gcc = "inputs.o: src/inputs.cpp include/glad/glad.h include/KHR/khrplatform.h\
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