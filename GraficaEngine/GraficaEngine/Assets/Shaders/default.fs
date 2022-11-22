#version 460 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 worldPosition;
in vec4 FragPosLightSpace;
in vec4 totalPosition;

layout (std140) uniform LightSpaceMatrices
{
    mat4 lightSpaceMatrices[16];
};
uniform float cascadePlaneDistances[16];
uniform int cascadeCount;   // number of frusta - 1
uniform mat4 view;
uniform float farPlane;

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

uniform sampler2DArray shadowMap;
uniform sampler2D texture_diffuse1;
uniform bool has_texture;
uniform bool is_flat;
uniform Material material;
uniform Light light;
uniform vec3 viewPos;
uniform vec2 texture_offset;

float ShadowCalculationV2(vec3 normal, vec3 lightDir)
{
    vec4 fragPosViewSpace = view * vec4(FragPos, 1.0);
    float depthValue = abs(fragPosViewSpace.z);
    int layer = -1;
    for (int i = 0; i < cascadeCount; ++i)
    {
        if (depthValue < cascadePlaneDistances[i])
        {
            layer = i;
            break;
        }
    }
    if (layer == -1)
    {
        layer = cascadeCount;
    }

    vec4 fragPosLightSpace = lightSpaceMatrices[layer] * vec4(FragPos, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float currentDepth = projCoords.z;
    if (currentDepth > 1.0)
    {
        return 0.0;
    }
    float bias = max(0.01 * (1.0 - dot(normal, lightDir)), 0.003);
    const float biasModifier = 0.5f;
    if (layer == cascadeCount)
    {
        bias *= 1 / (farPlane * biasModifier);
    }
    else
    {
        bias *= 1 / (cascadePlaneDistances[layer] * biasModifier);
    }

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / vec2(textureSize(shadowMap, 0));
    for(int x = -2; x <= 2; ++x)
    {
        for(int y = -2; y <= 2; ++y)
        {
            float pcfDepth = texture(shadowMap, vec3(projCoords.xy + vec2(x, y) * texelSize, layer)).r;
            shadow += (currentDepth - bias) > pcfDepth ? 1.0 : 0.0;        
        }
    }
    shadow /= 25.0;

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
    // float shadow = ShadowCalculation(FragPosLightSpace, norm, lightDir);
    float shadow = ShadowCalculationV2(norm, lightDir);
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
    FragColor = vec4(totalPosition.xyz, 1.0);

    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));

    if(brightness > 1.0)
        BrightColor = vec4(FragColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
}
