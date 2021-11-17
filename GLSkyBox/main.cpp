/*
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
//GLEW
#define GLEW_STATIC
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>
#include <math.h>
#include "Shader.h"
#include "camera.h"
#include "model.h"
#include "light.h"

#include "SOIL2/SOIL2.h"
#include "SOIL2/stb_image.h"

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>//利用旋转所需头文件
using namespace std;
const GLint WIDTH = 800, HEIGHT = 600;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();
GLuint loadTexture(GLchar* path);
GLuint loadCubemap(vector<const GLchar*> faces);

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//主版本号
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//副版本号
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // must for Mac
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);//不让窗口变动大小
    
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Learn OpenGL", nullptr, nullptr);//创建窗口
    if (nullptr == window)  //窗口创建失败
    {
    std::cout<<"Failed to create GLFW window"<<std::endl;
        glfwTerminate();
    return -1;
    }
    //下面两行是针对高清屏
    int screenWidth, screenHeight;
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    
    //当前窗口上下文进行绘制
    glfwMakeContextCurrent( window );
    glewExperimental = GL_TRUE;//使用openGL
    glViewport(0, 0, screenWidth, screenHeight);//最左下角00的位置，宽高都使用
    glewInit();
    glEnable(GL_DEPTH_TEST);
    //glDepthFunc(GL_LESS);
    if (GLEW_OK != glewInit() )
    {
        std::cout << "Failed to initialise GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }
   
 
    Shader ourShader = Shader("res/shaders/core.vs","res/shaders/core.fs");
    Shader skyboxShader = Shader("res/shaders/skybox.vs","res/shaders/skybox.fs");

    GLfloat cubeVertices[] = {
            // Positions          // Normals
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
             0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
             0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
        };
        GLfloat skyboxVertices[] = {
            // Positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
      
            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,
      
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
       
            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,
      
            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,
      
            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f
        };
    
    // Setup cube VAO
       GLuint cubeVAO, cubeVBO;
       glGenVertexArrays(1, &cubeVAO);
       glGenBuffers(1, &cubeVBO);
       glBindVertexArray(cubeVAO);
       glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
       glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
       glEnableVertexAttribArray(0);
       glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
       glEnableVertexAttribArray(1);
       glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
       glBindVertexArray(0);
       // Setup skybox VAO
       GLuint skyboxVAO, skyboxVBO;
       glGenVertexArrays(1, &skyboxVAO);
       glGenBuffers(1, &skyboxVBO);
       glBindVertexArray(skyboxVAO);
       glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
       glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
       glEnableVertexAttribArray(0);
       glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
       glBindVertexArray(0);
    


    
    vector<const GLchar*> faces;
    faces.push_back("right.jpg");
    faces.push_back("left.jpg");
    faces.push_back("top.jpg");
    faces.push_back("bottom.jpg");
    faces.push_back("back.jpg");
    faces.push_back("front.jpg");
    GLuint skyboxTexture = loadCubemap(faces);
    
    
    while (!glfwWindowShouldClose (window)){
        
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        glfwPollEvents();
        Do_Movement();
        
        
        //glfwPollEvents();//获得鼠标信息、键盘操作
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//RGB和透明度，四个
        //初始化赋值，背景色
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        
        //ourShader.use();
        skyboxShader.use();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program,"model"),1,GL_FALSE,glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program,"view"),1,GL_FALSE,glm::value_ptr(view));
                glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3f(glGetUniformLocation(skyboxShader.Program, "ViewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        
        
                skyboxShader.use();
                // Cubes
                glBindVertexArray(skyboxVAO);
                glActiveTexture(GL_TEXTURE0);
                glUniform1i(glGetUniformLocation(skyboxShader.Program, "skybox"), 0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glBindVertexArray(0);

                // Draw skybox as last
                glDepthMask(GL_FALSE);// 关掉深度
                glDepthFunc(GL_LEQUAL);
                
                view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
                glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
                glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
                // skybox cube
                glBindVertexArray(skyboxVAO);
                glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glBindVertexArray(0);
                glDepthFunc(GL_LESS);
                

                // Swap the buffers
                glfwSwapBuffers(window);
        

        
    }
    glfwTerminate();

//    glDeleteVertexArrays(1,&cubeVAO);
//    glDeleteBuffers(1,&cubeVBO);
//    glDeleteVertexArrays(1,&skyboxVAO);
//    glDeleteBuffers(1,&skyboxVBO);

    return 0;
    
    
    
}

GLuint loadCubemap(vector<const GLchar*> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);

    int width,height;
    unsigned char* image;

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for(GLuint i = 0; i < faces.size(); i++)
    {
        image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
            GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
        );
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}
GLuint loadTexture(GLchar* path)
{
    //Generate texture ID and load texture data
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width,height;
    unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);
    return textureID;
}
void Do_Movement()
{
 
    if(keys[GLFW_KEY_W])
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if(keys[GLFW_KEY_S])
        camera.ProcessKeyboard(BACKWORD, deltaTime);
    if(keys[GLFW_KEY_A])
        camera.ProcessKeyboard(LEFT, deltaTime);
    if(keys[GLFW_KEY_D])
        camera.ProcessKeyboard(RIGHT, deltaTime);
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}
*/



