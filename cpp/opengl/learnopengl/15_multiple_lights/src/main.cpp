#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "utils.h"
#include "shader.hpp"
#include "texture.h"
#include "camera.hpp"
#include "callbacks.h"
#include "constants.h"

#include <array>

// window
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

// frame rate
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// mouse
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// lighting
glm::vec3 lightDir(-0.2f, -1.0f, -0.3f);

glm::vec3 pointLightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};

bool flashlightOn = true;
bool fPressedLastFrame = false;

// arguments
const char *args[] = {
    "ch1",
    "ex1",
    "ex2",
    "ex3",
    "ex4",
};

size_t args_size = sizeof(args) / sizeof(args[0]);

int help() {
    std::cout << R"(Demo of some basic multiple lights examples with OpenGL.

The available commands are listed below:
- ch1       Scene with multiple light sources.
- ex1       Desert.
- ex2       Factory.
- ex3       Horror.
- ex4       Biochemical lab.

For example: ./build/main ch1)" << std::endl;

    return 0;
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);

    int fState = glfwGetKey(window, GLFW_KEY_F);
    if (fState == GLFW_PRESS && !fPressedLastFrame)
        flashlightOn = !flashlightOn;

    fPressedLastFrame = (fState == GLFW_PRESS);
}

GLFWwindow *initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL - Multiple Lights", NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return NULL;
    }

    glEnable(GL_DEPTH_TEST);

    return window;
}

std::string fmtPoint(int i, const std::string& prop) {
    return "pointLights[" + std::to_string(i) + "]." + prop;
};

std::array<glm::vec3, 4> getColors() {
    if (is_arg("ex1"))
        return {
            glm::vec3(1.0f, 0.6f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0, 0.0),
            glm::vec3(0.2f, 0.2f, 1.0f)
        };

    if (is_arg("ex2"))
        return {
            glm::vec3(0.2f, 0.2f, 0.6f),
            glm::vec3(0.3f, 0.3f, 0.7f),
            glm::vec3(0.0f, 0.0f, 0.3f),
            glm::vec3(0.4f, 0.4f, 0.4f)
        };

    if (is_arg("ex3"))
        return {
            glm::vec3(0.1f, 0.1f, 0.1f),
            glm::vec3(0.1f, 0.1f, 0.1f),
            glm::vec3(0.1f, 0.1f, 0.1f),
            glm::vec3(0.3f, 0.1f, 0.1f)
        };

    if (is_arg("ex4"))
        return {
            glm::vec3(0.4f, 0.7f, 0.1f),
            glm::vec3(0.4f, 0.7f, 0.1f),
            glm::vec3(0.4f, 0.7f, 0.1f),
            glm::vec3(0.4f, 0.7f, 0.1f)
        };

    return {
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(1.0f, 1.0f, 1.0f)
    };
}

void scene(Shader cubeShader) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    cubeShader.setVec3("dirLight.direction", lightDir);
    cubeShader.setVec3("dirLight.ambient",   0.05f, 0.05f, 0.05f);
    cubeShader.setVec3("dirLight.diffuse",   0.4f, 0.4f, 0.4f);
    cubeShader.setVec3("dirLight.specular",  0.5f, 0.5f, 0.5f);

    for (unsigned int i = 0; i < 4; i++) {
        cubeShader.setVec3(fmtPoint(i, "position"), pointLightPositions[i]);

        cubeShader.setVec3(fmtPoint(i, "specular"), 1.0f, 1.0f, 1.0f);
        cubeShader.setVec3(fmtPoint(i, "ambient"),  0.2f, 0.2f, 0.2f);
        cubeShader.setVec3(fmtPoint(i, "diffuse"),  0.5f, 0.5f, 0.5f);

        cubeShader.setFloat(fmtPoint(i, "constant"),  1.0f);
        cubeShader.setFloat(fmtPoint(i, "linear"),    0.09f);
        cubeShader.setFloat(fmtPoint(i, "quadratic"), 0.032f);
    }

    cubeShader.setVec3("spotLight.diffuse",      1.0f, 1.0f, 1.0f);
    cubeShader.setVec3("spotLight.specular",     1.0f, 1.0f, 1.0f);
    cubeShader.setFloat("spotLight.linear",      0.09f);
    cubeShader.setFloat("spotLight.quadratic",   0.032f);
    cubeShader.setFloat("spotLight.cutOff",      glm::cos(glm::radians(12.5f)));
    cubeShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));   
}

