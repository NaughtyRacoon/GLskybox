#version 330 core
layout(location = 0) in vec3 position;
uniform mat4 transform;//顶点位置修改
uniform mat4 view;
uniform mat4 projection;
void main()
{
    gl_Position = projection * view * transform * vec4(position,1.0f);
}


