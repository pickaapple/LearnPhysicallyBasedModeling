#pragma once
#include <vector>

#include "Particle.h"
#include "Force.h"

class ODESolver;

using namespace std;

class ParticleState
{
public:
	typedef float Element[6];
	ParticleState(const ParticleState& other, bool warning = true);
	ParticleState(ParticleState&& other);
	ParticleState(size_t length);
	~ParticleState();

	void SetState(long long index, const float* state, size_t length);
	const float* GetState(long long index) const;
	void CopyState(long long index, float* state, size_t length) const;

	string ToString() const;
	inline size_t Length() const;
	ParticleState& operator *= (float scale);
	ParticleState& operator +=(ParticleState& other);
	ParticleState& operator=(ParticleState&& other);
	float& operator[](size_t index) const;
protected:
	float* m_State;
	size_t m_Length;
};

size_t ParticleState::Length() const
{
	return m_Length;
}


class ParticleSystem
{
public:
	ParticleSystem();
	//��������ϵͳ״̬�б�
	//һ�����ӵ�״̬Ϊ{position, velocity} 6�ֽ�
	ParticleState GetState() const;
	void SetState(const ParticleState& state);
	//��������ϵͳ״̬�б����ʱ��ĵ���
	//һ������״̬��ʱ����Ϊ{velocity, acceleration} 6�ֽ�
	ParticleState Derivative() const;
	void ZeroParticlesForces();
	void CalculateForces();
	//������������״̬�ж�״̬֮��ļ���ڼ䣬�Ƿ�����ײ��
	//���������ײ��ͨ�����Բ�ֵ�������һ�����ӷ�����ײʱ�̵�״̬��
	//��ײʱ�̣���0 preStateʱ�̿�ʼ����
	bool Collision(const ParticleState& preState, ParticleState& nextState, ODESolver& solver, float& t);

	vector<Particle*> m_Particles;
	vector<Force*> m_Forces;
	float m_Time;
	//ָ��Ϸ�����
	Vector m_GroundNormal;
	float m_Krestitution;// �ص�ϵ�� coefficient of restitution
	float m_GroundY;
};