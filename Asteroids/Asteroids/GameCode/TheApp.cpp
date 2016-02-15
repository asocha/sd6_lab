//=====================================================
// TheApp.cpp
// by Andrew Socha
//=====================================================

#include "Engine/Core/EngineCore.hpp"
#include "TheApp.hpp"
#include "Engine/Time/Time.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Sound/SoundSystem.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/Time/Clock.hpp"
#include "Engine/Core/ProfileSection.hpp"
#include "Engine/Console/ConsoleCommands.hpp"
#include "Engine/Renderer/OpenGLRenderer.hpp"
#include "World.hpp"
#include "Engine/Core/SignpostMemoryManager.hpp"
#include <Xinput.h>


///=====================================================
/// 
///=====================================================
TheApp::TheApp(){
	m_isRunning = true;
	m_world = 0;
}

///=====================================================
/// 
///=====================================================
TheApp::~TheApp(){

}

///=====================================================
/// 
///=====================================================
void TheApp::Startup(void* windowHandle){
	m_windowHandle = windowHandle;

	InitializeTimer();

	m_masterClock = new Clock(nullptr);
	RECOVERABLE_ASSERT(m_masterClock != nullptr);

	m_inputSystem = new InputSystem();
	RECOVERABLE_ASSERT(m_inputSystem != nullptr);
	if (m_inputSystem) {
		m_inputSystem->Startup(windowHandle);
		m_inputSystem->ShowMouse(false);
	}

	m_soundSystem = new SoundSystem();
	RECOVERABLE_ASSERT(m_soundSystem != nullptr);
	if (m_soundSystem)
		m_soundSystem->Startup();

	m_renderer = new OpenGLRenderer();
	RECOVERABLE_ASSERT(m_renderer != nullptr);
	if (m_renderer) {
		m_renderer->Startup((HWND)windowHandle);
		m_renderer->CheckGLCompatibility();
		m_renderer->SetAlphaTest(true);
		m_renderer->IgnoreEmptyPixels();

		m_console = new Console();
		RECOVERABLE_ASSERT(m_console != nullptr);
		if (m_console != nullptr) {
			m_console->Startup(m_renderer);
			m_console->m_isVisible = false;
		}

		ProfileSection::StartupProfiling(m_renderer);

		Vec2 displaySize(m_renderer->GetDisplayWidth(), m_renderer->GetDisplayHeight());
		m_world = new World(displaySize, m_renderer);
		RECOVERABLE_ASSERT(m_world != nullptr);
		if (m_world == nullptr) {
			m_isRunning = false;
		}
	}
	else {
		m_isRunning = false;
	}
}

///=====================================================
/// 
///=====================================================
void TheApp::Run(){
	while(m_isRunning){
		ProcessInput();
		UpdateWorld();
		RenderWorld();
	}
}

///=====================================================
/// 
///=====================================================
void TheApp::Shutdown(){
	if (m_masterClock)
		delete m_masterClock;

	if (m_world) {
		delete m_world;
	}

	if (m_console) {
		m_console->Shutdown(m_renderer);
		delete m_console;
	}

	ProfileSection::ShutdownProfiling(m_renderer);

	if (m_renderer) {
		m_renderer->Shutdown();
		delete m_renderer;
	}

	if (m_inputSystem) {
		m_inputSystem->Shutdown();
		delete m_inputSystem;
	}

	if (m_soundSystem) {
		m_soundSystem->Shutdown();
		delete m_soundSystem;
	}

	if (s_theCommandList)
		delete s_theCommandList;
}

