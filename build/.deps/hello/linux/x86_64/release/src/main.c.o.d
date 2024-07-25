{
    files = {
        "src/main.c"
    },
    depfiles_gcc = "main.o: src/main.c\
",
    values = {
        "/usr/bin/gcc",
        {
            "-m64"
        }
    }
}