//=====================================================
// Bullet.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_Bullet__
#define __included_Bullet__

#include "GameEntity.hpp"
class OpenGLRenderer;
class Material;

class Bullet : public GameEntity{
private:
	double m_spawnTime;

public:
	Bullet(const Vec2& position, float shipOrientation, const OpenGLRenderer& renderer, EngineAndrew::Material& material);

	inline double GetSpawnTime() const{return m_spawnTime;}
};

typedef std::vector<Bullet*> Bullets;

#endif