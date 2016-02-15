//=====================================================
// World.cpp
// by Andrew Socha
//=====================================================

#include "Engine/Core/EngineCore.hpp"
#include "World.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Math/Disc2D.hpp"
#include "Engine/Math/Math2D.hpp"
#include "Engine/Time/Time.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Ship.hpp"

///=====================================================
/// 
///=====================================================
World::World(const Vec2& displaySize, OpenGLRenderer* renderer) :
m_isRunning(true),
m_displaySize(displaySize),
m_stage(6),
m_ship(nullptr),
m_bullets(),
m_asteroids(),
m_renderer(renderer),
m_material(),
m_objectToWorld(nullptr){
	FATAL_ASSERT(m_renderer != nullptr);
	m_material.CreateProgram(renderer, "Data/Shaders/basicAnim.vert", "Data/Shaders/basicAnim.frag");
	m_material.CreateSampler(renderer);

	m_material.SetBaseShape(GL_LINE_LOOP);

	UniformMatrix* projection = (UniformMatrix*)m_material.CreateUniform("u_cameraToClip");
	FATAL_ASSERT(projection != nullptr);
	projection->m_data.push_back(renderer->CreateOrthographicMatrix());

	m_objectToWorld = (UniformMatrix*)m_material.CreateUniform("u_objectToWorld");
	FATAL_ASSERT(m_objectToWorld != nullptr);
	m_objectToWorld->m_data.push_back(Matrix4());

	UniformMatrix* worldToCamera = (UniformMatrix*)m_material.CreateUniform("u_worldToCamera");
	FATAL_ASSERT(worldToCamera != nullptr);
	worldToCamera->m_data.push_back(Matrix4());

	SpawnShip();
	CreateStage();
}

///=====================================================
/// 
///=====================================================
void World::SpawnAsteroid(){
	Vec2 position;
	float asteroidRadius = Asteroid::BASE_ASTEROID_RADIUS * Asteroid::ASTEROID_SIZE_LARGE;
	if (GetRandomIntLessThan(3)){ // 66% chance for bottom/top, 33% for left/right
		position = Vec2(GetRandomFloatInRange(0.0f,m_displaySize.x), -asteroidRadius);
	}
	else{
		position = Vec2(-asteroidRadius, GetRandomFloatInRange(0.0f,m_displaySize.y));
	}

	FATAL_ASSERT(m_renderer != nullptr);
	Asteroid* asteroid = new Asteroid(position, Asteroid::ASTEROID_SIZE_LARGE, *m_renderer, m_material);

	m_asteroids.push_back(asteroid);
}

///=====================================================
/// 
///=====================================================
void World::SpawnShip(){
	Vec2 position(m_displaySize.x*0.5f, m_displaySize.y*0.5f);

	FATAL_ASSERT(m_renderer != nullptr);
	m_ship = new Ship(position, *m_renderer, m_material);
}

///=====================================================
/// 
///=====================================================
void World::SpawnBullet(){
	if (!m_ship) return;

	FATAL_ASSERT(m_renderer != nullptr);
	Bullet* bullet = m_ship->SpawnBullet(*m_renderer, m_material);

	m_bullets.push_back(bullet);
}



///=====================================================
/// 
///=====================================================
void World::Draw() const{
	for (Asteroids::const_iterator asteroidIter = m_asteroids.begin(); asteroidIter != m_asteroids.end(); ++asteroidIter){
		Asteroid* asteroid = *asteroidIter;
		asteroid->Draw(m_material, m_objectToWorld);
	}
	if (m_ship && !m_ship->IsDestroyed()) m_ship->Draw(m_material, m_objectToWorld);
	for (Bullets::const_iterator bulletIter = m_bullets.begin(); bulletIter != m_bullets.end(); ++bulletIter){
		Bullet* bullet = *bulletIter;
		bullet->Draw(m_material, m_objectToWorld);
	}
}