#include <iostream>
#include <vector>
using namespace std;
//GLEW
#define GLEW_STATIC
#include <GL/glew.h>

//GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>//利用旋转所需头文件

#include "SOIL2/SOIL2.h"
#include "SOIL2/stb_image.h"

#include "camera.h"
#include "Shader.h"
#include "light.h"


const GLint WIDTH = 800, HEIGHT = 600;
void KeyCallback(GLFWwindow *window , int key ,int scancode, int action,int mode);//退出函数
void ScrollCallback(GLFWwindow *window,double xOffset,double yOffset);//滚轮移动
void MouseCallback(GLFWwindow *window,double xPos,double yPos);//鼠标移动

void DoMovement();//获得当前这帧下按键处理,每次poll event之后才会有键盘操作

GLuint loadTexture(GLchar* path);
GLuint loadCubemap(vector<const GLchar*> faces);

bool keys[1024];//临时变量存储所有键盘操作
Camera camera(glm::vec3(0.0f,0.0f,2.0f));//实例化，往后看

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
GLfloat lastX = WIDTH/2.0f;
GLfloat lastY = HEIGHT/2.0f;
bool firstMouse = true;
int main()
{
glfwInit();
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // must for Mac
glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);//不让窗口变动大小

//大写开头是变量
GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Learn OpenGL", nullptr, nullptr);//创建窗口



if (nullptr == window)
{
std::cout<<"Failed to create GLFW window"<<std::endl;
    glfwTerminate();
return -1;
}


    // next two lines are for mac retina display
    int screenWidth, screenHeight;
    glfwGetFramebufferSize( window, &screenWidth, &screenHeight);
    glfwMakeContextCurrent( window );
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);
    glewExperimental = GL_TRUE;
    

if (GLEW_OK != glewInit() )
{
std::cout << "Failed to initialise GLEW" << std::endl;
return -1;
}
    
    
    glEnable(GL_DEPTH_TEST);//深度测试开启
    glDepthFunc(GL_LESS);//小于就改变
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport( 0, 0, screenWidth, screenHeight );


    Shader ourShader = Shader("res/shaders/core.vs","res/shaders/core.fs");
    Shader lightShader = Shader("res/shaders/light.vs","res/shaders/light.fs");
    Shader skyboxShader = Shader("res/shaders/skybox.vs","res/shaders/skybox.fs");

    
   // Light lightModel = Light();


