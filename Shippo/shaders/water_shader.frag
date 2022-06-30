#version 460 core

in vec3 fragPos;
in vec4 fWaterColor;

out vec4 FragColor;

uniform vec3 sunColor;
uniform vec3 sunDirection;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * sunColor;

    vec3 norm = vec3(0.0, 1.0, 0.0);
    // vec3 lightDir = normalize(lightPos - fragPos);
    vec3 lightDir = normalize(-sunDirection);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * sunColor;

    float specularStrength = 0.3;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * sunColor;  

    FragColor = vec4(ambient + diffuse + specular, 1.0) * fWaterColor;
} 

