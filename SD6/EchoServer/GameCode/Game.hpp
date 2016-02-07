//=====================================================
// Game.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_World__
#define __included_World__

class OpenGLRenderer;
#include "Engine/Renderer/Material.hpp"
class NetworkSession;
#include "Engine/Networking/NetworkSystem.hpp"
struct ShortVec2;

class Game{
private:
	bool m_isRunning;

	EngineAndrew::Material m_material;
	int m_indexBufferID;
	int m_vaoID;
	int m_vertexbufferID;

	NetworkSession* m_gameSession;
	NetworkSystem m_netSystem;

public:
	Game();

	void StartHosting(unsigned short port);
	void StartHosting(const ShortVec2& ports);
	
	void Draw(OpenGLRenderer* renderer);
	void Update(OpenGLRenderer* renderer);

	void Startup(OpenGLRenderer* renderer);
	void Shutdown(const OpenGLRenderer* renderer);
	inline bool IsRunning() const{return m_isRunning;}
};

extern Game* s_theGame;

#endif