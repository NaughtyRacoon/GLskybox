
#ifndef camera_h
#define camera_h

#include <vector>
#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
enum Camera_Movement//解决朝向问题
{
    FORWARD,
    BACKWORD,
    LEFT,
    RIGHT
};
const GLfloat YAW         = -90.0f;   //从外往里视角,水平角
const GLfloat PITCH       = 0.0f;     //俯仰角
const GLfloat ZOOM        = 45.0f;    //视域，控制缩放
const GLfloat SPEED       = 6.0f;     //设置初始速度
const GLfloat SENSITIVITY = 0.25f;    //灵敏度

class Camera
{
public:
    //camera位置信息
    Camera(glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f),glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f),GLfloat yaw=YAW,GLfloat pitch = PITCH):front(glm::vec3(0.0f,0.0f,-1.0f)),zoom(ZOOM),movementSpeed(SPEED),mouseSensitivity(SENSITIVITY)
    {
        this->position = position;
        this->worldUP = up;
        this->yaw = yaw;
        this->pitch = pitch;
        this->updateCameraVectors();
        
    }
    glm::mat4 GetViewMatrix()//获得私有变量的值
    {
        return glm::lookAt(this->position,this->position + this->front,this->up);
    }
    GLfloat GetZoom()  //获得私有变量的值
    {
        return this->zoom;
    }
    glm::vec3 GetPosition()
    {
        return  this->position;
    }
    void ProcessKeyboard(Camera_Movement direction,GLfloat deltaTime)
    {
        GLfloat velocity = this->movementSpeed * deltaTime;
        if (direction == FORWARD){
            this->position+=this->front*velocity;
        }
        if (direction == BACKWORD){
            this->position-=this->front*velocity;
        }
        if (direction == LEFT){
            this->position-=this->right*velocity;
        }
        if (direction == RIGHT){
            this->position+=this->right*velocity;
        }
    }
    
    
    void ProcessMouseScroll(GLfloat yOffset)
    {
        if ( this->zoom >= 1.0f && this->zoom <= 45.0f )
        {
            this->zoom += yOffset;
        }
        if ( this->zoom <= 1.0f )
        {
            this->zoom = 1.0f;
        }
        if ( this->zoom >= 45.0f )
        {
            this->zoom = 45.0f;
        }
    }
    
    void ProcessMouseMovement(GLfloat xOffset,GLfloat yOffset)
    {
        //灵敏度调节
        xOffset *= this->mouseSensitivity;
        yOffset *= this->mouseSensitivity;
        
        this->yaw += xOffset;
        this->pitch += yOffset;
        this->updateCameraVectors();//变化完更新相机指针
        
        
    }
private:
    GLfloat yaw;
    GLfloat pitch;
    GLfloat zoom;
    GLfloat movementSpeed;
    GLfloat mouseSensitivity;
    
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 worldUP;
    void updateCameraVectors()
    {
        glm::vec3 front;
        front.x=cos(glm::radians(this->pitch)) * cos(glm::radians(this->yaw));
        front.y=sin(glm::radians(this->pitch));
        front.z=cos(glm::radians(this->pitch)) * sin(glm::radians(this->yaw));
        this->front = glm::normalize(front);
        
        this->right=glm::normalize(glm::cross(this->front,this->worldUP));
        
        this->up = glm::normalize(glm::cross(this->right,this->front));
    }
};

#endif /* camera_h */