///=====================================================
/// 
///=====================================================
void TheApp::ProcessInput(){
	if (m_inputSystem) {
		m_inputSystem->Update();

		if ((m_inputSystem->IsKeyDown(VK_ESCAPE) && !m_console->m_isVisible) || m_inputSystem->IsReadyToQuit()) {
			m_isRunning = false;
		}
		else if (m_inputSystem->GetKeyWentDown(192)) { // ` key
			m_console->m_isVisible = !m_console->m_isVisible;

			if (m_console->m_isVisible) {
				m_console->CreateInputString();
			}
			else {
				m_console->DeleteInputString();
			}
		}
		else if (m_console->m_isVisible) {
			if (m_inputSystem->GetKeyWentDown(VK_UP)) {
				m_console->ScrollUpTextHistory();
			}
			else if (m_inputSystem->GetKeyWentDown(VK_DOWN)) {
				m_console->ScrollDownTextHistory();
			}
			else {
				int currentCharacterDown = m_inputSystem->GetCurrentCharacterDown();
				if (currentCharacterDown == VK_BACK) { //Backspace
					m_console->DeleteFromInputString();
				}
				else if (currentCharacterDown == VK_ESCAPE) { //Escape
					bool didDelete = m_console->ClearInputString();
					m_console->m_isVisible = didDelete;
				}
				else if (currentCharacterDown == VK_RETURN) { //Enter
					if (m_console->HasInputString()) {
						m_console->ExecuteInputString();
					}
					else {
						m_console->m_isVisible = false;
					}
				}
				else if (currentCharacterDown != -1) { //any other key
					m_console->AppendToInputString((unsigned char)currentCharacterDown);
				}
			}
			m_inputSystem->ResetKeys(); //prevent normal input while console is open
		}
	}

	HWND myWindowHandle = (HWND)m_windowHandle;
	MSG messageHandler;

	if (PeekMessage(&messageHandler, myWindowHandle, 0, 0, false)){
		BOOL messageWasFound = GetMessage(&messageHandler, myWindowHandle, 0, 0);
		if (messageWasFound){
			TranslateMessage(&messageHandler);
			DispatchMessage(&messageHandler);
		}
		else{ //quit message was sent
			m_isRunning = false;
		}
	}
	if (m_world){
		m_world->ProcessInput();

		const int CONTROLLER_NUMBER = 0;
		XINPUT_STATE xboxControllerState;
		memset(&xboxControllerState, 0, sizeof(xboxControllerState));
		DWORD errorStatus = XInputGetState(CONTROLLER_NUMBER, &xboxControllerState);
		if(errorStatus == ERROR_SUCCESS){
			short joystickX = xboxControllerState.Gamepad.sThumbLX;
			short joystickY = xboxControllerState.Gamepad.sThumbLY;
			const float inverseMaximumJoystick = 1.0f/32768.0f;
			float percentJoystickX = (float)joystickX * inverseMaximumJoystick;
			float percentJoystickY = (float)joystickY * inverseMaximumJoystick;
			if (percentJoystickX < 0.2f && percentJoystickX > -0.2f && percentJoystickY < 0.2f && percentJoystickY > -0.2f){
				percentJoystickX = 0.0f;
				percentJoystickY = 0.0f;
			}

			const unsigned short BIT_A_BUTTON = 0x1000;
			unsigned short isAButtonDown = BIT_A_BUTTON & xboxControllerState.Gamepad.wButtons;

			m_world->ProcessXBoxController(percentJoystickX, percentJoystickY, isAButtonDown);
		}
		else if(errorStatus == ERROR_DEVICE_NOT_CONNECTED){
			//ConsolePrintf( "Xbox controller is not connected.\n" );
		}
		else{
			ConsolePrintf( "Xbox controller reports unknown error status code %u (0x%08x).\n", errorStatus, errorStatus );
		}
	}

}

///=====================================================
/// 
///=====================================================
void TheApp::UpdateWorld(){
	double currentTime = GetCurrentSeconds();
	static double lastTime = currentTime;
	float deltaSeconds = (float)(currentTime - lastTime);

	if (deltaSeconds > 0.5f)
		deltaSeconds = 0.5f;

	lastTime = currentTime;

	m_masterClock->AdvanceTime(deltaSeconds);

	if (m_soundSystem) {
		m_soundSystem->Update();
	}

	if (m_world) {
		m_world->Update(deltaSeconds);

		if (!m_world->IsRunning())
			m_isRunning = false;
	}
}

///=====================================================
/// 
///=====================================================
void TheApp::RenderWorld() const{
	m_renderer->ClearBuffer();

	if (m_world)
		m_world->Draw();

	m_console->RenderText(m_renderer, "Data/Fonts/Arial", 32.0f, Vec2(10.0f, 10.0f));

	ProfileSection::Render(m_renderer);
	ProfileSection::FinishProfilingFrame();

	m_renderer->SwapBuffers();

	if (s_theMemoryManager)
		s_theMemoryManager->FinishFrame();
}
