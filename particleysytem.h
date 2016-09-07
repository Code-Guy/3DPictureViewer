#ifndef _PARTICLESYTEM_H
#define _PARTICLESYTEM_H

#include "texture.h"
#include "billboardshader.h"

#include <vector>
#include <list>

struct Particle
{
	glm::vec3 pos;//λ��
	glm::vec3 vec;//�ٶ�
	glm::vec3 color;//��ɫ
	float size;//��С
	float angle;//��ת�Ƕ�
	float life;//����
	float curTime;//��ǰʱ��

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

	std::vector<glm::vec3> poss;//����λ��
	std::vector<glm::vec2> sizeAngles;//���ӵĴ�С�ͽǶ�
	std::vector<glm::vec3> colors;//������ɫ

	unsigned int VB_ID;
	unsigned int Size_Angle_B_ID;
	unsigned int Color_B_ID;

	glm::vec3 position;//����ϵͳ������λ��
	int emitSpeed;//ÿ��������ٸ�����
	glm::vec3 gravity;//����

	glm::vec3 ellipsoid;
	float emitterRange;

	float minSize, maxSize;
	float minAngle, maxAngle;
	float minLife, maxLife;
	glm::vec3 minVec, maxVec;

	glm::vec3 colorSteps[4];

	bool emitting;

	void emitParticles(int num);//����
	void updateParticles(float deltaTime);//����
};

#endif //_PARTICLESYTEM_H