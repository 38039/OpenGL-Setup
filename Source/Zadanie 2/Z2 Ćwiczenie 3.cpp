// ZADANIE 2:     ĆWICZENIE 3
// NA PODSTAWIE:  Z2 Ćwiczenie 2.cpp
// TREŚĆ:         Zmodyfikować vertex shader oraz fragment shader, żeby narysować trójkąt, w którym położenie danego wierzchołka będzie jednocześnie jego
//                kolorem(przekazać z vertex shader do fragment shader położenie danego wierzchołka jako jego kolor).


#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// WYMIARY OKNA
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// WSPÓŁRZĘDNE WIERZCHOŁKÓW TRÓJKĄTA
float vertices[] = {
         0.5f, -0.5f, 0.0f,  // dolny prawy
        -0.5f, -0.5f, 0.0f,  // dolny lewy
         0.0f,  0.5f, 0.0f   // górny
};

// KOD ŹRÓDŁOWY VERTEX SHADERA 
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
out vec3 vertexColor;
uniform float offset;
void main() { 
    gl_Position = vec4(aPos.x + offset, -aPos.y, aPos.z, 1.0);
    vertexColor = aPos; // pozycja jako kolor 
}
)";

// KOD ŹRÓDŁOWY FRAGMENT SHADERA
const char* fragmentShaderSource = R"(
#version 330 core
in vec3 vertexColor;
out vec4 FragColor;
void main() { FragColor = vec4(vertexColor, 1.0); }
)";


// OBSŁUGUJE ZMIANĘ ROZMIARU OKNA
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// OBSŁUGUJE INPUT Z KLAWIATURY
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// GŁÓWNA FUNKCJA
int main(void) {
    glfwInit(); // INICJALIZACJA I KONFIGURACJA GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // UTWORZENIE OKNA
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Z2_3", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // INICJALIZACJA GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    // GENEROWANIE I BINDOWANIE VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // GENEROWANIE I BINDOWANIE VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // PRZEKAZANIE DANYCH DO BUFORU
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // KOMPILACJA KODU ŹRÓDŁOWEGO VERTEX SHADERA
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // KOMPILACJA KODU ŹRÓDŁOWEGO FRAGMENT SHADERA
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // LINKOWANIE SKOMPILOWANEGO KODU DO PROGRAMU
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // USTAWIENIE UNIFORMA
    int offsetLocation = glGetUniformLocation(shaderProgram, "offset");
    glUseProgram(shaderProgram);
	  glUniform1f(offsetLocation, 0.5f);  // Przesunięcie trójkąta w prawo

    // ZWALNIANIE PAMIĘCI Z KODU ŹRÓDŁOWEGO SHADERÓW
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ZWALNIANIE PAMIĘCI Z BUFFERÓW I SHADERÓW
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // ZAMYKANIE OKNA
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}