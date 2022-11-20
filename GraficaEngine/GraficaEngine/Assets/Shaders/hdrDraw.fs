#version 460 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D colorBuffer;
uniform float exposure;

void main()
{
    const float gamma = 1.5;
    vec3 hdrColor = texture(colorBuffer, TexCoords).rgb;
    // reinhard
    // vec3 result = hdrColor / (hdrColor + vec3(1.0));
    // exposure
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);

    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0);
}
