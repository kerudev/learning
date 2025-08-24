#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;
    vec3 direction;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;
};

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

out vec4 FragColor;

void main() {
    vec3 lightDir = normalize(light.position - FragPos);
    vec3 diffTexture = vec3(texture(material.diffuse, TexCoords));

    // ambient
    vec3 ambient = light.ambient * diffTexture;

    // diffuse
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * diffTexture;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (
        light.constant
        + light.linear * distance
        + light.quadratic * (distance * distance)
    );

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // we'll leave ambient unaffected so we always have a little light.
    diffuse  *= intensity;
    specular *= intensity;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}
