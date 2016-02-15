//=====================================================
// World.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_World__
#define __included_World__

#include "Engine/Math/Vec2.hpp"
#include "Asteroid.hpp"
class Ship;
#include "Bullet.hpp"
#include "Engine/Renderer/Material.hpp"

class World{
private:
	Vec2 m_displaySize;
	int m_stage;
	Asteroids m_asteroids;
	Ship* m_ship;
	Bullets m_bullets;
	OpenGLRenderer* m_renderer;
	EngineAndrew::Material m_material;
	UniformMatrix* m_objectToWorld;

	bool m_isRunning;

	void SpawnAsteroid();
	void SpawnShip();
	void SpawnBullet();
	void CreateStage();

	void DestroyAsteroid(Asteroids::iterator asteroidIndex);

	void CheckForGameEntityWrapping(GameEntity* gameEntity);
	void CheckForCollisions();

public:
	World(const Vec2& displaySize, OpenGLRenderer* renderer);
	~World();

	void Update(double deltaSeconds);
	void ProcessInput();

	void Draw() const;

	void ProcessXBoxController(float percentJoystickX, float percentJoystickY, unsigned short isAButtonDown);
	inline bool IsRunning() const { return m_isRunning; }
};

#endif