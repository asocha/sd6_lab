//=====================================================
// Bullet.cpp
// by Andrew Socha
//=====================================================

#include "Bullet.hpp"
#include "Engine/Time/Time.hpp"

///=====================================================
/// 
///=====================================================
Bullet::Bullet(const Vec2& position, float shipOrientation, const OpenGLRenderer& renderer, EngineAndrew::Material& material) :
GameEntity(position,  renderer, material),
m_spawnTime(GetCurrentSeconds()){
	Vertex_Anim v0(Vec3(0.0f, -0.6f, 0.0f));
	Vertex_Anim v1(Vec3(0.6f, 0.0f, 0.0f));
	Vertex_Anim v2(Vec3(0.0f, 0.6f, 0.0f));
	Vertex_Anim v3(Vec3(-0.6f, 0.0f, 0.0f));
	m_mesh.m_vertices.push_back(v0);
	m_mesh.m_vertices.push_back(v1);
	m_mesh.m_vertices.push_back(v2);
	m_mesh.m_vertices.push_back(v3);

	m_physics.m_orientationDegrees = shipOrientation;
	m_physics.m_velocity.SetLengthAndHeadingDegrees(300.0f, shipOrientation);

	m_radius = 0.6f;

	m_mesh.UseDefaultIndeces();
	m_mesh.SendVertexDataToBuffer(&renderer);
}