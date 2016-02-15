//=====================================================
// Ship.cpp
// by Andrew Socha
//=====================================================

#include "Engine/Core/EngineCore.hpp"
#include "Ship.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/Assert.hpp"

///=====================================================
/// 
///=====================================================
Ship::Ship(const Vec2& position, const OpenGLRenderer& renderer, EngineAndrew::Material& material) :
GameEntity(position, renderer, material),
m_thrustFraction(0.0f),
m_didThrustThisFrame(false),
m_isDestroyed(false){
	//ship
	Vertex_Anim shipV1(Vec3(-20.0f, 10.0f, 0.0f));
	Vertex_Anim shipV2(Vec3(20.0f, 0.0f, 0.0f));
	Vertex_Anim shipV3(Vec3(-20.0f, -10.0f, 0.0f));

	m_mesh.m_vertices.push_back(shipV1);
	m_mesh.m_vertices.push_back(shipV2);
	m_mesh.m_vertices.push_back(shipV3);

	//thruster
	Vertex_Anim thrusterV1(Vec3(-20.0f, -5.0f, 0.0f));
	Vertex_Anim thrusterV2(Vec3(-30.0f, 0.0f, 0.0f));
	Vertex_Anim thrusterV3(Vec3(-20.0f, 5.0f, 0.0f));
	m_mesh.m_vertices.push_back(thrusterV1);
	m_mesh.m_vertices.push_back(thrusterV2);
	m_mesh.m_vertices.push_back(thrusterV3);
	m_mesh.m_vertices.push_back(thrusterV1);

	m_radius = 10.0f;

	m_physics.m_orientationDegrees = 90.0f;

	m_mesh.UseDefaultIndeces();
	m_mesh.SendVertexDataToBuffer(&renderer, true);
}

///=====================================================
/// 
///=====================================================
void Ship::Update(double deltaSeconds, const OpenGLRenderer& renderer){
	static Vec3 thrusterEnd = m_mesh.m_vertices[THRUSTER_END_VERTEX_INDEX].m_position;

	if (m_didThrustThisFrame){
		ApplyThrust(deltaSeconds);
		
		m_mesh.m_vertices[THRUSTER_END_VERTEX_INDEX].m_position = thrusterEnd * (0.5f + 0.5f*m_thrustFraction) * GetRandomFloatInRange(0.9f, 1.1f);

		m_didThrustThisFrame = false;
		m_thrustFraction = 0.0f;
	}
	else {
		m_mesh.m_vertices[THRUSTER_END_VERTEX_INDEX] = thrusterEnd;
	}


	m_mesh.SendVertexDataToBuffer(&renderer, true);

	GameEntity::Update(deltaSeconds, renderer);
}

///=====================================================
/// 
///=====================================================
void Ship::ApplyThrust(double deltaSeconds){
	float acceleration = m_thrustFraction * SHIP_ACCELERATION;
	float deltaVelocityMagnitude = acceleration * (float)deltaSeconds;

	Vec2 deltaVelocity;
	deltaVelocity.SetLengthAndHeadingDegrees(deltaVelocityMagnitude, m_physics.m_orientationDegrees);

	m_physics.m_velocity += deltaVelocity;
}

///=====================================================
/// 
///=====================================================
Bullet* Ship::SpawnBullet(const OpenGLRenderer& renderer, EngineAndrew::Material& material){
	Vec2 bulletLocation = Vec2(m_mesh.m_vertices[SHIP_FRONT_VERTEX_INDEX].m_position);
	bulletLocation.RotateDegrees(m_physics.m_orientationDegrees);
	bulletLocation += m_physics.m_position;

	return new Bullet(bulletLocation, m_physics.m_orientationDegrees, renderer, material);
}

