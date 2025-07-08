#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <stdbool.h>

typedef struct {
    unsigned int id;
} Shader;

Shader createShader(const char* name);
Shader createShaderFromPaths(const char* vertexPath, const char* fragmentPath);

char *readShaderFile(const char *path);

void useShader(const Shader* shader);

void setShaderBool(const Shader* shader, const char* name, bool value);
void setShaderInt(const Shader* shader, const char* name, int value);
void setShaderFloat(const Shader* shader, const char* name, float value);

#endif
