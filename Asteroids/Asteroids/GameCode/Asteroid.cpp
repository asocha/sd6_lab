//=====================================================
// Asteroid.cpp
// by Andrew Socha
//=====================================================

#include "Asteroid.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/Material.hpp"

const float Asteroid::BASE_ASTEROID_RADIUS = 6.5f;

std::vector<Vertex_Anim> Asteroid::ASTEROID_VERTICES_CROSS;
std::vector<Vertex_Anim> Asteroid::ASTEROID_VERTICES_MUSHROOM;
std::vector<Vertex_Anim> Asteroid::ASTEROID_VERTICES_TREE;
std::vector<Vertex_Anim> Asteroid::ASTEROID_VERTICES_TEXAS;
std::vector<Vertex_Anim> Asteroid::ASTEROID_VERTICES[4];

///=====================================================
/// 
///=====================================================
Asteroid::Asteroid(const Vec2& position, AsteroidSize asteroidSize, const OpenGLRenderer& renderer, EngineAndrew::Material& material) :
GameEntity(position, renderer, material),
m_size(asteroidSize),
m_shape((Asteroid::AsteroidShape)GetRandomIntLessThan(4)) {
	if (ASTEROID_VERTICES_CROSS.empty())
		CreateVerticesBasedOnShape();

	m_mesh.m_vertices = ASTEROID_VERTICES[m_shape];

	m_physics.m_velocity = Vec2(GetRandomFloatInRange(20.0f, 40.0f), GetRandomFloatInRange(20.0f, 40.0f));
	if (GetRandomIntLessThan(2)) m_physics.m_velocity.x = -m_physics.m_velocity.x;
	if (GetRandomIntLessThan(2)) m_physics.m_velocity.y = -m_physics.m_velocity.y;

	m_physics.m_orientationDegrees = GetRandomFloatInRange(0.0f, 360.0f);

	m_physics.m_angularVelocity = GetRandomFloatInRange(20.0f, 40.0f);
	if (GetRandomIntLessThan(2)) m_physics.m_angularVelocity = -m_physics.m_angularVelocity;

	m_radius = BASE_ASTEROID_RADIUS * m_size;

	m_mesh.UseDefaultIndeces();
	m_mesh.SendVertexDataToBuffer(&renderer);
}

///=====================================================
/// 
///=====================================================
void Asteroid::Draw(const EngineAndrew::Material& material, UniformMatrix* objectToWorld) const {
	FATAL_ASSERT(objectToWorld != nullptr);
	Matrix4 modelMatrix = Matrix4::CreateScale((float)m_size);
	modelMatrix.RotateDegreesAboutZ(m_physics.m_orientationDegrees);
	modelMatrix.Translate(m_physics.m_position);
	objectToWorld->m_data[0] = modelMatrix;

	material.Render(m_mesh);
}

///=====================================================
/// 
///=====================================================
void Asteroid::CreateVerticesBasedOnShape() const{
	Vertex_Anim v0, v1, v2, v3, v4, v5, v6, v7;
	v0.m_position = Vec3(-7.0f, -7.0f, 0.0f);
	v1.m_position = Vec3(-7.0f, 0.0f);
	v2.m_position = Vec3(0.0f, 7.0f);
	v3.m_position = Vec3(7.0f, 0.0f);
	v4.m_position = Vec3(7.0f, -7.0f);
	v5.m_position = Vec3(0.0f, -14.0f);
	ASTEROID_VERTICES_CROSS.push_back(v0);
	ASTEROID_VERTICES_CROSS.push_back(v1);
	ASTEROID_VERTICES_CROSS.push_back(v2);
	ASTEROID_VERTICES_CROSS.push_back(v3);
	ASTEROID_VERTICES_CROSS.push_back(v4);
	ASTEROID_VERTICES_CROSS.push_back(v5);

	v0.m_position = Vec3(-7.0f, -7.0f);
	v1.m_position = Vec3(7.0f, -7.0f);
	v2.m_position = Vec3(0.0f, 7.0f);
	ASTEROID_VERTICES_MUSHROOM.push_back(v0);
	ASTEROID_VERTICES_MUSHROOM.push_back(v1);
	ASTEROID_VERTICES_MUSHROOM.push_back(v2);

	v0.m_position = Vec3(-3.0f, -6.0f);
	v1.m_position = Vec3(-6.0f, -3.0f);
	v2.m_position = Vec3(-7.0f, 0.0f);
	v3.m_position = Vec3(-6.0f, 3.0f);
	v4.m_position = Vec3(-3.0f, 6.0f);
	v5.m_position = Vec3(7.0f, 0.0f);
	ASTEROID_VERTICES_TEXAS.push_back(v0);
	ASTEROID_VERTICES_TEXAS.push_back(v1);
	ASTEROID_VERTICES_TEXAS.push_back(v2);
	ASTEROID_VERTICES_TEXAS.push_back(v3);
	ASTEROID_VERTICES_TEXAS.push_back(v4);
	ASTEROID_VERTICES_TEXAS.push_back(v5);

	v0.m_position = Vec3(-7.0f, -7.0f);
	v1.m_position = Vec3(-7.0f, 7.0f);
	v2.m_position = Vec3(7.0f, 7.0f);
	v3.m_position = Vec3(7.0f, -7.0f);
	ASTEROID_VERTICES_TREE.push_back(v0);
	ASTEROID_VERTICES_TREE.push_back(v1);
	ASTEROID_VERTICES_TREE.push_back(v2);
	ASTEROID_VERTICES_TREE.push_back(v3);


	ASTEROID_VERTICES[0] = ASTEROID_VERTICES_CROSS;
	ASTEROID_VERTICES[1] = ASTEROID_VERTICES_MUSHROOM;
	ASTEROID_VERTICES[2] = ASTEROID_VERTICES_TEXAS;
	ASTEROID_VERTICES[3] = ASTEROID_VERTICES_TREE;
}
