// ZADANIE 3:     ĆWICZENIE 1
// NA PODSTAWIE:  PRZYKŁAD 2
// TREŚĆ:         Poprzez uniform zmienić przy miksowaniu 2 tekstur procentowy stosunek jednej do drugiej.


#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <STB/stb_image.h>
#include <shader.h>
#include <iostream>

const unsigned int SCR_WIDTH  = 800;
const unsigned int SCR_HEIGHT = 600;

float vertices[] = {
    // pozycja           // kolor            // kordynaty tekstury
       0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // górny prawy
       0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // dolny prawy
      -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // dolny lewy
      -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // górny lewy
};

unsigned int indices[] = {
    0, 1, 3, // pierwszy trójkat
    1, 2, 3  // drugi trójkat
};


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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Z3_1", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // INICJALIZACJA GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    // WCZYTYWANIE, KOMPILACJA, LINKOWANIE I INICJALIZACJA ZEWNĘTRZNYCH SHADERÓW
    Shader ourShader("Shaders/Z3_01/vertex.vs", "Shaders/Z3_01/fragment.fs");

	// GENEROWANIE I BINDOWANIE VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

	// GENEROWANIE I BINDOWANIE VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
	// GENEROWANIE I BINDOWANIE EBO
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // ATRYBUTY POZYCJI
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ATRYBUTY KOLORU
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ATRYBUTY TEXTURY
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

	// TWORZENIE TEKSTURY 1
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // USTAWIENIE MAPOWANIA TEXTURY 1
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// USTAWIENIE PARAMETRÓW FILTROWANIA TEXTURY 1
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// ZAŁADOWNIE TEXTURY 1 I GENERACJA MIP-MAP
    int width;
    int height;
	int nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("Resources/box_texture.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else std::cout << "Failed to load texture" << std::endl;
    stbi_image_free(data);

	// TWORZENIE TEKSTURY 2
	unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    // USTAWIENIE MAPOWANIA TEXTURY 2
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // USTAWIENIE PARAMETRÓW FILTROWANIA TEXTURY 2
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// ZAŁADOWNIE TEXTURY 2 I GENERACJA MIP-MAP
    data = stbi_load("Resources/lennyface_texture.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else std::cout << "Failed to load texture" << std::endl;
    stbi_image_free(data);

    // AKTYWACJA SHADERA DO USTAWIENIA SAMPLERÓW
    ourShader.use();

    // USTAWIENIE INFORMACJI, KTÓRY SAMPLER DOTYCZY KTÓREJ TEXTURY
	ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);
    ourShader.setFloat("mixValue", 0.5f);


    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		// PODŁĄCZENIE TEKSTUR DO SAMPLERÓW
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

		// RENDEROWANIE OBIEKTU
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // ZWALNIANIE PAMIĘCI Z BUFFERÓW
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // ZAMYKANIE OKNA
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}