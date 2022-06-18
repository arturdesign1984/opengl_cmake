#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "stb_image.h" // библиотека для загрузки изображений

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <deque>
#include <math.h>

#include "shader.h"
#include "camera.h"

#define DEBUG
//#define __APPLE__
#define STB_IMAGE_IMPLEMENTATION

// функции
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

// void glfwWindowSizeCallback(GLFWwindow* window, int width, int height)
// {
//     windowSizeX = width;
//     windowSizeY = height;
//     glViewport(0,0,windowSizeX, windowSizeY);
// }

// void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
// {
//     if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//     {
//         glfwSetWindowShouldClose(window, GL_TRUE);
//     }
// }

// константы
int windowSizeX = 1920; // ширина окна
int windowSizeY = 1080; // высота окна
int numberOfFrames = 20000; // количество рамок в глубину

// камера
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = windowSizeX / 2.0f;
float lastY = windowSizeY / 2.0f;

bool firstMouse = true;

// тайминги
float deltaTime = 0.0f;	// время между текущим кадром и последним кадром
float lastFrame = 0.0f;

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
        #ifdef DEBUG
        else
        {
            std::cout << "error in class FrameRainbowTwistRGB --> twistFrameColors()" << std::endl;
        }
        #endif
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

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // раскомментируйте эту строку, если используете macOS
#endif

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(windowSizeX, windowSizeY, "Изучаем OpenGL", nullptr, nullptr);
    if (!window)
    {
        std::cout << "glfwCreateWindow failed!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // glfwSetWindowSizeCallback(window, glfwWindowSizeCallback);
    // glfwSetKeyCallback(window,glfwKeyCallback);

    glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

    // сообщаем GLFW, чтобы он захватил наш курсор
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


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

    
    

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    // glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);                          // положение камеры
    // glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);                       // целевая позиция
    // glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);                                 // вектор-вверх
    // glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);                       // вектор-вперед
    // glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    // glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    // glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

    glm::mat4 view;
    glm::mat4 projection;

    int directionX = 1;
    int directionY = 1;
    int directionZ = 1;

    std::deque<glm::vec4> frames;
    for(int k = 0; k < numberOfFrames; k++)
    {
        directionX *= (k%100 == 0) ? (-1) : (1);
        directionY *= (k%100 == 0) ? (-1) : (1);
        directionZ *= (k%100 == 0) ? (-1) : (1);

        float deep = -1.0;//-1 + 1/(k * directionDeep + 1); // sin(glfwGetTime() * directionDeep);
        float angleX = 0.5 * (k%50 < 25) ? glm::sin(glm::degrees(glfwGetTime() * directionX * 10)) : glm::tan(glm::degrees(glfwGetTime() * directionX * 10));//glm::sin(k/5);
        float angleY = 0.5 * (k%50 < 25) ? glm::sin(glm::degrees(glfwGetTime() * directionY * 10)) : glm::cos(glm::degrees(glfwGetTime() * directionY * 10));//glm::sin(glm::degrees(glfwGetTime() * directionY));//glm::sin(k/5);
        float angleZ = 0.5 * (k%50 < 25) ? glm::tan(glm::degrees(glfwGetTime() * directionZ * 10)) : glm::tan(glm::degrees(glfwGetTime() * directionZ * 10)); // glm::sin(glm::degrees(glfwGetTime() * directionZ));//glm::sin(k/5);

        // model = glm::rotate(model, glm::radians(angleX), glm::vec3(1.0f, 0.0f, 0.0f));
        // model = glm::rotate(model, glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
        // model = glm::rotate(model, glm::radians(angleZ), glm::vec3(0.0f, 0.0f, 1.0f));

        // model = glm::translate(model, glm::vec3(0.0,0.0,deep));

        glm::vec4 xyzd(angleX, angleY, angleZ, deep);

        frames.push_back(xyzd);
    }


    std::cout << glfwGetTime() << std::endl;

    int framesCount = 0;
    std::deque<glm::mat4> matrixFrames;
    std::deque<glm::vec3> colorFrames;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // логическая часть работы со временем для каждого кадра
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // обработка ввода
        processInput(window);

        /* Render here */
        glEnable(GL_DEPTH_TEST);
        glClearColor(0,0,0,1); // цвет фона
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // const float radius = 10.0f;
        // float camX = sin(glfwGetTime()) * radius;
        // float camZ = cos(glfwGetTime()) * radius;
        
        // активируем шейдер
        firstShader.use();

        // передаем шейдеру матрицу проекции (поскольку проекционная матрица редко меняется, то нет необходимости делать это для каждого кадра)
        projection = glm::perspective(glm::radians(camera.Zoom), (float)windowSizeX/(float)windowSizeY, 0.1f, 800.0f);

        //                              положение камеры             целевая позиция               вектор-вверх
        // view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f),  glm::vec3(0.0f, 1.0f, 0.0f));
        

        view = camera.GetViewMatrix();

        //view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));
        
        

        // отправляем все в шейдерную программу

        firstShader.setMat4("model", model);
        firstShader.setMat4("projection", projection);
        firstShader.setMat4("view", view);

        // int modelLoc = glGetUniformLocation(firstShader.ID, "model");
        // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        // int viewLoc = glGetUniformLocation(firstShader.ID, "view");
        // glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // int projectionLoc = glGetUniformLocation(firstShader.ID, "projection");
        // glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
 

        
 

        firstShader.use();
        glBindVertexArray(VAO);
        glLineWidth(2);

        if(framesCount < numberOfFrames)
        {
            framesCount++;
        }
        else
        {
            framesCount = 0;
        }

        
        model = glm::rotate(model, glm::radians(frames.at(framesCount).x), glm::vec3(0.5f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(frames.at(framesCount).y), glm::vec3(0.0f, 0.5f, 0.0f));
        model = glm::rotate(model, glm::radians(frames.at(framesCount).z), glm::vec3(0.0f, 0.0f, 0.5f));

        model = glm::translate(model, glm::vec3(0.0, 0.0, frames.at(framesCount).a));

        matrixFrames.push_back(model);

        glm::vec3 setColor = glm::vec3(rainbow.setFrameColor()[Chanel.R],rainbow.setFrameColor()[Chanel.G],rainbow.setFrameColor()[Chanel.B]);
        colorFrames.push_back(setColor);
        rainbow.twistFrameColors(10);

        // camera.Position.z += frames.at(framesCount).a;
        // //camera.Yaw = glm::radians(frames.at(framesCount).y);
        // camera.Pitch = glm::radians(frames.at(framesCount).x);



        //view = glm::translate(view, glm::vec3(1.0f, 0.0f, 0.0f));


        for(int k = 0; k < framesCount; k++)
        {
            //float deep = -1 + 1/(k+1);
            //float angle = 10 * sin(glfwGetTime());//glm::sin(k/5);

            //model = glm::rotate(frames.at(k), glm::radians(angle), glm::vec3(0.2f, 0.2f, 0.2f));
            //model = glm::translate(frames.at(k), glm::vec3(0.0,0.0,deep*2));

            firstShader.setVec3("setColor", colorFrames.at(k));
            // int setColorLoc = glGetUniformLocation(firstShader.ID, "setColor");
            // glUniform3fv(setColorLoc, 1, glm::value_ptr(setColor));

            firstShader.setMat4("model", matrixFrames.at(k));
            // firstShader.setMat4("view", view);

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

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: всякий раз, когда изменяются размеры окна (пользователем или операционной системой), вызывается данная callback-функция
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // Убеждаемся, что окно просмотра соответствует новым размерам окна.
    // Обратите внимание, ширина и высота будут значительно больше, чем указано, на Retina-дисплеях
	glViewport(0, 0, width, height);
}

// glfw: всякий раз, когда перемещается мышь, вызывается данная callback-функция
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // перевернуто, так как y-координаты идут снизу вверх

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: всякий раз, когда прокручивается колесико мыши, вызывается данная callback-функция
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}