void desert(Shader cubeShader) {
    glClearColor(0.75f, 0.52f, 0.3f, 1.0f);

    auto lightColors = getColors();

    cubeShader.setVec3("dirLight.direction", lightDir);
    cubeShader.setVec3("dirLight.ambient",   0.3f, 0.24f, 0.14f);
    cubeShader.setVec3("dirLight.diffuse",   0.7f, 0.42f, 0.26f);
    cubeShader.setVec3("dirLight.specular",  0.5f, 0.5f, 0.5f);

    for (unsigned int i = 0; i < 4; i++) {
        cubeShader.setVec3(fmtPoint(i, "position"), pointLightPositions[i]);

        cubeShader.setVec3(fmtPoint(i, "ambient"),  lightColors[i] * 0.1f);
        cubeShader.setVec3(fmtPoint(i, "diffuse"),  lightColors[i]);
        cubeShader.setVec3(fmtPoint(i, "specular"), lightColors[i]);

        cubeShader.setFloat(fmtPoint(i, "constant"),  1.0f);
        cubeShader.setFloat(fmtPoint(i, "linear"),    0.09);
        cubeShader.setFloat(fmtPoint(i, "quadratic"), 0.032);
    }

    cubeShader.setVec3("spotLight.diffuse",      0.8f, 0.8f, 0.0f);
    cubeShader.setVec3("spotLight.specular",     0.8f, 0.8f, 0.0f);
    cubeShader.setFloat("spotLight.linear",      0.09);
    cubeShader.setFloat("spotLight.quadratic",   0.032);
    cubeShader.setFloat("spotLight.cutOff",      glm::cos(glm::radians(12.5f)));
    cubeShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(13.0f)));
}

void factory(Shader cubeShader) {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    auto lightColors = getColors();

    cubeShader.setVec3("dirLight.direction", lightDir);
    cubeShader.setVec3("dirLight.ambient",   0.05f, 0.05f, 0.1f);
    cubeShader.setVec3("dirLight.diffuse",   0.2f, 0.2f, 0.7);
    cubeShader.setVec3("dirLight.specular",  0.7f, 0.7f, 0.7f);

    for (unsigned int i = 0; i < 4; i++) {
        cubeShader.setVec3(fmtPoint(i, "position"), pointLightPositions[i]);

        cubeShader.setVec3(fmtPoint(i, "ambient"),  lightColors[i] * 0.1f);
        cubeShader.setVec3(fmtPoint(i, "diffuse"),  lightColors[i]);
        cubeShader.setVec3(fmtPoint(i, "specular"), lightColors[i]);

        cubeShader.setFloat(fmtPoint(i, "constant"),  1.0f);
        cubeShader.setFloat(fmtPoint(i, "linear"),    0.09);
        cubeShader.setFloat(fmtPoint(i, "quadratic"), 0.032);
    }

    cubeShader.setVec3("spotLight.diffuse",      1.0f, 1.0f, 1.0f);
    cubeShader.setVec3("spotLight.specular",     1.0f, 1.0f, 1.0f);
    cubeShader.setFloat("spotLight.linear",      0.009);
    cubeShader.setFloat("spotLight.quadratic",   0.0032);
    cubeShader.setFloat("spotLight.cutOff",      glm::cos(glm::radians(10.0f)));
    cubeShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(12.5f)));
}

void horror(Shader cubeShader) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    auto lightColors = getColors();

    cubeShader.setVec3("dirLight.direction", lightDir);
    cubeShader.setVec3("dirLight.ambient", 0.0f, 0.0f, 0.0f);
    cubeShader.setVec3("dirLight.diffuse", 0.05f, 0.05f, 0.05);
    cubeShader.setVec3("dirLight.specular", 0.2f, 0.2f, 0.2f);

    for (unsigned int i = 0; i < 4; i++) {
        cubeShader.setVec3(fmtPoint(i, "position"), pointLightPositions[i]);

        cubeShader.setVec3(fmtPoint(i, "ambient"), lightColors[i] * 0.1f);
        cubeShader.setVec3(fmtPoint(i, "diffuse"), lightColors[i]);
        cubeShader.setVec3(fmtPoint(i, "specular"), lightColors[i]);

        cubeShader.setFloat(fmtPoint(i, "constant"), 1.0f);
        cubeShader.setFloat(fmtPoint(i, "linear"),    (i == 2) ? 0.22 : 0.14);
        cubeShader.setFloat(fmtPoint(i, "quadratic"), (i == 2) ? 0.20 : 0.07);
    }

    cubeShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
    cubeShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    cubeShader.setFloat("spotLight.linear", 0.09);
    cubeShader.setFloat("spotLight.quadratic", 0.032);
    cubeShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(10.0f)));
    cubeShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
}

