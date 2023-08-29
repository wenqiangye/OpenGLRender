#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

class Light
{
public:
    /* data */
    glm::vec3 postion; //位置
	glm::vec3 direction = glm::vec3(0, 0, 1.0f);
	glm::vec3 angles; //光旋转角
	glm::vec3 ambient=glm::vec3(1.0f,1.0f, 1.0f);
    glm::vec3 diffuse=glm::vec3(1.0f,1.0f, 1.0f);
    glm::vec3 specular=glm::vec3(1.0f,1.0f, 1.0f);
public:
    Light(glm::vec3 _postion, glm::vec3 _direction, glm::vec3 _angles, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular):
    postion(_postion),direction(_direction),angles(_angles),ambient(_ambient), diffuse(_diffuse), specular(_specular)
    {

    }
    Light() {

    }
    //为基类声明virtual 析构函数
    virtual ~Light(){
        
    }
};


// 平行光
// 当一个光源很远的时候，来自光源的每条光线接近于平行。这看起来就像所有的光线来自于同一个方向，无论物体和观察者在哪儿。
// 当一个光源被设置为无限远时，它被称为平行光(Directional Light)，因为所有的光线都有着同一个方向；它会独立于光源的位置。
// 平行光跟光源的位置无关 与旋转角相关

class DirLight:public Light
{
private:

public:
    DirLight(glm::vec3 _direction,glm::vec3 _angles,glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular):
    Light(glm::vec3(1.0f,1.0f,1.0f),_direction,_angles,_ambient,_diffuse,_specular)
    {

    }
    DirLight () {

    }
    DirLight operator= (const DirLight &light) {
        this->postion = light.postion;
        this->angles = light.angles;
        this->ambient = light.ambient;
        this->diffuse = light.diffuse;
        this->specular = light.specular;
        return *this;
    }
    ~DirLight()
    {

    }
};
// 点光源
// 点光是一个在时间里有位置的光源，它向所有方向发光，光线随距离增加逐渐变暗。想象灯泡和火炬作为投光物，它们可以扮演点光的角色。
// 点光源与旋转角无关 跟位置有关
class PointLight:public Light
{
public:
    //衰减
	float constant=1.0f;
	float linear=0.09f;
	float quadratic=0.032f;
public:
    PointLight(glm::vec3 _postion, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular, 
    float _constant, float _linear, float _quadratic) : Light(_postion, glm::vec3(1.0f,1.0f,1.0f),glm::vec3(1.0f,1.0f,1.0f),_ambient,_diffuse,_specular), 
    constant(_constant),linear(_linear),quadratic(_quadratic)
    {

    }
    PointLight() {

    }
    PointLight operator= (const PointLight &light) {
        this->postion = light.postion;
        this->angles = light.angles;
        this->ambient = light.ambient;
        this->diffuse = light.diffuse;
        this->specular = light.specular;
        this->constant = light.constant;
        this->linear = light.linear;
        this->quadratic = light.quadratic;
        return *this;
    }
    ~PointLight() {
        
    }
};

//聚光灯
// 聚光是一种位于环境中某处的光源，它不是向所有方向照射，而是只朝某个方向照射。
// 结果是只有一个聚光照射方向的确定半径内的物体才会被照亮，其他的都保持黑暗。聚光的好例子是路灯或手电筒。
class LightSpot:public Light
{
public:
    float cosPhyInner = glm::cos(glm::radians(12.5f));
	float cosPhyOuter = glm::cos(glm::radians(15.0f));
    //衰减
	float constant = 1.0f;
	float linear=0.09f;
	float quadratic=0.032f;
public:
    LightSpot(glm::vec3 _postion, glm::vec3 _direction_, glm::vec3 _angles, glm::vec3 _ambient, glm::vec3 _diffuse, glm::vec3 _specular,float _cosPhyInner = glm::cos(glm::radians(12.5f)), float _cosPhyOuter = glm::cos(glm::radians(12.5f)))
    :Light(_postion,glm::vec3(1.0f,1.0f,1.0f),_angles,_ambient,_diffuse,_specular), cosPhyInner(_cosPhyInner), cosPhyOuter(_cosPhyOuter)
    {

    }
    LightSpot() {

    }
    LightSpot operator= (const LightSpot &light) {
        this->postion = light.postion;
        this->angles = light.angles;
        this->ambient = light.ambient;
        this->diffuse = light.diffuse;
        this->specular = light.specular;
        this->cosPhyInner = light.cosPhyInner;
        this->cosPhyOuter = light.cosPhyOuter;
        this->constant = light.constant;
        this->linear = light.linear;
        this->quadratic = light.quadratic;
        return *this;
    }
    ~LightSpot(){

    }
    void UpdateDirection()
	{
		//glm旋转方法根据 angles算出direction
		direction = glm::vec3(0, 0, 1.0f);
		direction = glm::rotateZ(direction, angles.z);
		direction = glm::rotateX(direction, angles.x);
		direction = glm::rotateY(direction, angles.y);
		direction = -1.0f*direction;
	}
};

#endif