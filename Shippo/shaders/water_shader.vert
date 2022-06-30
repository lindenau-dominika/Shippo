#version 460 core

layout (location = 0) in vec3 iPosition;
out vec3 fragPos;
uniform mat4 mvp;
uniform mat4 model;
uniform float time;

uniform float waveAmplitude;
uniform float waveLength;
uniform int waveStatic;

out vec4 fWaterColor;
uniform vec4 waterColor;


const float PI = 3.14159;

float calculateOffset(float x, float z, float value1, float value2, float time) {
    float speed = 1.0;

    // Hesus what is this
    float radiansX, radiansZ;
    if(waveStatic == 0) {
        radiansX = ((mod(x + z * x * value1, waveLength) / waveLength) + (time * speed) * mod(x * 0.8 + z, 1.5)) * 2.0 * 3.14;
        radiansZ = ((mod(value2 * (z * x + x * z), waveLength) / waveLength) + (time * speed) * 2.0 * mod(x, 2.0)) * 2.0 * 3.14;
    } else {
        radiansX = (x / waveLength) * 2.0 * 3.14;
        radiansZ = (z / waveLength) * 2.0 * 3.14;
    }
    return waveAmplitude * 0.5 * (sin(radiansZ) + cos(radiansX));
}

vec3 applyWaves(vec3 vertex, float time) {
    float dx = calculateOffset(vertex.x, vertex.z, 0.2, 0.1, time);
    float dy = calculateOffset(vertex.x, vertex.z, 0.1, 0.3, time);
    float dz = calculateOffset(vertex.x, vertex.z, 0.15, 0.2, time);

    return vertex + vec3(dx, dy, dz);
}


void main()
{
    vec3 vertex = applyWaves(iPosition, time * 0.2);
    gl_Position = mvp * vec4(vertex, 1.0);
    fragPos = vec3(model * vec4(iPosition, 1.0));

    float waterScale = (vertex.y + waveAmplitude) / (2 * waveAmplitude);
    float minColorScale = 0.6;
    float convertedColorScale = (waterScale * (1 - minColorScale)) + minColorScale;

    fWaterColor = waterColor * convertedColorScale;

}