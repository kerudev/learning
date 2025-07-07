#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"

int help() {
    std::cout << "Demo of some basic transformation examples with OpenGL.\n" << std::endl;

    std::cout << "The available commands are listed below:" << std::endl;
    std::cout << "- tra1      Example using rotation and translate transformations." << std::endl;
    std::cout << "- tra2      Example using a texture and coloring over it." << std::endl;
    std::cout << "- ex1       Exercise 1: swap the order of rotate and translate." << std::endl;
    std::cout << "- ex2       Exercise 2: move container using transforms and scale with the sin function." << std::endl;

    std::cout << "\nFor example: ./build/main tra1" << std::endl;

    return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
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

    GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL - Transformations", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }

    return window;
}

int transformation(GLFWwindow *window, Shader shader, int num) {
    float vertices[] = {
        // positions            // texture coords
         0.5f,  0.5f, 0.0f,     1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,     1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,     0.0f, 1.0f    // top left 
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texture 1
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    unsigned char *data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0); 

    if (!data) {
        std::cout << "Failed to load texture1" << std::endl;
        stbi_image_free(data);
        return 1;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    
    // texture 2
    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // load image, create texture and generate mipmaps
    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0); 

    if (!data) {
        std::cout << "Failed to load texture2" << std::endl;
        stbi_image_free(data);
        return 1;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    
    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // create transformations
        glm::mat4 transform = glm::mat4(1.0f);

        if (num == 1) {
            transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
            transform = glm::scale(transform, glm::vec3(0.5, 0.5, 0.5));
        }
        else if (num == 3) {
            transform = glm::rotate(transform, (float) glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
            transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
        }
        else {  // 2, 4
            transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.0f));
            transform = glm::rotate(transform, (float) glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
        }

        shader.use();
        unsigned int transformLoc = glGetUniformLocation(shader.id, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // render container
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        if (num == 4) {
            transform = glm::mat4(1.0f);
            transform = glm::translate(transform, glm::vec3(-0.5f, 0.5f, 0.0f)); 

            float scaleRatio = (float) sin(glfwGetTime());
            transform = glm::scale(transform, glm::vec3(scaleRatio, scaleRatio, scaleRatio));

            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]);

            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

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

    GLFWwindow *window = initWindow();

    if (window == NULL) {
        std::cout << "Terminating example" << std::endl;
        return 1;
    };

    Shader shader = Shader::createShader("transform");

    if (!shader.id) {
        std::cout << "Failed to create the shader program" << std::endl;
        return 1;
    }

    int exit_code = 0;

    if (strcmp(command, "tra1") == 0) {
        exit_code = transformation(window, shader, 1);
    }
    else if (strcmp(command, "tra2") == 0) {
        exit_code = transformation(window, shader, 2);
    }
    else if (strcmp(command, "ex1") == 0) {
        exit_code = transformation(window, shader, 3);
    }
    else if (strcmp(command, "ex2") == 0) {
        exit_code = transformation(window, shader, 4);
    }

    glDeleteProgram(shader.id);
    glfwTerminate();

    return exit_code;
}
