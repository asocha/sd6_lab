//=====================================================
// Game.cpp
// by Andrew Socha
//=====================================================

#include "Game.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "Engine/Networking/NetworkSystem.hpp"
#include "Engine/Networking/NetworkSession.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/Console/ConsoleCommands.hpp"
#include "Engine/Core/Utilities.hpp"
#include "Engine/Math/ShortVec2.hpp"

Game* s_theGame = nullptr;

///=====================================================
/// 
///=====================================================
Game::Game()
:m_isRunning(true),
m_material(),
m_vertexbufferID(0),
m_indexBufferID(0),
m_vaoID(0),
m_gameSession(nullptr),
m_netSystem(){
	FATAL_ASSERT(s_theGame == nullptr);
	s_theGame = this;
}

///=====================================================
/// 
///=====================================================
void Game::Startup(OpenGLRenderer* renderer){
	//Create HUD decoration
	bool wasCreated = m_material.CreateProgram(renderer, "Data/Shaders/basic2DNoTexture.vert", "Data/Shaders/basic2DNoTexture.frag");
	if (wasCreated == false){
		m_isRunning = false;
		return;
	}

	Vertex2D_PCT boxVerts[4] = {
		Vertex2D_PCT(Vec2(100.0f, 100.0f), Vec2(0.0f, 1.0f)),
		Vertex2D_PCT(Vec2(1500.0f, 100.0f), Vec2(1.0f, 1.0f)),
		Vertex2D_PCT(Vec2(100.0f, 800.0f), Vec2(0.0f, 0.0f)),
		Vertex2D_PCT(Vec2(1500.0f, 800.0f), Vec2(1.0f, 0.0f))
	};

	int boxIndeces[8] = {
		0, 1, 1, 3, 3, 2, 2, 0
	};

	renderer->GenerateBuffer((GLuint*)&m_vertexbufferID);
	renderer->SendVertexDataToBuffer(&boxVerts, sizeof(Vertex2D_PCT) * 4, m_vertexbufferID);

	renderer->GenerateBuffer((GLuint*)&m_indexBufferID);
	renderer->SendVertexDataToBuffer(&boxIndeces, sizeof(int) * 8, m_indexBufferID);

	m_vaoID = renderer->CreateVAOBasic();

	m_material.BindVertexData(m_vertexbufferID, Vertex2D_PCT());
	m_material.CreateSampler(renderer, GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT);
	m_material.SetBaseShape(GL_LINES);

	UniformMatrix* projection = (UniformMatrix*)m_material.CreateUniform("u_projection");
	FATAL_ASSERT(projection != nullptr);
	projection->m_data.push_back(renderer->CreateOrthographicMatrix());




	if (!m_netSystem.Init()) {
		return;
	}


	s_theConsole->SetMaxLines(20);
}

///=====================================================
/// 
///=====================================================
void Game::Shutdown(const OpenGLRenderer* renderer){
	m_isRunning = false;

	renderer->DeleteBuffer((GLuint*)&m_vertexbufferID);
	renderer->DeleteBuffer((GLuint*)&m_indexBufferID);
	renderer->DeleteVAO((GLuint*)&m_vaoID);

	
	delete m_gameSession;

	m_netSystem.Deinit();
}

///=====================================================
/// 
///=====================================================
void Game::Draw(OpenGLRenderer* /*renderer*/){
	m_material.Render(m_vaoID, m_indexBufferID, 8);
}

///=====================================================
/// 
///=====================================================
void Game::Update(OpenGLRenderer* /*renderer*/){
	if (s_theNetworkSession != nullptr) {
		s_theNetworkSession->Tick();
	}
}

///=====================================================
/// Create Session for UDP connections
///=====================================================
void Game::StartHosting(unsigned short port) {
	if (m_gameSession != nullptr) {
		FATAL_ERROR("Already hosting");
		s_theConsole->Printf("Already Hosting");
		return;
	}

	NetworkSession* session = new NetworkSession();
	
	if (session->Host(port)) {
		m_gameSession = session;
		m_gameSession->Listen(true);

		s_theConsole->Printf("Game Hosted: %s (%i)", m_gameSession->GetAddressString().c_str(), port);
	}
	else {
		delete session;
		FATAL_ERROR("Failed to host.");
	}
}

///=====================================================
/// Create Session for UDP connections
///=====================================================
void Game::StartHosting(const ShortVec2& ports) {
	if (m_gameSession != nullptr) {
		FATAL_ERROR("Already hosting");
		s_theConsole->Printf("Already Hosting");
		return;
	}

	NetworkSession* session = new NetworkSession();

	unsigned short port;
	if (session->Host(ports, port)) {
		m_gameSession = session;
		m_gameSession->Listen(true);
		
		s_theConsole->Printf("Game Hosted: %s (%i)", m_gameSession->GetAddressString().c_str(), port);
	}
	else {
		delete session;
		FATAL_ERROR("Failed to host.");
	}
}

///=====================================================
/// 
///=====================================================
CONSOLE_COMMAND(CreateSession) {
	if (args->m_args == nullptr) {
		return false;
	}

	short port;
	GetShort(args->m_args[1], port);

	if (args->m_args[0] == "1") {
		s_theGame->StartHosting(port);
	}
	else {
		short maxPort;
		GetShort(args->m_args[2], maxPort);
		s_theGame->StartHosting(ShortVec2(port, maxPort));
	}

	return true;
}