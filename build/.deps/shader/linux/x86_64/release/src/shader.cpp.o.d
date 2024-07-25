{
    files = {
        "src/shader.cpp"
    },
    values = {
        "/usr/bin/gcc",
        {
            "-m64",
            "-Iinclude"
        }
    },
    depfiles_gcc = "shader.o: src/shader.cpp include/glad/glad.h include/KHR/khrplatform.h\
"
}