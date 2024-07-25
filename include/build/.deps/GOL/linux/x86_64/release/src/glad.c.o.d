{
    depfiles_gcc = "glad.o: src/glad.c include/glad/glad.h include/KHR/khrplatform.h\
",
    files = {
        "src/glad.c"
    },
    values = {
        "/usr/bin/gcc",
        {
            "-m64",
            "-Iinclude"
        }
    }
}