//#shader vertex
#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
    TexCoord = aTexCoord;
}

//#shader fragment
#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D screenTexture;
uniform int enabled;

void main()
{
    vec3 color = texture(screenTexture, TexCoord).rgb;

    if (enabled == 1)
    {
        color = (color - 0.5) * 1.10 + 0.5;

        float dist = distance(TexCoord, vec2(0.5));
        float vignette = 1.0 - smoothstep(0.25, 0.75, dist);
        color *= mix(0.72, 1.0, vignette);
    }

    FragColor = vec4(color, 1.0);
}
