{
    depfiles_gcc = "main.o: src/main.c\
",
    files = {
        "src/main.c"
    },
    values = {
        "/usr/bin/gcc",
        {
            "-m64",
            "-Iinclude"
        }
    }
}