#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 worldPosition;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D texture_diffuse1;
uniform bool has_texture;
uniform bool is_flat;
uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform vec2 texture_offset;

void main()
{
    vec3 norm;
    if (is_flat)
    {
        vec3 xTangent = dFdx(worldPosition);
        vec3 yTangent = dFdy(worldPosition);
        norm = normalize(cross(xTangent, yTangent));
    }
    else
    {
        norm = normalize(Normal);
    }

    vec3 lightDir = normalize(-light.direction);
    vec3 textureColor = texture(texture_diffuse1, TexCoords + texture_offset).xyz;

    // ambient
    vec3 ambient = light.ambient;
    if (has_texture)
    {
        ambient = ambient * textureColor;
    }
    else
    {
        ambient = ambient * material.diffuse;
    }

    // diffuse
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse;
    if (has_texture)
    {
        diffuse = diffuse * diff * textureColor;
    }
    else
    {
        diffuse = diffuse * (diff * material.diffuse);
    }

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}
