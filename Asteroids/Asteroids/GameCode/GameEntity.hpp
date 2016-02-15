//=====================================================
// GameEntity.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_GameEntity__
#define __included_GameEntity__

struct Vec2;
#include "Engine/Physics/Physics2D.hpp"
class OpenGLRenderer;
#include "Engine/Renderer/Mesh.hpp"
class Material;

class GameEntity{
protected:
	Physics2D m_physics;
	float m_radius;
	EngineAndrew::Mesh m_mesh;

public:
	GameEntity(const Vec2& position, const OpenGLRenderer& renderer, EngineAndrew::Material& material);

	inline const Vec2& GetPosition() const{return m_physics.m_position;}
	inline float GetRadius() const{return m_radius;}

	inline void SetPosition(const Vec2& position){m_physics.m_position = position;}

	virtual void Update(double deltaSeconds, const OpenGLRenderer& renderer);
	virtual void Draw(const EngineAndrew::Material& material, UniformMatrix* objectToWorld) const;
};

#endif