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
    files = {
        "src/game.cpp"
    },
    depfiles_gcc = "game.o: src/game.cpp src/game.h\
"
}