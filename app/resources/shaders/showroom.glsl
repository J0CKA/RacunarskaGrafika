//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    TexCoord = aTexCoord;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

//#shader fragment
#version 330 core

in vec2 TexCoord;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D texture1;
uniform samplerCube shadowMap;
uniform vec3 lightPos;
uniform float farPlane;
uniform int shadowsEnabled;

float pointShadow(vec3 fragPos)
{
    vec3 fragToLight = fragPos - lightPos;
    float currentDepth = length(fragToLight);

    float shadow = 0.0;
    float bias = 0.10;
    float diskRadius = 0.035;

    const vec3 offsets[12] = vec3[](
        vec3( 1.0,  0.0,  0.0), vec3(-1.0,  0.0,  0.0),
        vec3( 0.0,  1.0,  0.0), vec3( 0.0, -1.0,  0.0),
        vec3( 0.0,  0.0,  1.0), vec3( 0.0,  0.0, -1.0),
        vec3( 1.0,  1.0,  0.0), vec3(-1.0,  1.0,  0.0),
        vec3( 1.0, -1.0,  0.0), vec3(-1.0, -1.0,  0.0),
        vec3( 0.0,  1.0,  1.0), vec3( 0.0, -1.0, -1.0)
    );

    for (int i = 0; i < 12; ++i)
    {
        float closestDepth = texture(shadowMap, fragToLight + offsets[i] * diskRadius).r;
        closestDepth *= farPlane;
        if (currentDepth - bias > closestDepth)
            shadow += 1.0;
    }

    return shadow / 12.0;
}

void main()
{
    vec4 color = texture(texture1, TexCoord);
    float shadow = 0.0;

    if (shadowsEnabled == 1)
        shadow = pointShadow(FragPos);

    // Namerno ostavljamo deo svetla u senci da senka bude jasno vidljiva,
    // ali ne potpuno crna.
    color.rgb *= mix(1.0, 0.28, shadow);
    FragColor = color;
}
