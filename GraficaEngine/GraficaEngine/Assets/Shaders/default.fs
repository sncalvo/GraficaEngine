#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 worldPosition;
in vec4 FragPosLightSpace;

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

uniform sampler2D shadowMap;
uniform sampler2D texture_diffuse1;
uniform bool has_texture;
uniform bool is_flat;
uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform vec2 texture_offset;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    // float bias = max(0.01 * (1.0 - dot(normal, lightDir)), 0.005);  
    float shadow = currentDepth > closestDepth ? 1.0 : 0.0;
    if (projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

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
    float shadow = ShadowCalculation(FragPosLightSpace, norm, lightDir);
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

    vec3 diffuseSpecular = (1.0 - shadow) * (diffuse + specular);
    vec3 result = (1.4 - shadow) * ambient + diffuseSpecular;
    FragColor = vec4(result, 1.0);
}