///=====================================================
/// 
///=====================================================
World::~World(){
	for (Asteroids::const_iterator asteroidIter = m_asteroids.begin(); asteroidIter != m_asteroids.end(); ++asteroidIter){
		delete *asteroidIter;
	}
	if (m_ship) delete m_ship;
	for (Bullets::const_iterator bulletIter = m_bullets.begin(); bulletIter != m_bullets.end(); ++bulletIter){
		delete *bulletIter;
	}
}

///=====================================================
/// 
///=====================================================
void World::Update(double deltaSeconds){
	FATAL_ASSERT(m_renderer != nullptr);
	for (Asteroids::iterator asteroidIter = m_asteroids.begin(); asteroidIter != m_asteroids.end(); ++asteroidIter){
		Asteroid* asteroid = *asteroidIter;
		asteroid->Update(deltaSeconds, *m_renderer);
		CheckForGameEntityWrapping(asteroid);
	}

	if (m_ship && !m_ship->IsDestroyed()){
		m_ship->Update(deltaSeconds, *m_renderer);
		CheckForGameEntityWrapping(m_ship);
	}

	double currentTime = GetCurrentSeconds();
	double minimumSpawnTime = currentTime - 2.0;
	for (Bullets::iterator bulletIter = m_bullets.begin(); bulletIter != m_bullets.end();){
		Bullet* bullet = *bulletIter;
		if (bullet->GetSpawnTime() < minimumSpawnTime){
			delete bullet;
			bulletIter = m_bullets.erase(bulletIter);
		}
		else{
			bullet->Update(deltaSeconds, *m_renderer);
			CheckForGameEntityWrapping(bullet);
			++bulletIter;
		}
	}

	CheckForCollisions();

	if (m_asteroids.empty()){
		m_stage += 3;
		CreateStage();
	}
}

///=====================================================
/// 
///=====================================================
void World::ProcessInput() {
	if (s_theInputSystem->IsKeyDown('A') || s_theInputSystem->IsKeyDown(VK_LEFT)) {
		if (m_ship) m_ship->RotateCounterClockwise();
	}
	else if (s_theInputSystem->IsKeyDown('D') || s_theInputSystem->IsKeyDown(VK_RIGHT)) {
		if (m_ship) m_ship->RotateClockwise();
	}
	else if (s_theInputSystem->IsKeyDown('W') || s_theInputSystem->IsKeyDown(VK_UP)) {
		if (m_ship) m_ship->SetThrust(1.0f);
	}

	else if (s_theInputSystem->IsKeyDown(VK_SPACE)) {
		if (m_ship && !m_ship->IsDestroyed()) SpawnBullet();
	}

	else if (s_theInputSystem->GetKeyWentDown('P')) {
		if (m_ship && m_ship->IsDestroyed()) m_ship->Respawn(Vec2(m_displaySize.x*0.5f, m_displaySize.y*0.5f));
	}
	else if (s_theInputSystem->GetKeyWentDown('O')) {
		SpawnAsteroid();
	}
	else if (s_theInputSystem->GetKeyWentDown('L')) {
		DestroyAsteroid(m_asteroids.end() - 1);
	}
}

///=====================================================
/// 
///=====================================================
void World::CheckForGameEntityWrapping(GameEntity* gameEntity){
	if (!gameEntity) return;

	Vec2 gameEntityPosition = gameEntity->GetPosition();
	float radius = gameEntity->GetRadius();

	if (gameEntityPosition.x + radius < 0.0f){
		gameEntityPosition.x = m_displaySize.x + radius;
	}
	else if (gameEntityPosition.x - radius > m_displaySize.x){
		gameEntityPosition.x = -radius;
	}

	if (gameEntityPosition.y + radius < 0.0f){
		gameEntityPosition.y = m_displaySize.y + radius;
	}
	else if (gameEntityPosition.y - radius > m_displaySize.y){
		gameEntityPosition.y = -radius;
	}

	gameEntity->SetPosition(gameEntityPosition);
}

///=====================================================
/// 
///=====================================================
void World::CreateStage(){
	for (int i = 0; i < m_stage; i++){
		SpawnAsteroid();
	}
}

