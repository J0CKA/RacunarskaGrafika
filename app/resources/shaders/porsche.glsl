//#shader vertex
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;
out vec3 Tangent;
out vec3 Bitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));

    mat3 normalMatrix = mat3(transpose(inverse(model)));
    Normal = normalize(normalMatrix * aNormal);
    Tangent = normalize(normalMatrix * aTangent);
    Bitangent = normalize(normalMatrix * aBitangent);

    TexCoords = aTexCoords;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

//#shader fragment
#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 Tangent;
in vec3 Bitangent;

out vec4 FragColor;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;

uniform int hasDiffuseTexture;
uniform int hasSpecularTexture;
uniform int hasNormalTexture;
uniform int hasHeightTexture;

uniform vec4 diffuseColor;
uniform float opacity;
uniform bool isGlass;
uniform bool alphaMask;
uniform float alphaCutoff;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    // --------------------------------------------------
    // MATERIAL / BASE COLOR
    // --------------------------------------------------
    vec4 texColor = vec4(1.0);

    if (hasDiffuseTexture == 1)
        texColor = texture(texture_diffuse1, TexCoords);

    vec3 baseColor = hasDiffuseTexture == 1
        ? texColor.rgb
        : diffuseColor.rgb;

    float alpha = (hasDiffuseTexture == 1 ? texColor.a : diffuseColor.a) * opacity;

    // MASK materials (e.g. Lexus windowmask/grille) are cut out,
    // not smoothly blended.
    if (alphaMask && alpha < alphaCutoff)
        discard;

    // --------------------------------------------------
    // NORMAL MAP
    // --------------------------------------------------
    vec3 normal = normalize(Normal);

    if (hasNormalTexture == 1)
    {
        vec3 tangent = normalize(Tangent);
        tangent = normalize(tangent - dot(tangent, normal) * normal);
        vec3 bitangent = normalize(cross(normal, tangent));

        mat3 TBN = mat3(tangent, bitangent, normal);
        vec3 normalMap = texture(texture_normal1, TexCoords).rgb * 2.0 - 1.0;
        normal = normalize(TBN * normalMap);
    }

    // --------------------------------------------------
    // LIGHTING
    // --------------------------------------------------
    vec3 toLight = lightPos - FragPos;
    float distanceToLight = length(toLight);
    vec3 lightDirection = normalize(toLight);

    float attenuation = 1.0 / (1.0 + 0.045 * distanceToLight + 0.0075 * distanceToLight * distanceToLight);
    float diffuse = max(dot(normal, lightDirection), 0.0);

    vec3 viewDirection = normalize(viewPos - FragPos);
    vec3 halfwayDirection = normalize(lightDirection + viewDirection);

    float shininess = isGlass ? 96.0 : 48.0;
    float specular = pow(max(dot(normal, halfwayDirection), 0.0), shininess);

    float specularStrength = isGlass ? 0.85 : 0.28;

    if (hasSpecularTexture == 1 && !isGlass)
    {
        vec3 specMap = texture(texture_specular1, TexCoords).rgb;
        float specMask = dot(specMap, vec3(0.299, 0.587, 0.114));
        specularStrength *= specMask;
    }

    // --------------------------------------------------
    // BODY / METAL APPEARANCE
    // --------------------------------------------------
    vec3 ambient = 0.24 * baseColor;
    vec3 diffuseLight = 0.78 * diffuse * baseColor * attenuation;
    vec3 specularLight = specularStrength * specular * vec3(1.0) * attenuation;

    // --------------------------------------------------
    // GLASS
    // --------------------------------------------------
    if (isGlass)
    {
        // Keep the windows dark and slightly blue instead of black,
        // while the strong Fresnel/specular highlight makes them read
        // as glass under the showroom light.
        vec3 glassTint = vec3(0.035, 0.075, 0.105);
        baseColor = mix(glassTint, baseColor, 0.20);

        float fresnel = pow(1.0 - max(dot(normal, viewDirection), 0.0), 4.0);
        float glassDiffuse = 0.10 * diffuse * attenuation;

        ambient = 0.08 * baseColor;
        diffuseLight = glassDiffuse * baseColor;
        specularLight = (0.55 + 0.45 * fresnel) * specular * vec3(1.0) * attenuation;

        // Most car glass is visibly transparent but not completely clear.
        alpha = min(alpha, 0.72);
    }

    vec3 finalColor = ambient + diffuseLight + specularLight;

    // A small minimum level prevents dark body materials from becoming
    // completely black when the point light is far from one of the cars.
    finalColor = max(finalColor, baseColor * 0.055);
    finalColor = clamp(finalColor, 0.0, 1.0);

    FragColor = vec4(finalColor, alpha);
}
