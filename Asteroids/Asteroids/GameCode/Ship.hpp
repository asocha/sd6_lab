//=====================================================
// Ship.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_Ship__
#define __included_Ship__

#include "GameEntity.hpp"
class OpenGLRenderer;
#include "Bullet.hpp"

class Ship : public GameEntity{
private:
	float m_thrustFraction;
	bool m_didThrustThisFrame;
	bool m_isDestroyed;

	const unsigned int THRUSTER_END_VERTEX_INDEX = 4;
	const unsigned int SHIP_FRONT_VERTEX_INDEX = 1;
	const float SHIP_ACCELERATION = 300.0f;

public:
	Ship(const Vec2& position, const OpenGLRenderer& renderer, EngineAndrew::Material& material);

	inline bool IsDestroyed() const{return m_isDestroyed;}

	inline void Destroy() { m_isDestroyed = true; }
	inline void Respawn(const Vec2& initialPosition);

	Bullet* SpawnBullet(const OpenGLRenderer& renderer, EngineAndrew::Material& material);

	inline void RotateCounterClockwise();
	inline void RotateClockwise();
	inline void SetThrust(float thrustFraction);
	inline void SetOrientationDegrees(float orientationDegrees){m_physics.m_orientationDegrees = orientationDegrees;}

	void Update(double deltaSeconds, const OpenGLRenderer& renderer);
	void ApplyThrust(double deltaSeconds);
};


///=====================================================
/// 
///=====================================================
void Ship::RotateCounterClockwise(){
	m_physics.m_orientationDegrees += 6.0f;
}

///=====================================================
/// 
///=====================================================
void Ship::RotateClockwise(){
	m_physics.m_orientationDegrees -= 6.0f;
}

///=====================================================
/// 
///=====================================================
void Ship::SetThrust(float thrustFraction){
	m_thrustFraction = thrustFraction;
	m_didThrustThisFrame = true;
}

///=====================================================
/// 
///=====================================================
void Ship::Respawn(const Vec2& initialPosition){
	m_isDestroyed = false;
	m_physics.m_position = initialPosition;
	m_physics.m_velocity = Vec2(0.0f, 0.0f);
	m_physics.m_orientationDegrees = 90.0f;
}

#endif