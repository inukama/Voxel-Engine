{
    values = {
        "/usr/bin/gcc",
        {
            "-m64",
            "-Iinclude",
            "-DROOT=\"/home/kommy/projects/dwarf_miner\"",
            "-DSPDLOG_SHARED_LIB",
            "-DSPDLOG_COMPILED_LIB",
            "-DSPDLOG_FMT_EXTERNAL"
        }
    },
    depfiles_gcc = "main.o: src/main.cpp include/glad/glad.h include/KHR/khrplatform.h  include/stb_image.h include/noise.h src/callbacks.h src/inputs.h  src/shader.h src/game.h\
",
    files = {
        "src/main.cpp"
    }
}