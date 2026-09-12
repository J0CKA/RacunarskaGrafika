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

    mat3 normalMatrix =
        mat3(transpose(inverse(model)));

    Normal = normalMatrix * aNormal;
    Tangent = normalMatrix * aTangent;
    Bitangent = normalMatrix * aBitangent;

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

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    // --------------------------------------------------
    // BASE COLOR
    // --------------------------------------------------

    vec4 diffuseTexture = vec4(1.0);

    if (hasDiffuseTexture == 1)
    {
        diffuseTexture = texture(texture_diffuse1, TexCoords);
    }

    vec3 baseColor;

    if (hasDiffuseTexture == 1)
    {
        // Ako postoji tekstura, koristimo NJU kao glavnu boju.
        // Ne množimo je sa diffuseColor jer to kvari postojeće
        // boje modela.
        baseColor = diffuseTexture.rgb;
    }
    else
    {
        // Ako nema teksture, koristimo boju materijala iz Assimp-a.
        baseColor = diffuseColor.rgb;
    }

    float alpha;

    if (hasDiffuseTexture == 1)
    {
        alpha = diffuseTexture.a * opacity;
    }
    else
    {
        alpha = diffuseColor.a * opacity;
    }


    // --------------------------------------------------
    // NORMAL MAP
    // --------------------------------------------------

    vec3 normal = normalize(Normal);

    if (hasNormalTexture == 1)
    {
        vec3 tangent = normalize(Tangent);

        // Ortogonalizacija tangente prema normali
        tangent =
            normalize(
                tangent -
                dot(tangent, normal) * normal
            );

        vec3 bitangent =
            normalize(cross(normal, tangent));

        mat3 TBN =
            mat3(
                tangent,
                bitangent,
                normal
            );

        vec3 normalMap =
            texture(
                texture_normal1,
                TexCoords
            ).rgb;

        normalMap =
            normalMap * 2.0 - 1.0;

        normal =
            normalize(TBN * normalMap);
    }


    // --------------------------------------------------
    // LIGHT
    // --------------------------------------------------

    vec3 lightDirection =
        normalize(lightPos - FragPos);

    float diffuse =
        max(
            dot(normal, lightDirection),
            0.0
        );


    // --------------------------------------------------
    // AMBIENT
    // --------------------------------------------------

    float ambientStrength = 0.18;

    vec3 ambient =
        ambientStrength * baseColor;


    // --------------------------------------------------
    // DIFFUSE
    // --------------------------------------------------

    vec3 diffuseLight =
        0.65 *
        diffuse *
        baseColor;


    // --------------------------------------------------
    // SPECULAR
    // --------------------------------------------------

    vec3 viewDirection =
        normalize(viewPos - FragPos);

    vec3 reflectDirection =
        reflect(
            -lightDirection,
            normal
        );

    float specular =
        pow(
            max(
                dot(
                    viewDirection,
                    reflectDirection
                ),
                0.0
            ),
            32.0
        );

    float specularStrength = 0.08;

    if (hasSpecularTexture == 1)
    {
        // Specular mapa je uglavnom INTENZITET,
        // a ne RGB boja.
        vec3 specularMap =
            texture(
                texture_specular1,
                TexCoords
            ).rgb;

        float specularMask =
            dot(
                specularMap,
                vec3(0.299, 0.587, 0.114)
            );

        specularStrength =
            0.08 * specularMask;
    }

    vec3 specularColor =
        specularStrength *
        specular *
        vec3(1.0);


    // --------------------------------------------------
    // TRANSPARENT MATERIALS
    // --------------------------------------------------

    // Kod stakla ne želimo jak beli specular koji
    // može da ga pretvori u crno/belo.
    float transparencyFactor =
        1.0 - alpha;

    specularColor *=
        mix(
            1.0,
            0.35,
            clamp(transparencyFactor, 0.0, 1.0)
        );


    // --------------------------------------------------
    // FINAL COLOR
    // --------------------------------------------------

    vec3 finalColor =
        ambient +
        diffuseLight +
        specularColor;

    finalColor =
        clamp(
            finalColor,
            0.0,
            1.0
        );

    FragColor =
        vec4(
            finalColor,
            alpha
        );
}