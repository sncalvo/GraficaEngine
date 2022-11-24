#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;

float cyclingCoord(float x) {
    return mod(x, 2.0) - 1;
}

vec4 cycling(vec4 d) {
    vec3 v = d.xyz / d.w;
    return vec4(cyclingCoord(v.x),cyclingCoord(v.y),cyclingCoord(v.z), 1.0);
}

void main()
{
    gl_Position = model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}