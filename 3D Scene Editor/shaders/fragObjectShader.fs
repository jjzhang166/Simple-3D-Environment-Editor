#version 330 core

uniform vec3 lightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float ambientStrength;
uniform float specularStrength;
uniform int reflectivity;

in vec3 normal;
in vec3 fragPos;
out vec4 FragColor;

void main()
{
    // Ambient Component
    vec3 ambient = ambientStrength * lightColor;

    // Diffusion Component
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * lightColor;

    // Specular Component
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), reflectivity);
    vec3 specular = specularStrength * spec * lightColor;

    // Final Result
    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0f);
}