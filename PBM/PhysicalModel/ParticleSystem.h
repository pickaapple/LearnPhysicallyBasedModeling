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
	//返回粒子系统状态列表
	//一个粒子的状态为{position, velocity} 6字节
	ParticleState GetState() const;
	void SetState(const ParticleState& state);
	//返回粒子系统状态列表关于时间的导数
	//一个粒子状态对时间求导为{velocity, acceleration} 6字节
	ParticleState Derivative() const;
	void ZeroParticlesForces();
	void CalculateForces();
	//根据两个粒子状态判断状态之间的间隔期间，是否发生碰撞。
	//如果发生碰撞则通过线性插值计算出第一个粒子发生碰撞时刻的状态。
	//碰撞时刻，从0 preState时刻开始计算
	bool CollisionDetection(const ParticleState& preState, ParticleState& nextState, ODESolver& solver, float& collisionTime);
	void CollisionResponse(ODESolver& solver);
	vector<Particle*> m_Particles;
	vector<Force*> m_Forces;
	float m_Time;
	//指向合法区域
	Vector m_GroundNormal;
	float m_Kfriction; // 地面摩擦力系数
	float m_Krestitution;// 地面回弹系数 coefficient of restitution
	float m_GroundY;
};