#version 460 core

in vec2 texCoord;

out vec4 FragColor;

uniform sampler2D ourTexture;

void main()
{
    // FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    FragColor = texture(ourTexture, texCoord);
} 