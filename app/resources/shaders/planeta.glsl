//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoords = aTexCoords;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}


//#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;

uniform vec3 viewPos;

struct DirectionalLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform DirectionalLight dirLight;
uniform PointLight pointLight;

vec3 calculateDirectionalLight(DirectionalLight light, vec3 color)
{
    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(-light.direction);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0),32.0);

    vec3 specular = spec * light.specular;

    vec3 ambient = light.ambient;

    return (ambient + diffuse + specular) * color;
}

vec3 calculatePointLight(PointLight light, vec3 color)
{
    vec3 norm = normalize(Normal);

    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.diffuse;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow( max(dot(viewDir, reflectDir), 0.0), 32.0);

    vec3 specular = spec * light.specular;

    vec3 ambient = light.ambient;

    float distance = length(light.position - FragPos);

    float attenuation = 1.0 / ( light.constant + light.linear * distance + light.quadratic * distance * distance);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular) * color;
}

void main()
{
    vec3 color = texture(texture_diffuse1, TexCoords).rgb;
    vec3 directional = calculateDirectionalLight(dirLight, color);
    vec3 point = calculatePointLight(pointLight, color);
    vec3 result = directional + point;
    FragColor = vec4(result, 1.0);
}