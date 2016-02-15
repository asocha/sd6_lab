//=====================================================
// TheApp.hpp
// by Andrew Socha
//=====================================================

#pragma once

#ifndef __included_TheApp__
#define __included_TheApp__

class OpenGLRenderer;
class World;
class InputSystem;
class SoundSystem;
class Clock;
class Console;

class TheApp{
public:
	TheApp();
	~TheApp();

	void Startup(void* windowHandle);
	void Shutdown();
	void Run();

	void ProcessInput();

	void UpdateWorld();
	void RenderWorld() const;

private:
	void* m_windowHandle;
	OpenGLRenderer* m_renderer;
	InputSystem* m_inputSystem;
	SoundSystem* m_soundSystem;
	bool m_isRunning;
	World* m_world;
	Console* m_console;
	Clock* m_masterClock;
};

#endif