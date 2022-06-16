#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <math.h>
#include "shader.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h" // библиотека для загрузки изображений

int gWindowSizeX = 1024; // ширина окна
int gWindowSizeY = 768; // высота окна



class FrameRainbowTwistRGB
{
    private:
    float color[3];
    float r,g,b;
    float speedR,speedG,speedB;
    const float defaultSpeed = 0.003;
    const float revers = -1;

    public:
    FrameRainbowTwistRGB() : r(1), g(1), b(1)
    {
        this->speedR = this->speedG = this->speedB = this->defaultSpeed;
    }
    float* setFrameColor()
    {
        color[0] = r;
        color[1] = g;
        color[2] = b;
        return color;
    }
    void twistFrameColors(const int speed)
    {
        if(r == 1 && g == 1 && b == 1)
        {
            speedB *= revers;
            b = b + (speedB * speed);
        }
        else if(r == 1 && g == 1 && b > 0)
        {
            b = b + (speedB * speed);
            if(b > 1) b = 1;
            if(b < 0) b = 0;
        }
        else if(r == 1 && g == 1 && b == 0)
        {
            speedG *= revers;
            g = g + (speedG * speed);
        }
        else if(r == 1 && g > 0 && b == 0)
        {
            g = g + (speedG * speed);
            if(g > 1) g = 1;
            if(g < 0) g = 0;
        }
        else if(r == 1 && g == 0 && b == 0)
        {
            speedB *= revers;
            b = b + (speedB * speed);
        }
        else if(r == 1 && g == 0 && b < 1)
        {
            b = b + (speedB * speed);
            if(b > 1) b = 1;
            if(b < 0) b = 0;
        }
        else if(r == 1 && g == 0 && b == 1)
        {
            speedR *= revers;
            r = r + (speedR * speed);
        }
        else if(r > 0 && g == 0 && b == 1)
        {
            r = r + (speedR * speed);
            if(r > 1) r = 1;
            if(r < 0) r = 0;
        }
        else if(r == 0 && g == 0 && b == 1)
        {
            speedG *= revers;
            g = g + (speedG * speed);
        }
        else if(r == 0 && g < 1 && b == 1)
        {
            g = g + (speedG * speed);
            if(g > 1) g = 1;
            if(g < 0) g = 0;
        }
        else if(r == 0 && g == 1 && b == 1)
        {
            speedB *= revers;
            b = b + (speedB * speed);
        }
        else if(r == 0 && g == 1 && b > 0)
        {
            b = b + (speedB * speed);
            if(b > 1) b = 1;
            if(b < 0) b = 0;
        }
        else if(r == 0 && g == 1 && b == 0)
        {
            speedR *= revers;
            r = r + (speedR * speed);
            speedB *= revers;
            b = b + (speedB * speed);
        }
        else if(r < 1 && g == 1 && b < 1)
        {
            b = b + (speedB * speed);
            if(b > 1) b = 1;
            if(b < 0) b = 0;
            r = r + (speedR * speed);
            if(r > 1) r = 1;
            if(r < 0) r = 0;
        }
    }

};

struct EnumChanel
{
    const int R = 0;
    const int G = 1;
    const int B = 2;
    const int A = 3;
} Chanel;

GLfloat points[] = {
    -0.5,-0.5,0.0,
    -0.5,0.5,0.0,
    0.5,0.5,0.0,
    0.5,-0.5,0.0,
    

};

GLfloat colors[] = {
    1.0,1.0,1.0,
    1.0,1.0,1.0,
    1.0,1.0,1.0,
    1.0,1.0,1.0
};



void glfwWindowSizeCallback(GLFWwindow* window, int width, int height)
{
    gWindowSizeX = width;
    gWindowSizeY = height;
    glViewport(0,0,gWindowSizeX, gWindowSizeY);
}

void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

int main(int argc, char** argv)
{

    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "Can't load GLFW!" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(gWindowSizeX, gWindowSizeY, "Изучаем OpenGL", nullptr, nullptr);
    if (!window)
    {
        std::cout << "glfwCreateWindow failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);
    glfwSetKeyCallback(window,glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if(!gladLoadGL())
    {
        std::cout << "Can't load GLAD!" << std::endl;
        return -1;
    }

    std::cout << "Rnderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;



    Shader firstShader("shaders\\firstShader.vs","shaders\\firstShader.fs", argv[0]);

    FrameRainbowTwistRGB rainbow;

    GLuint pointsVBO = 0;
    glGenBuffers(1, &pointsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

    GLuint colorsVBO = 0;
    glGenBuffers(1, &colorsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);


    GLuint VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, pointsVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorsVBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    



    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glEnable(GL_DEPTH_TEST);
        glClearColor(0,0,0,1); // цвет фона
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 1024.0f / 768.0f, 0.1f, 100.0f);

        int modelLoc = glGetUniformLocation(firstShader.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        int viewLoc = glGetUniformLocation(firstShader.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int projectionLoc = glGetUniformLocation(firstShader.ID, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
 

        glm::vec3 setColor = glm::vec3(rainbow.setFrameColor()[Chanel.R],rainbow.setFrameColor()[Chanel.G],rainbow.setFrameColor()[Chanel.B]);
        rainbow.twistFrameColors(10);
        int setColorLoc = glGetUniformLocation(firstShader.ID, "setColor");
        glUniform3fv(setColorLoc, 1, glm::value_ptr(setColor));
 

        firstShader.use();
        glBindVertexArray(VAO);
        glLineWidth(2);

        for(int k = 0; k < 30; k++)
        {
            float deep = -1 + 1/(k+1);
            float angle = 10 * glm::sin(k/5);

            model = glm::rotate(model, glm::radians(angle), glm::vec3(0.2f, 0.2f, 0.2f));
            model = glm::translate(model, glm::vec3(0.0,0.0,deep));
            firstShader.setMat4("model", model);
        
            glDrawArrays(GL_LINE_LOOP, 0, 4);
        }

        //glDrawArrays(GL_LINE_LOOP, 0, 4);
 
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}