///=====================================================
/// 
///=====================================================
void World::DestroyAsteroid(Asteroids::iterator asteroidIndex){
	Asteroid* asteroid = *asteroidIndex;
	delete asteroid;
	m_asteroids.erase(asteroidIndex);
}

///=====================================================
/// 
///=====================================================
void World::CheckForCollisions(){
	FATAL_ASSERT(m_renderer != nullptr);
	Asteroids asteroidsToAdd;
	for (Asteroids::iterator asteroidIter = m_asteroids.begin(); asteroidIter != m_asteroids.end();){
		Asteroid* asteroid = *asteroidIter;
		if (!asteroid) break;

		for (Bullets::iterator bulletIter = m_bullets.begin(); bulletIter != m_bullets.end();){
			Bullet* bullet = *bulletIter;
			if (!bullet) break;

			Disc2D asteroidDisc(asteroid->GetPosition(), asteroid->GetRadius());
			Disc2D bulletDisc(bullet->GetPosition(), bullet->GetRadius());

			if (DoDiscsOverlap(asteroidDisc, bulletDisc)){
				delete bullet;
				bulletIter = m_bullets.erase(bulletIter);

				int shrunkSize = asteroid->GetSize() - 1;
				if (shrunkSize > 0){
  					Asteroid* newAsteroid = new Asteroid(asteroid->GetPosition(), (Asteroid::AsteroidSize)shrunkSize, *m_renderer, m_material);
					asteroidsToAdd.push_back(newAsteroid);

					Vec2 oldVelocity = asteroid->GetVelocity();
					*asteroid = Asteroid(asteroid->GetPosition(), (Asteroid::AsteroidSize)shrunkSize, *m_renderer, m_material);

					newAsteroid->SetVelocity(oldVelocity + asteroid->GetVelocity());
					asteroid->SetVelocity(oldVelocity - asteroid->GetVelocity());
				}
				else{
					delete asteroid;
					asteroidIter = m_asteroids.erase(asteroidIter);
					goto nextAsteroid;
				}
			}
			else{
				++bulletIter;
			}
		}
			
		if (m_ship && !m_ship->IsDestroyed()){
			Disc2D asteroidDisc(asteroid->GetPosition(), asteroid->GetRadius());
			Disc2D shipDisc(m_ship->GetPosition(), m_ship->GetRadius());

			if (DoDiscsOverlap(asteroidDisc, shipDisc)){
				int shrunkSize = asteroid->GetSize() - 1;
				if (shrunkSize > 0){
					Asteroid* newAsteroid = new Asteroid(asteroid->GetPosition(), (Asteroid::AsteroidSize)shrunkSize, *m_renderer, m_material);
					asteroidsToAdd.push_back(newAsteroid);

					Vec2 oldVelocity = asteroid->GetVelocity();
					*asteroid = Asteroid(asteroid->GetPosition(), (Asteroid::AsteroidSize)shrunkSize, *m_renderer, m_material);

					newAsteroid->SetVelocity(oldVelocity + asteroid->GetVelocity());
					asteroid->SetVelocity(oldVelocity - asteroid->GetVelocity());
				}
				else{
					asteroidIter = m_asteroids.erase(asteroidIter);
					delete asteroid;
				}

				m_ship->Destroy();
			}
			else {
				++asteroidIter;
			}
		}
		else {
			++asteroidIter;
		}
nextAsteroid:;
	}

	for (Asteroids::iterator asteroidIter = asteroidsToAdd.begin(); asteroidIter != asteroidsToAdd.end(); ++asteroidIter){
		m_asteroids.push_back(*asteroidIter);
	}
}

///=====================================================
/// 
///=====================================================
void World::ProcessXBoxController(float percentJoystickX, float percentJoystickY, unsigned short isAButtonDown){
	if (m_ship && !m_ship->IsDestroyed()){
		if (percentJoystickX != 0.0f || percentJoystickY != 0.0f){
			Vec2 heading(percentJoystickX, percentJoystickY);
			m_ship->SetOrientationDegrees(heading.CalcHeadingDegrees());

			m_ship->SetThrust(heading.CalcLength());
		}
		if (isAButtonDown) SpawnBullet();
	}
}
