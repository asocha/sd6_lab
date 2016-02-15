//=====================================================
// Asteroid.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_Asteroid__
#define __included_Asteroid__

#include "GameEntity.hpp"
class OpenGLRenderer;

class Asteroid : public GameEntity{
public:
	const static float BASE_ASTEROID_RADIUS;

	enum AsteroidSize{
		ASTEROID_SIZE_SMALL = 1,
		ASTEROID_SIZE_MEDIUM = 2,
		ASTEROID_SIZE_LARGE = 3
	};
	enum AsteroidShape{
		ASTEROID_SHAPE_CROSS,
		ASTEROID_SHAPE_MUSHROOM,
		ASTEROID_SHAPE_TREE,
		ASTEROID_SHAPE_TEXAS
	};

private:
	AsteroidShape m_shape;
	AsteroidSize m_size;

	static std::vector<Vertex_Anim> ASTEROID_VERTICES_CROSS;
	static std::vector<Vertex_Anim> ASTEROID_VERTICES_MUSHROOM;
	static std::vector<Vertex_Anim> ASTEROID_VERTICES_TREE;
	static std::vector<Vertex_Anim> ASTEROID_VERTICES_TEXAS;
	static std::vector<Vertex_Anim> ASTEROID_VERTICES[4];

	void CreateVerticesBasedOnShape() const;
	
public:
	Asteroid(const Vec2& position, AsteroidSize asteroidSize, const OpenGLRenderer& renderer, EngineAndrew::Material& material);

	inline AsteroidSize GetSize() const { return m_size; }
	inline const Vec2 GetVelocity() const{return m_physics.m_velocity;}
	inline void SetVelocity(const Vec2& velocity){m_physics.m_velocity = velocity;}

	void Draw(const EngineAndrew::Material& material, UniformMatrix* objectToWorld) const;
};

typedef std::vector<Asteroid*> Asteroids;

#endif