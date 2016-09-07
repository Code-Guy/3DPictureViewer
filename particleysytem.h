#ifndef _PARTICLESYTEM_H
#define _PARTICLESYTEM_H

#include "texture.h"
#include "billboardshader.h"

#include <vector>
#include <list>

struct Particle
{
	glm::vec3 pos;//位置
	glm::vec3 vec;//速度
	glm::vec3 color;//颜色
	float size;//大小
	float angle;//旋转角度
	float life;//寿命
	float curTime;//当前时间

	bool operator <(const Particle &rhs)
	{
		return this->pos.z < rhs.pos.z;
	}
};

class ParticleSystem
{
public:
	ParticleSystem(glm::vec3 position, int emitSpeed, glm::vec3 gravity,
		glm::vec3 ellipsoid, float emitterRange,
		float minSize, float maxSize,
		float minAngle, float maxAngle,
		float minLife, float maxLife,
		glm::vec3 minVec, glm::vec3 maxVec,
		glm::vec3 colorSteps[4]);

	~ParticleSystem();

	void logic(float deltaTime);
	void render(BillboardShader *billboardShader);

	void setTexture(Texture *texture);
	void setEmitting(bool emitting);

private:
	Texture *texture;

	std::list<Particle> particles;

	std::vector<glm::vec3> poss;//粒子位置
	std::vector<glm::vec2> sizeAngles;//粒子的大小和角度
	std::vector<glm::vec3> colors;//粒子颜色

	unsigned int VB_ID;
	unsigned int Size_Angle_B_ID;
	unsigned int Color_B_ID;

	glm::vec3 position;//粒子系统的中心位置
	int emitSpeed;//每秒产生多少个粒子
	glm::vec3 gravity;//重力

	glm::vec3 ellipsoid;
	float emitterRange;

	float minSize, maxSize;
	float minAngle, maxAngle;
	float minLife, maxLife;
	glm::vec3 minVec, maxVec;

	glm::vec3 colorSteps[4];

	bool emitting;

	void emitParticles(int num);//发射
	void updateParticles(float deltaTime);//更新
};

#endif //_PARTICLESYTEM_H