GLfloat cubeVertices[]=
    {
        //position                 //法向量nomal
       -0.5f,-0.5f,-0.5f,       0.0f,0.0f,-1.0f,
        0.5f,-0.5f,-0.5f,       0.0f,0.0f,-1.0f,
        0.5f, 0.5f,-0.5f,       0.0f,0.0f,-1.0f,
        0.5f, 0.5f,-0.5f,       0.0f,0.0f,-1.0f,
       -0.5f, 0.5f,-0.5f,       0.0f,0.0f,-1.0f,
       -0.5f,-0.5f,-0.5f,       0.0f,0.0f,-1.0f,

        -0.5f,-0.5f,0.5f,       0.0f,0.0f,1.0f,
        0.5f,-0.5f,0.5f,        0.0f,0.0f,1.0f,
        0.5f,0.5f,0.5f,         0.0f,0.0f,1.0f,
        0.5f,0.5f,0.5f,         0.0f,0.0f,1.0f,
        -0.5f,0.5f,0.5f,        0.0f,0.0f,1.0f,
        -0.5f,-0.5f,0.5f,       0.0f,0.0f,1.0f,

        -0.5f,0.5f,0.5f,        -1.0f,0.0f,0.0f,
        -0.5f,0.5f,-0.5f,       -1.0f,0.0f,0.0f,
        -0.5f,-0.5f,-0.5f,      -1.0f,0.0f,0.0f,
        -0.5f,-0.5f,-0.5f,      -1.0f,0.0f,0.0f,
        -0.5f,-0.5f,0.5f,       -1.0f,0.0f,0.0f,
        -0.5f,0.5f,0.5f,        -1.0f,0.0f,0.0f,

        0.5f,0.5f,0.5f,         1.0f,0.0f,0.0f,
        0.5f,0.5f,-0.5f,        1.0f,0.0f,0.0f,
        0.5f,-0.5f,-0.5f,       1.0f,0.0f,0.0f,
        0.5f,-0.5f,-0.5f,       1.0f,0.0f,0.0f,
        0.5f,-0.5f,0.5f,        1.0f,0.0f,0.0f,
        0.5f,0.5f,0.5f,         1.0f,0.0f,0.0f,

        -0.5f,-0.5f,-0.5f,      0.0f,-1.0f,0.0f,
        0.5f,-0.5f,-0.5f,       0.0f,-1.0f,0.0f,
        0.5f,-0.5f,0.5f,        0.0f,-1.0f,0.0f,
        0.5f,-0.5f,0.5f,        0.0f,-1.0f,0.0f,
        -0.5f,-0.5f,0.5f,       0.0f,-1.0f,0.0f,
        -0.5f,-0.5f,-0.5f,      0.0f,-1.0f,0.0f,

        -0.5f,0.5f,-0.5f,       0.0f,1.0f,0.0f,
        0.5f,0.5f,-0.5f,        0.0f,1.0f,0.0f,
        0.5f,0.5f,0.5f,         0.0f,1.0f,0.0f,
        0.5f,0.5f,0.5f,         0.0f,1.0f,0.0f,
        -0.5f,0.5f,0.5f,        0.0f,1.0f,0.0f,
        -0.5f,0.5f,-0.5f,       0.0f,1.0f,0.0f,
    };
    GLfloat skyboxVertices[] = {
           // Positions
           -1.0f,  1.0f, -1.0f,
           -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
           -1.0f,  1.0f, -1.0f,
     
           -1.0f, -1.0f,  1.0f,
           -1.0f, -1.0f, -1.0f,
           -1.0f,  1.0f, -1.0f,
           -1.0f,  1.0f, -1.0f,
           -1.0f,  1.0f,  1.0f,
           -1.0f, -1.0f,  1.0f,
     
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
      
           -1.0f, -1.0f,  1.0f,
           -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
           -1.0f, -1.0f,  1.0f,
     
           -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
           -1.0f,  1.0f,  1.0f,
           -1.0f,  1.0f, -1.0f,
     
           -1.0f, -1.0f, -1.0f,
           -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
           -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
       };
    GLfloat vertices[]=
        {
            //position           //color
           -0.5f,-0.5f,-0.5f,      1.0f,0.0f,0.0f,
            0.5f,-0.5f,-0.5f,      1.0f,0.0f,0.0f,
            0.5f, 0.5f,-0.5f,      1.0f,0.0f,0.0f,
            0.5f, 0.5f,-0.5f,      1.0f,0.0f,0.0f,
           -0.5f, 0.5f,-0.5f,      1.0f,0.0f,0.0f,
           -0.5f,-0.5f,-0.5f,      1.0f,0.0f,0.0f,
            
            -0.5f,-0.5f,0.5f,      0.0f,1.0f,0.0f,
            0.5f,-0.5f,0.5f,       0.0f,1.0f,0.0f,
            0.5f,0.5f,0.5f,        0.0f,1.0f,0.0f,
            0.5f,0.5f,0.5f,        0.0f,1.0f,0.0f,
            -0.5f,0.5f,0.5f,       0.0f,1.0f,0.0f,
            -0.5f,-0.5f,0.5f,      0.0f,1.0f,0.0f,
            
            -0.5f,0.5f,0.5f,       0.0f,0.0f,1.0f,
            -0.5f,0.5f,-0.5f,      0.0f,0.0f,1.0f,
            -0.5f,-0.5f,-0.5f,     0.0f,0.0f,1.0f,
            -0.5f,-0.5f,-0.5f,     0.0f,0.0f,1.0f,
            -0.5f,-0.5f,0.5f,      0.0f,0.0f,1.0f,
            -0.5f,0.5f,0.5f,       0.0f,0.0f,1.0f,
            
            0.5f,0.5f,0.5f,       1.0f,1.0f,0.0f,
            0.5f,0.5f,-0.5f,      1.0f,1.0f,0.0f,
            0.5f,-0.5f,-0.5f,     1.0f,1.0f,0.0f,
            0.5f,-0.5f,-0.5f,     1.0f,1.0f,0.0f,
            0.5f,-0.5f,0.5f,      1.0f,1.0f,0.0f,
            0.5f,0.5f,0.5f,       1.0f,1.0f,0.0f,
            
            -0.5f,-0.5f,-0.5f,  1.0f,0.0f,1.0f,
            0.5f,-0.5f,-0.5f,   1.0f,0.0f,1.0f,
            0.5f,-0.5f,0.5f,    1.0f,0.0f,1.0f,
            0.5f,-0.5f,0.5f,    1.0f,0.0f,1.0f,
            -0.5f,-0.5f,0.5f,   1.0f,0.0f,1.0f,
            -0.5f,-0.5f,-0.5f,  1.0f,0.0f,1.0f,
            
            -0.5f,0.5f,-0.5f,  0.0f,1.0f,1.0f,
            0.5f,0.5f,-0.5f,   0.0f,1.0f,1.0f,
            0.5f,0.5f,0.5f,    0.0f,1.0f,1.0f,
            0.5f,0.5f,0.5f,    0.0f,1.0f,1.0f,
            -0.5f,0.5f,0.5f,   0.0f,1.0f,1.0f,
            -0.5f,0.5f,-0.5f,  0.0f,1.0f,1.0f
        };
    
    GLuint VAO,VBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    
    //绑定 VAO 和 VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);//告诉显卡需要预留空间
    
    //传输数据
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);//static多次读不改数据，dynamic多次读并且修改
    
    //设置VAO
    //location=0的变量，每次读三个浮点数，赋值到position，每隔三个浮点数取一个当作位置信息
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat),(GLvoid *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(GLfloat),(GLvoid *)(3*sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    //下面是解绑操作
    glBindBuffer(GL_ARRAY_BUFFER,0);        //VBO解绑
    glBindVertexArray(0);                   //VAO解绑

    
    
    // Setup cube VAO
        GLuint cubeVAO, cubeVBO;
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        glBindVertexArray(cubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
        glBindVertexArray(0);
    
    
    // Setup skybox VAO
       GLuint skyboxVAO, skyboxVBO;
       glGenVertexArrays(1, &skyboxVAO);
       glGenBuffers(1, &skyboxVBO);
       glBindVertexArray(skyboxVAO);
       glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
       glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
       glEnableVertexAttribArray(0);
       glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
       glBindVertexArray(0);
    
    
    
    vector<const GLchar*> faces;
       faces.push_back("res/images/right.jpg");
       faces.push_back("res/images/left.jpg");
       faces.push_back("res/images/top.jpg");
       faces.push_back("res/images/bottom.jpg");
       faces.push_back("res/images/back.jpg");
       faces.push_back("res/images/front.jpg");
       GLuint skyboxTexture = loadCubemap(faces);
   
    
    
//draw loop 绘制循环
while (!glfwWindowShouldClose (window))
{

    GLfloat currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    
    glViewport(0, 0, screenWidth, screenHeight);
    glfwPollEvents();
    DoMovement();

    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
      
    // Draw scene as normal
    ourShader.use();
    glm::mat4 model;
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)screenWidth/(float)screenHeight, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3f(glGetUniformLocation(ourShader.Program, "cameraPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
    
   
    
    
    // Cubes
    glBindVertexArray(cubeVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    
    // Draw skybox as last
    glDepthFunc(GL_LEQUAL);
    skyboxShader.use();
    view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    
    // skybox cube
    glBindVertexArray(skyboxVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glfwSwapBuffers (window);

}

glfwTerminate();
glDeleteVertexArrays(1,&skyboxVAO);
glDeleteBuffers(1,&skyboxVBO);
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1,&VBO);

return 0;

}

void KeyCallback(GLFWwindow *window,int key,int scancode,int action,int mode)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if(key>=0 && key<1024){
        if(action==GLFW_PRESS){
            keys[key]=true;
        }else if(action==GLFW_RELEASE)
        {
            keys[key]=false;
        }
    }
}

void DoMovement()
{
    if(keys[GLFW_KEY_W] ){
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if(keys[GLFW_KEY_S] ){
        camera.ProcessKeyboard(BACKWORD, deltaTime);
    }
    if(keys[GLFW_KEY_A] ){
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if(keys[GLFW_KEY_D] ){
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
}

void MouseCallback(GLFWwindow *window,double xPos,double yPos)
{
    //取消第一次变换，让图形一开始就出现在屏幕上
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement(xOffset, yOffset);
    
    
}



void ScrollCallback(GLFWwindow *window,double xOffset,double yOffset)
{

    camera.ProcessMouseScroll(yOffset);
    
}



GLuint loadCubemap(vector<const GLchar*> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);

    int width,height;
    unsigned char* image;

    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    for(GLuint i = 0; i < faces.size(); i++)
    {
        image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
        glTexImage2D(
            GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
            GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
        );
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}
GLuint loadTexture(GLchar* path)
{
    //Generate texture ID and load texture data
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width,height;
    unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);
    return textureID;
}
