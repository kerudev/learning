#include <glad/glad.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "shader.h"

char *readLines(const char *path) {
    FILE *f = fopen(path, "r");

    if (!f) {
        printf("Error reading %s\n", path);
        return NULL;
    }

    char *content = NULL;
    size_t size = 0;
    char line[128];
    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);

        char* temp = realloc(content, size + len + 1);

        if (!temp) {
            perror("Error on realloc");
            free(content);
            fclose(f);
            return NULL;
        }

        content = temp;

        memcpy(content + size, line, len);
        size += len;
    }

    if (content) {
        content[size] = '\0';
    }

    fclose(f);

    return content;
}

Shader createShader(const char* vertexPath, const char* fragmentPath) {
    // read vertex shader
    const char *vertexSource = readLines(vertexPath);
    if (!vertexSource) {
        return (Shader){ .id = 0 };
    }
    
    // read fragment shader
    const char *fragmentSource = readLines(fragmentPath);
    if (!fragmentSource) {
        return (Shader){ .id = 0 };
    }

    // compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // compile vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, NULL);
    glCompileShader(vertex);

    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    };

    // compile fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, NULL);
    glCompileShader(fragment);

    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    };

    // shader Program
    unsigned int id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);

    glGetProgramiv(id, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(id, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n%s\n", infoLog);
    }

    // delete shaders
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return (Shader){ .id = id };
}

void useShader(const Shader* shader) {
    glUseProgram(shader->id);
}

void setShaderBool(const Shader* shader, const char* name, bool value) {
    glUniform1i(glGetUniformLocation(shader->id, name), (int)value);
}

void setShaderInt(const Shader* shader, const char* name, int value) {
    glUniform1i(glGetUniformLocation(shader->id, name), value);
}

void setShaderFloat(const Shader* shader, const char* name, float value) {
    glUniform1f(glGetUniformLocation(shader->id, name), value);
}
