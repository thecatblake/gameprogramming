#pragma once

#include <SDL.h>
#undef main

#include "Vector.h"

class Game {
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;
	Vector2 mPaddlePos;
	Vector2 mBallPos;
	Vector2 mBallVel;
	int mThickness;
	int mPaddleDir;
};