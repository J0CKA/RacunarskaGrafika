//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos =
        vec3(model * vec4(aPos, 1.0));

    Normal =
        mat3(transpose(inverse(model))) *
        aNormal;

    TexCoords = aTexCoords;

    gl_Position =
        projection *
        view *
        model *
        vec4(aPos, 1.0);
}


//#shader fragment
#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform int hasDiffuseTexture;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vec3 baseColor;

    if (hasDiffuseTexture == 1)
    {
        baseColor =
            texture(texture_diffuse1, TexCoords).rgb;
    }
    else
    {
        baseColor = vec3(0.7, 0.7, 0.7);
    }

    vec3 normal =
        normalize(Normal);

    vec3 lightDirection =
        normalize(lightPos - FragPos);

    float diffuse =
        max(dot(normal, lightDirection), 0.0);

    vec3 viewDirection =
        normalize(viewPos - FragPos);

    vec3 reflectDirection =
        reflect(-lightDirection, normal);

    float specular =
        pow(
            max(
                dot(viewDirection, reflectDirection),
                0.0
            ),
            32.0
        );

    vec3 ambient =
        0.20 * baseColor;

    vec3 diffuseColor =
        0.70 * diffuse * baseColor;

    vec3 specularColor =
        0.30 * specular * vec3(1.0);

    FragColor =
        vec4(
            ambient +
            diffuseColor +
            specularColor,
            1.0
        );
}