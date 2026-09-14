//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;

out vec4 FragPos;

void main()
{
    FragPos = model * vec4(aPos, 1.0);
    gl_Position = FragPos;
}

//#shader geometry
#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

in vec4 FragPos[];

uniform mat4 shadowMatrices[6];

void main()
{
    for (int face = 0; face < 6; ++face)
    {
        gl_Layer = face;
        for (int i = 0; i < 3; ++i)
        {
            gl_Position = shadowMatrices[face] * FragPos[i];
            EmitVertex();
        }
        EndPrimitive();
    }
}

//#shader fragment
#version 330 core

in vec4 FragPos;

uniform vec3 lightPos;
uniform float farPlane;

void main()
{
    float lightDistance = length(FragPos.xyz - lightPos);
    gl_FragDepth = lightDistance / farPlane;
}
