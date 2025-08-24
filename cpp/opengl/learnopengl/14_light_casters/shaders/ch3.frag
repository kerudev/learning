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

    float theta = dot(lightDir, normalize(-light.direction));

    // ambient
    vec3 ambient = light.ambient * diffTexture;

    if (theta > light.cutOff) { // do lighting calculations
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

        // remove attenuation from ambient. At large distances the light would
        // be darker inside than outside the spotlight due the ambient term in
        // the else branch

        // ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;

        FragColor = vec4(ambient + diffuse + specular, 1.0);
    }
    else  // else, use ambient light so scene isn't completely dark outside the spotlight.
        FragColor = vec4(ambient, 1.0);
}
