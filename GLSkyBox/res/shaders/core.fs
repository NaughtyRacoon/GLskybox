#version 330 core
out vec4 color;
//in vec3 vertexColor;
in vec3 Normal;
in vec3 FragPos;

//uniform运行过程中从CPU传给我们的
uniform float time;
uniform vec3 LightPos;
uniform vec3 ViewPos;

struct Material
{
    float diffuse;
    float specular;
    
};
uniform Material material;

void main()
{
//color = vec4(vertexColor,1.0f);
    vec3 lightDirection = normalize(LightPos-FragPos);//光源方向
    vec3 viewDirection = normalize(ViewPos-FragPos);//视野方向
    vec3 norm = normalize(Normal);
    
    //ambient环境部分
//    vec3 ambient = 0.5 * vertexColor;
    
    //diffuse
    float diff = material.diffuse * max(dot(norm,lightDirection),0.0f);
    //vec3 diffuse = diff * vertexColor;
    
    //specular
    vec3 halfAngle = normalize(viewDirection + lightDirection);
    float spec = material.specular * pow(max (dot(norm,halfAngle),0.0f),64.0f);
    //vec3 specular = spec * vertexColor;
    
    color = vec4(1.0f,0.0f,0.0f,1.0f);
    
  
}



