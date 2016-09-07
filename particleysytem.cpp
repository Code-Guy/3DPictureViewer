#include "particleysytem.h"
#include "tool.h"
#include "scene.h"

using namespace std;

const float TimeRatioSteps[4] = { 0, 0.33f, 0.66f, 1.0f };

ParticleSystem::ParticleSystem(glm::vec3 position, int emitSpeed, glm::vec3 gravity,
	glm::vec3 ellipsoid, float emitterRange,
	float minSize, float maxSize,
	float minAngle, float maxAngle,
	float minLife, float maxLife,
	glm::vec3 minVec, glm::vec3 maxVec,
	glm::vec3 colorSteps[4])
	: position(position), emitSpeed(emitSpeed), gravity(gravity),
	ellipsoid(ellipsoid), emitterRange(emitterRange),
	minSize(minSize), maxSize(maxSize),
	minAngle(minAngle), maxAngle(maxAngle),
	minLife(minLife), maxLife(maxLife),
	minVec(minVec), maxVec(maxVec)
{
	memcpy(this->colorSteps, colorSteps, sizeof(glm::vec3) * 4);
	emitting = true;

	//创建点buffer
	glGenBuffers(1, &VB_ID);
	glGenBuffers(1, &Size_Angle_B_ID);
	glGenBuffers(1, &Color_B_ID);
}

ParticleSystem::~ParticleSystem()
{
	glDeleteBuffers(1, &VB_ID);
	glDeleteBuffers(1, &Size_Angle_B_ID);
	glDeleteBuffers(1, &Color_B_ID);
}

void ParticleSystem::logic(float deltaTime)
{
	static float accumulator = 0;
	if (emitting)
	{
		accumulator += deltaTime * emitSpeed;
		if (accumulator > 1)
		{
			int emitNum = (int)accumulator;
			emitParticles(emitNum);
			accumulator -= emitNum;
		}
	}

	updateParticles(deltaTime);
}

void ParticleSystem::render(BillboardShader *billboardShader)
{
	if (!poss.empty())
	{
		//填充数据，绘制
		glBindBuffer(GL_ARRAY_BUFFER, VB_ID);
		glBufferData(GL_ARRAY_BUFFER, poss.size() * sizeof(glm::vec3), &poss[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, Size_Angle_B_ID);
		glBufferData(GL_ARRAY_BUFFER, sizeAngles.size() * sizeof(glm::vec2), &sizeAngles[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, Color_B_ID);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors[0], GL_DYNAMIC_DRAW);

		billboardShader->setMV(Scene::getCamera()->getViewMatrix());
		billboardShader->setP(Scene::getCamera()->getProjMatrix());
		texture->bind(GL_TEXTURE0);

		// 1rst attribute buffer : pos
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, VB_ID);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 2rst attribute buffer : size_angle
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, Size_Angle_B_ID);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 3rst attribute buffer : color
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, Color_B_ID);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);


		// Draw call
		glDrawArrays(GL_POINTS, 0, poss.size());

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}
}

void ParticleSystem::setTexture(Texture *texture)
{
	this->texture = texture;
}

void ParticleSystem::setEmitting(bool emitting)
{
	this->emitting = emitting;
}

void ParticleSystem::emitParticles(int num)//初始化粒子系统的各种属性值
{
	for (int i = 0; i < num; i++)
	{
		Particle p;
		p.pos = Tool::uniformEllipsoidSample(ellipsoid, emitterRange) + position;
		p.size = Tool::randFloatToFloat(minSize, maxSize);
		p.angle = Tool::randFloatToFloat(minAngle, maxAngle);
		p.life = Tool::randFloatToFloat(minLife, maxLife);
		p.vec = Tool::randVec3ToVec3(minVec, maxVec);
		p.curTime = 0;

		particles.push_back(p);
	}
}

void ParticleSystem::updateParticles(float deltaTime)
{
	poss.clear();
	sizeAngles.clear();
	colors.clear();

	particles.sort();

	for (list<Particle>::iterator pi = particles.begin(); pi != particles.end();)
	{
		pi->curTime += deltaTime;
		if (pi->curTime > pi->life)
		{
			pi = particles.erase(pi);
			continue;
		}

		glm::vec3 newVec = pi->vec + gravity * deltaTime;//计算新速度
		pi->pos += (pi->vec + newVec) * 0.5f * deltaTime;//更新粒子位置
		pi->vec = newVec;//更新粒子速度
	
		float timeRatio = pi->curTime / pi->life;
		if (timeRatio < TimeRatioSteps[1])
		{
			pi->color = Tool::interpVec3(colorSteps[0], colorSteps[1], (timeRatio - TimeRatioSteps[0]) * 3);
		}
		else if (timeRatio < TimeRatioSteps[2])
		{
			pi->color = Tool::interpVec3(colorSteps[1], colorSteps[2], (timeRatio - TimeRatioSteps[1]) * 3);
		}
		else 
		{
			pi->color = Tool::interpVec3(colorSteps[2], colorSteps[3], (timeRatio - TimeRatioSteps[2]) * 3);
		}

		poss.push_back(pi->pos);
		sizeAngles.push_back(glm::vec2(pi->size, pi->angle));
		colors.push_back(pi->color);

		pi++;
	}
}