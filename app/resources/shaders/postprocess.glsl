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
    FragColor = vec4(color, 1.0);
}