void biochemical_lab(Shader cubeShader) {
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);

    auto lightColors = getColors();

    cubeShader.setVec3("dirLight.direction", lightDir);
    cubeShader.setVec3("dirLight.ambient",   0.5f, 0.5f, 0.5f);
    cubeShader.setVec3("dirLight.diffuse",   1.0f, 1.0f, 1.0f);
    cubeShader.setVec3("dirLight.specular",  1.0f, 1.0f, 1.0f);

    for (unsigned int i = 0; i < 4; i++) {
        cubeShader.setVec3(fmtPoint(i, "position"), pointLightPositions[i]);

        cubeShader.setVec3(fmtPoint(i, "ambient"),  lightColors[i] * 0.1f);
        cubeShader.setVec3(fmtPoint(i, "diffuse"),  lightColors[i]);
        cubeShader.setVec3(fmtPoint(i, "specular"), lightColors[i]);

        cubeShader.setFloat(fmtPoint(i, "constant"),  1.0f);
        cubeShader.setFloat(fmtPoint(i, "linear"),    0.07);
        cubeShader.setFloat(fmtPoint(i, "quadratic"), 0.017);
    }

    cubeShader.setVec3("spotLight.diffuse",      0.0f, 1.0f, 0.0f);
    cubeShader.setVec3("spotLight.specular",     0.0f, 1.0f, 0.0f);
    cubeShader.setFloat("spotLight.linear",      0.07);
    cubeShader.setFloat("spotLight.quadratic",   0.017);
    cubeShader.setFloat("spotLight.cutOff",      glm::cos(glm::radians(7.0f)));
    cubeShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(10.0f)));

}

int chapter(GLFWwindow *window) {
    Shader cubeShader = Shader::createShader("cube");
    if (!cubeShader.id) {
        std::cout << "Failed to create the cube shader program" << std::endl;
        return 1;
    }

    Shader lightShader = Shader::createShader("light");
    if (!lightShader.id) {
        std::cout << "Failed to create the light shader program" << std::endl;
        return 1;
    }

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    // cube's VBO and VAO
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // light's VAO
    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int diffuseMap = loadTexture(CONTAINER2_TEX);
    unsigned int specularMap = loadTexture(CONTAINER2_SPEC);

    cubeShader.use();
    cubeShader.setInt("material.diffuse", 0);
    cubeShader.setInt("material.specular", 1);
    cubeShader.setFloat("material.shininess", 32.0f);

    auto lightColor = getColors();

    // render loop
    while (!glfwWindowShouldClose(window)) {
        // per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        // render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        cubeShader.use();
        cubeShader.setVec3("viewPos", camera.position);

        // setup lighting
        if      (is_arg("ch1")) scene(cubeShader);
        else if (is_arg("ex1")) desert(cubeShader);
        else if (is_arg("ex2")) factory(cubeShader);
        else if (is_arg("ex3")) horror(cubeShader);
        else if (is_arg("ex4")) biochemical_lab(cubeShader);

        cubeShader.setVec3("spotLight.position",  camera.position);
        cubeShader.setVec3("spotLight.direction", camera.front);
        cubeShader.setVec3("spotLight.ambient",   0.0f, 0.0f, 0.0f);
        cubeShader.setFloat("spotLight.constant", 1.0f);

        if (!flashlightOn) {
            cubeShader.setVec3("spotLight.diffuse",      glm::vec3(0.0f));
            cubeShader.setVec3("spotLight.specular",     glm::vec3(0.0f));
            cubeShader.setFloat("spotLight.linear",      0.0f);
            cubeShader.setFloat("spotLight.quadratic",   0.0f);
            cubeShader.setFloat("spotLight.cutOff",      0.0f);
            cubeShader.setFloat("spotLight.outerCutOff", 0.0f);
        }

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        cubeShader.setMat4("projection", projection);
        cubeShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        cubeShader.setMat4("model", model);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        glBindVertexArray(VAO);

        for (unsigned int i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);

            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            cubeShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // also draw the lamp object
        lightShader.use();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);

        glBindVertexArray(lightVAO);

        for (unsigned int i = 0; i < 4; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube

            lightShader.setMat4("model", model);
            lightShader.setVec3("lightColor", lightColor[i]);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    glDeleteProgram(cubeShader.id);
    glDeleteProgram(lightShader.id);

    return 0;
}

int main(int argc, char const *argv[]) {
    switch (process_args(argc, argv, args, args_size)) {
        case -1:
            help();
            return 0;
        case 1:
            return 1;
    }

    GLFWwindow *window = initWindow();

    if (window == NULL) {
        std::cout << "Can't initiate window" << std::endl;
        return 1;
    };

    int exit_code = chapter(window);

    glfwTerminate();

    return exit_code;
}
