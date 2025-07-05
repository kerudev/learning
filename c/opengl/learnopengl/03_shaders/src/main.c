#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shader.h"

const char *TEX_WALL = "textures/wall.jpg";
const char *TEX_WOOD = "textures/container.jpg";
const char *TEX_FACE = "textures/awesomeface.png";

int wall = 0;
float alpha = 0.0f;

int help() {
    printf("Demo of some basic texture examples with OpenGL.\n");
    printf("Use the --wall flag to change the texture.\n\n");

    printf("The available commands are listed below:\n");
    printf("- tex1      Example using a texture.\n");
    printf("- tex2      Example using a texture and coloring over it.\n");
    printf("- tex3      Example using uniform variables.\n");
    printf("- ex1       Exercise 1: Awesome face looks the other way.\n");
    printf("- ex2       Exercise 2: Display 4 smiley faces on a single container image clamped at its edge.\n");
    printf("- ex3       Exercise 3: Display only the center pixels of the textures (GL_NEAREST).\n");
    printf("- ex4       Exercise 4: Change alpha value using up and down arrow keys.\n\n");
    
    printf("For example: ./build/main tex1\n");
    printf("For example: ./build/main tex2 --wall\n");

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);

    if (alpha < 1 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        alpha += 0.02;

    if (alpha > -1 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        alpha -= 0.02;
}

void genExampleVertices(int num, float* vertices) {
    switch (num) {
        case 2: {
            float tmp[] = {
                // positions          // colors           // texture coords
                 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   2.0f, 2.0f,   // top right
                 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   2.0f, 0.0f,   // bottom right
                -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
                -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 2.0f    // top left 
            };
            memcpy(vertices, tmp, sizeof(tmp));
            break;
        }

        case 3: {
            float tmp[] = {
                // positions          // colors           // texture coords
                 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.55f, 0.55f, // top right
                 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.55f, 0.45f, // bottom right
                -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.45f, 0.45f, // bottom left
                -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.45f, 0.55f  // top left 
            };

            memcpy(vertices, tmp, sizeof(tmp));
            break;
        }

        default: {
            float tmp[] = {
                // positions          // colors           // texture coords
                 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
                 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
                -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
                -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
            };

            memcpy(vertices, tmp, sizeof(tmp));
            break;
        }
    }
}

GLFWwindow *initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL - Shaders", NULL, NULL);

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

int texture(GLFWwindow* window, const Shader shader, int num) {
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    
    unsigned int indices[] = {
        0, 1, 3,    // first triangle
        1, 2, 3,    // second triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // create texture
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    const char *texturePath = (wall) ? "textures/wall.jpg" : TEX_WOOD;
    int width, height, nrChannels;
    unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0); 

    if (!data) {
        printf("Failed to load texture '%s'\n", texturePath);
        stbi_image_free(data);
        return 1;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    if (num == 3) {
        glGenTextures(1, &texture2);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // load image, create texture and generate mipmaps
        const char *texturePath = TEX_FACE;
        
        stbi_set_flip_vertically_on_load(true);
        unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0); 

        if (!data) {
            printf("Failed to load texture '%s'\n", texturePath);
            stbi_image_free(data);
            return 1;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);
        
        useShader(&shader);
        setShaderInt(&shader, "texture1", 0);
        setShaderInt(&shader, "texture2", 1);
    }

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // input
        processInput(window);
        
        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        if (num == 3) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2);
        }

        // render
        useShader(&shader);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        // swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    return 0;
}

int exercise(GLFWwindow* window, const Shader shader, int num) {
    float vertices[32] = {};
    genExampleVertices(num, vertices);

    unsigned int indices[] = {
        0, 1, 3,    // first triangle
        1, 2, 3,    // second triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // create texture
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // set the texture wrapping parameters
    switch (num) {
    case 2:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        break;

    default:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        break;
    }

    // set texture filtering parameters
    switch (num) {
    case 3:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        break;
    
    default:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;
    }

    // load image, create texture and generate mipmaps
    const char *texturePath = (wall) ? TEX_WALL : TEX_WOOD;
    int width, height, nrChannels;
    unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0); 

    if (!data) {
        printf("Failed to load texture '%s'\n", texturePath);
        stbi_image_free(data);
        return 1;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // set texture filtering parameters
    switch (num) {
    case 3:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        break;
    
    default:
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        break;
    }

    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true);

    texturePath = TEX_FACE;
    data = stbi_load(texturePath, &width, &height, &nrChannels, 0); 

    if (!data) {
        printf("Failed to load texture '%s'\n", texturePath);
        stbi_image_free(data);
        return 1;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    useShader(&shader);
    setShaderInt(&shader, "texture1", 0);
    setShaderInt(&shader, "texture2", 1);

    while (!glfwWindowShouldClose(window)) {
        // input
        processInput(window);
        
        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // render
        if (num == 4) setShaderFloat(&shader, "alpha", alpha);

        useShader(&shader);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    return 0;
}

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        help();
        return 1;
    }

    const char *command = argv[1];

    if (strcmp(command, "-h") == 0 || strcmp(command, "--help") == 0) {
        help();
        return 0;
    }

    if (argc > 2 && strcmp(argv[2], "--wall") == 0) wall = 1;

    GLFWwindow * window = initWindow();

    if (window == NULL) {
        printf("Terminating example\n");
        return 1;
    };

    Shader shader = createShader(command);

    if (!shader.id) {
        printf("Failed to create the shader program for '%s'\n", command);
        return 1;
    }

    int exit_code = 0;

    if (strcmp(command, "tex1") == 0) {
        exit_code = texture(window, shader, 1);
    }
    else if (strcmp(command, "tex2") == 0) {
        exit_code = texture(window, shader, 2);
    }
    else if (strcmp(command, "tex3") == 0) {
        exit_code = texture(window, shader, 3);
    }
    else if (strcmp(command, "ex1") == 0) {
        // 1. Make sure only the happy face looks in the other/reverse
        // direction by changing the fragment shader.
        exit_code = exercise(window, shader, 1);
    }
    else if (strcmp(command, "ex2") == 0) {
        // 2. Experiment with the different texture wrapping methods by
        // specifying texture coordinates in the range 0.0f to 2.0f instead of
        // 0.0f to 1.0f. See if you can display 4 smiley faces on a single
        // container image clamped at its edge.
        // See if you can experiment with other wrapping methods as well.
        exit_code = exercise(window, shader, 2);
    }
    else if (strcmp(command, "ex3") == 0) {
        // 3. Try to display only the center pixels of the texture image on the
        // rectangle in such a way that the individual pixels are getting
        // visible by changing the texture coordinates. Try to set the texture
        // filtering method to GL_NEAREST to see the pixels more clearly.
        exit_code = exercise(window, shader, 3);
    }
    else if (strcmp(command, "ex4") == 0) {
        // 4. Use a uniform variable as the mix function's third parameter to
        // vary the amount the two textures are visible. Use the up and down
        // arrow keys to change how much the container or the smiley face is visible.
        exit_code = exercise(window, shader, 4);
    }

    glDeleteProgram(shader.id);
    glfwTerminate();

    return exit_code;
}
