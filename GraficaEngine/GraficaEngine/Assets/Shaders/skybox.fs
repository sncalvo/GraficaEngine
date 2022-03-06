#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

struct Light
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform samplerCube skybox;
uniform Light light;

void main()
{
    FragColor = vec4(texture(skybox, -TexCoords).xyz * light.diffuse, 1.0);
}
