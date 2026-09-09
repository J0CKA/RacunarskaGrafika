#include <../libs/glad/include/glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include <../libs/stb/include/stb_image.h>

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec2 aTexCoord;\n"
"out vec2 TexCoord;\n"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main() {\n"
"   gl_Position = projection * view * model * vec4(aPos, 1.0f);\n"
"   TexCoord = aTexCoord;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D texture1;\n"
"void main() {\n"
"   FragColor = texture(texture1, TexCoord);\n"
"}\0";

unsigned int loadTexture(char const *path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format = (nrComponents == 4) ? GL_RGBA : GL_RGB;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        std::cout << "Uspesno ucitana tekstura: " << path << std::endl;
    } else {
        std::cout << "Greska pri ucitavanju teksture na putanji: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1280, 720, "Car Showroom - Salon Automobila", NULL, NULL);
    if (window == NULL) {
        std::cout << "Greska pri kreiranju GLFW prozora!" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Greska pri inicijalizaciji GLAD-a!" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // Kompajliranje šejdera
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Učitavanje tekstura
    unsigned int floorTexture = loadTexture("resources/textures/tiles.jpg");
    unsigned int wallTexture = loadTexture("resources/textures/zidovi.jpg");
    unsigned int podiumTexture = loadTexture("resources/textures/podium1.jpg");

    // Pod
    float floorVertices[] = {
         10.0f, -0.5f,  10.0f,   10.0f,  0.0f,
        -10.0f, -0.5f,  10.0f,    0.0f,  0.0f,
        -10.0f, -0.5f, -10.0f,    0.0f, 10.0f,

         10.0f, -0.5f,  10.0f,   10.0f,  0.0f,
        -10.0f, -0.5f, -10.0f,    0.0f, 10.0f,
         10.0f, -0.5f, -10.0f,   10.0f, 10.0f
    };

    // Zadnji zid (z = -10.0f)
    float backWallVertices[] = {
         10.0f,  5.0f, -10.0f,   4.0f, 2.0f,
        -10.0f,  5.0f, -10.0f,   0.0f, 2.0f,
        -10.0f, -0.5f, -10.0f,   0.0f, 0.0f,

         10.0f,  5.0f, -10.0f,   4.0f, 2.0f,
        -10.0f, -0.5f, -10.0f,   0.0f, 0.0f,
         10.0f, -0.5f, -10.0f,   4.0f, 0.0f
    };

    // Levi zid (x = -10.0f)
    float leftWallVertices[] = {
        -10.0f,  5.0f,  10.0f,   4.0f, 2.0f,
        -10.0f,  5.0f, -10.0f,   0.0f, 2.0f,
        -10.0f, -0.5f, -10.0f,   0.0f, 0.0f,

        -10.0f,  5.0f,  10.0f,   4.0f, 2.0f,
        -10.0f, -0.5f, -10.0f,   0.0f, 0.0f,
        -10.0f, -0.5f,  10.0f,   4.0f, 0.0f
    };

    // Desni zid (x = 10.0f)
    float rightWallVertices[] = {
         10.0f,  5.0f, -10.0f,   4.0f, 2.0f,
         10.0f,  5.0f,  10.0f,   0.0f, 2.0f,
         10.0f, -0.5f,  10.0f,   0.0f, 0.0f,

         10.0f,  5.0f, -10.0f,   4.0f, 2.0f,
         10.0f, -0.5f,  10.0f,   0.0f, 0.0f,
         10.0f, -0.5f, -10.0f,   4.0f, 0.0f
    };

    // Generisanje pravog 3D cilindra (podijuma) sa gornjom pločom i bočnim stranicama
    const int segments = 32;
    float podiumRadius = 2.8f;
    float podiumHeight = 0.25f;
    float baseY = -0.5f;
    float topY = baseY + podiumHeight;

    std::vector<float> podiumVertices;
    for (int i = 0; i < segments; ++i) {
        float angle1 = i * 2.0f * 3.14159265359f / segments;
        float angle2 = (i + 1) * 2.0f * 3.14159265359f / segments;

        float x1 = podiumRadius * cos(angle1);
        float z1 = podiumRadius * sin(angle1);
        float x2 = podiumRadius * cos(angle2);
        float z2 = podiumRadius * sin(angle2);

        // 1. Gornja ploča podijuma (trougao ka centru)
        podiumVertices.push_back(0.0f); podiumVertices.push_back(topY); podiumVertices.push_back(0.0f);
        podiumVertices.push_back(0.5f); podiumVertices.push_back(0.5f);

        podiumVertices.push_back(x1); podiumVertices.push_back(topY); podiumVertices.push_back(z1);
        podiumVertices.push_back((x1 / podiumRadius + 1.0f) * 0.5f); podiumVertices.push_back((z1 / podiumRadius + 1.0f) * 0.5f);

        podiumVertices.push_back(x2); podiumVertices.push_back(topY); podiumVertices.push_back(z2);
        podiumVertices.push_back((x2 / podiumRadius + 1.0f) * 0.5f); podiumVertices.push_back((z2 / podiumRadius + 1.0f) * 0.5f);

        // 2. Bočni omotač (dva trougla za visinu cilindra)
        podiumVertices.push_back(x1); podiumVertices.push_back(topY); podiumVertices.push_back(z1);
        podiumVertices.push_back(0.0f); podiumVertices.push_back(1.0f);

        podiumVertices.push_back(x1); podiumVertices.push_back(baseY); podiumVertices.push_back(z1);
        podiumVertices.push_back(0.0f); podiumVertices.push_back(0.0f);

        podiumVertices.push_back(x2); podiumVertices.push_back(baseY); podiumVertices.push_back(z2);
        podiumVertices.push_back(1.0f); podiumVertices.push_back(0.0f);

        podiumVertices.push_back(x1); podiumVertices.push_back(topY); podiumVertices.push_back(z1);
        podiumVertices.push_back(0.0f); podiumVertices.push_back(1.0f);

        podiumVertices.push_back(x2); podiumVertices.push_back(baseY); podiumVertices.push_back(z2);
        podiumVertices.push_back(1.0f); podiumVertices.push_back(0.0f);

        podiumVertices.push_back(x2); podiumVertices.push_back(topY); podiumVertices.push_back(z2);
        podiumVertices.push_back(1.0f); podiumVertices.push_back(1.0f);
    }

    // VAO i VBO za pod
    unsigned int floorVAO, floorVBO;
    glGenVertexArrays(1, &floorVAO);
    glGenBuffers(1, &floorVBO);
    glBindVertexArray(floorVAO);
    glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // VAO i VBO za zadnji zid
    unsigned int backWallVAO, backWallVBO;
    glGenVertexArrays(1, &backWallVAO);
    glGenBuffers(1, &backWallVBO);
    glBindVertexArray(backWallVAO);
    glBindBuffer(GL_ARRAY_BUFFER, backWallVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(backWallVertices), backWallVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // VAO i VBO za levi zid
    unsigned int leftWallVAO, leftWallVBO;
    glGenVertexArrays(1, &leftWallVAO);
    glGenBuffers(1, &leftWallVBO);
    glBindVertexArray(leftWallVAO);
    glBindBuffer(GL_ARRAY_BUFFER, leftWallVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(leftWallVertices), leftWallVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // VAO i VBO za desni zid
    unsigned int rightWallVAO, rightWallVBO;
    glGenVertexArrays(1, &rightWallVAO);
    glGenBuffers(1, &rightWallVBO);
    glBindVertexArray(rightWallVAO);
    glBindBuffer(GL_ARRAY_BUFFER, rightWallVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rightWallVertices), rightWallVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // VAO i VBO za podijum
    unsigned int podiumVAO, podiumVBO;
    glGenVertexArrays(1, &podiumVAO);
    glGenBuffers(1, &podiumVBO);
    glBindVertexArray(podiumVAO);
    glBindBuffer(GL_ARRAY_BUFFER, podiumVBO);
    glBufferData(GL_ARRAY_BUFFER, podiumVertices.size() * sizeof(float), podiumVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glClearColor(0.15f, 0.16f, 0.21f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 2.0f, 10.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)1280 / (float)720, 0.1f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // 1. Iscrtavanje poda
        glm::mat4 model = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);
        glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
        glBindVertexArray(floorVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // 2. Iscrtavanje zidova
        glBindTexture(GL_TEXTURE_2D, wallTexture);

        glBindVertexArray(backWallVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(leftWallVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindVertexArray(rightWallVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // 3. Iscrtavanje 5 podijuma za automobile sa novom podijum tekstukrom i razmakom
        glBindTexture(GL_TEXTURE_2D, podiumTexture);
        glBindVertexArray(podiumVAO);

        // Centralni podijum
        model = glm::mat4(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, podiumVertices.size() / 5);

        // Levi prednji podijum
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-6.0f, 0.0f, 2.5f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, podiumVertices.size() / 5);

        // Levi zadnji podijum
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-6.0f, 0.0f, -4.5f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, podiumVertices.size() / 5);

        // Desni prednji podijum
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(6.0f, 0.0f, 2.5f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, podiumVertices.size() / 5);

        // Desni zadnji podijum
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(6.0f, 0.0f, -4.5f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, podiumVertices.size() / 5);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &floorVAO);
    glDeleteBuffers(1, &floorVBO);
    glDeleteVertexArrays(1, &backWallVAO);
    glDeleteBuffers(1, &backWallVBO);
    glDeleteVertexArrays(1, &leftWallVAO);
    glDeleteBuffers(1, &leftWallVBO);
    glDeleteVertexArrays(1, &rightWallVAO);
    glDeleteBuffers(1, &rightWallVBO);
    glDeleteVertexArrays(1, &podiumVAO);
    glDeleteBuffers(1, &podiumVBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}