//=====================================================
// GameEntity.cpp
// by Andrew Socha
//=====================================================

#include "GameEntity.hpp"
#include "Engine\Renderer\Material.hpp"


///=====================================================
/// 
///=====================================================
GameEntity::GameEntity(const Vec2& position, const OpenGLRenderer& renderer, EngineAndrew::Material& material) :
m_physics(),
m_radius(0.0f){
	m_physics.m_position = position;

	m_mesh.Startup(&renderer);
	material.BindVertexData(m_mesh);
}

///=====================================================
/// 
///=====================================================
void GameEntity::Update(double deltaSeconds, const OpenGLRenderer& /*renderer*/){
	m_physics.Update((float)deltaSeconds);
}

///=====================================================
/// 
///=====================================================
void GameEntity::Draw(const EngineAndrew::Material& material, UniformMatrix* objectToWorld) const {
	FATAL_ASSERT(objectToWorld != nullptr);
	Matrix4 modelMatrix = Matrix4::CreateRotationDegreesAboutZ(m_physics.m_orientationDegrees);
	modelMatrix.Translate(m_physics.m_position);
	objectToWorld->m_data[0] = modelMatrix;

	material.Render(m_mesh);
}
