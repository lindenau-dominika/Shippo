#version 460 core

in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;

out vec4 FragColor;

uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;
uniform sampler2D metallicTexture;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
    
    FragColor = vec4(ambient + diffuse + specular, 1.0) * texture(diffuseTexture, texCoord);
} 