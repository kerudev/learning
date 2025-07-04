#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "shader.h"

int help() {
    printf("Demo of some basic shader examples with OpenGL.\n\n");

    printf("The available commands are listed below:\n");
    printf("- uni       Example using uniform variables.\n");
    printf("- attr      Example using vertex and color attributes.\n");
    printf("- ex1       Exercise 1: Upside down triangle.\n");
    printf("- ex2       Exercise 2: Triangle moved with X offset.\n");
    printf("- ex3       Exercise 3: Output the vertex position to the fragment shader.\n");

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}

GLFWwindow *initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);

    if (window == NULL) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return NULL;
    }

    return window;
}

unsigned int buildShaderProgram(const char *shaderName) {
    char vertexPath[64];
    char fragmentPath[64];

    sprintf(vertexPath, "shaders/vertex/%s.vert", shaderName);
    sprintf(fragmentPath, "shaders/fragment/%s.frag", shaderName);

    Shader shader = createShader(vertexPath, fragmentPath);
    
    return shader.id;
}

int uniform(GLFWwindow *window, unsigned int shaderProgram) {
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // left
         0.5f, -0.5f, 0.0f, // right
         0.0f,  0.5f, 0.0f  // top
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    return 0;
}

int attributes(GLFWwindow *window, unsigned int shaderProgram) {
    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    return 0;
}

int exercise(GLFWwindow *window, unsigned int shaderProgram, int num) {
    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        if (num == 2) {
            int offset = glGetUniformLocation(shaderProgram, "offset");
            glUniform1f(offset, 1.0f);
        }

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    return 0;
}

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        help();
        return 1;
    }

    char *command = argv[1];

    if (strcmp(command, "-h") == 0 || strcmp(command, "--help") == 0) {
        help();
        return 0;
    }

    GLFWwindow * window = initWindow();

    if (window == NULL) {
        printf("Terminating example\n");
        return 1;
    };

    unsigned int shaderProgram = buildShaderProgram(command);

    if (!shaderProgram) {
        printf("Failed to create the shader program for '%s'\n", command);
        return 1;
    }

    int exit_code = 0;

    if (strcmp(command, "uni") == 0) {
        exit_code = uniform(window, shaderProgram);
    }
    else if (strcmp(command, "attr") == 0) {
        exit_code = attributes(window, shaderProgram);
    }
    else if (strcmp(command, "ex1") == 0) {
        // 1. Adjust the vertex shader so that the triangle is upside down
        exit_code = exercise(window, shaderProgram, 1);
    }
    else if (strcmp(command, "ex2") == 0) {
        // 2. Specify a horizontal offset via a uniform and move the triangle 
        // to the right side of the screen in the vertex shader using this
        // offset value
        exit_code = exercise(window, shaderProgram, 2);
    }
    else if (strcmp(command, "ex3") == 0) {
        // 3. Output the vertex position to the fragment shader using the out
        // keyword and set the fragment's color equal to this vertex position
        // (see how even the vertex position values are interpolated across the
        // triangle). Once you managed to do this; try to answer the following
        // question: why is the bottom-left side of our triangle black?
        exit_code = exercise(window, shaderProgram, 3);
    }

    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return exit_code;
}
