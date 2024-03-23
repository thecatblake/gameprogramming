#include "Game.h"

Game::Game() : mIsRunning(true), mWindow(nullptr), mRenderer(nullptr), mThickness(20)
{
    mBallVel.x = -200.0f;
    mBallVel.y = 235.0f;
}

bool Game::Initialize()
{
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);

    if (sdlResult != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    int width = 1024;
    int height = 768;

    mWindow = SDL_CreateWindow(
        "Game Programming in C++",
        100,
        100,
        width,
        height,
        0
    );

    if (!mWindow) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    mPaddlePos.x = 30;
    mPaddlePos.y = height / 2.0;

    mBallPos.x = width / 2.0;
    mBallPos.y = height / 2.0;

    return true;
}

void Game::RunLoop()
{
    while (mIsRunning) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::Shutdown()
{
    SDL_DestroyWindow(mWindow);
    SDL_DestroyRenderer(mRenderer);
    SDL_Quit();
}

void Game::ProcessInput()
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            mIsRunning = false;
            break;
        }
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);

    if (state[SDL_SCANCODE_ESCAPE]) {
        mIsRunning = false;
    }

    mPaddleDir = 0;
    if (state[SDL_SCANCODE_W]) {
        mPaddleDir = -1;
    }
    if (state[SDL_SCANCODE_S]) {
        mPaddleDir = 1;
    }
}

void Game::UpdateGame()
{
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    mTicksCount = SDL_GetTicks();

    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }

    mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
    mPaddlePos.y = fmin(mPaddlePos.y, 768 - mThickness - 60);
    mPaddlePos.y = fmax(mPaddlePos.y, mThickness);

    mBallPos.x += mBallVel.x * deltaTime;
    mBallPos.y += mBallVel.y * deltaTime;

    if (
        (mBallPos.y < mThickness && mBallVel.y < 0) 
        || (mBallPos.y > 768 - mThickness - mThickness && mBallVel.y > 0)
        
        ) {
        mBallVel.y *= -1;
    }
    if (

        (fabs(mPaddlePos.x - mBallPos.x) < mThickness && fabs(mPaddlePos.y - mBallPos.y) < 60.0f && mBallVel.x < 0)
        || (mBallPos.x > 1024 - mThickness - mThickness && mBallVel.x > 0)
        ) {
        mBallVel.x *= -1;
    }


}

void Game::GenerateOutput()
{
    SDL_SetRenderDrawColor(
        mRenderer,
        0,   // R
        0,   // G
        255, // B
        255  // A
    );

    SDL_RenderClear(mRenderer);

    SDL_SetRenderDrawColor(
        mRenderer,
        255,
        255,
        255,
        255
    );

    const int paddleHeight = 40;

    // Top wall
    SDL_Rect wall{
        0,
        0,
        1024,
        mThickness
    };
    SDL_RenderFillRect(mRenderer, &wall);

    // Right wall
    wall.w = mThickness;
    wall.h = 768;
    wall.x = 1024 - mThickness;
    wall.y = 0;
    SDL_RenderFillRect(mRenderer, &wall);

    // Bottom wall
    wall.w = 1024;
    wall.h = mThickness;
    wall.x = 0;
    wall.y = 768 - mThickness;
    SDL_RenderFillRect(mRenderer, &wall);

    SDL_Rect ball{
        static_cast<int>(mBallPos.x),
        static_cast<int>(mBallPos.y),
        mThickness,
        mThickness
    };
    SDL_RenderFillRect(mRenderer, &ball);

    const int paddleLength = 60;

    SDL_Rect paddle{
        static_cast<int>(mPaddlePos.x),
        static_cast<int>(mPaddlePos.y),
        mThickness,
        paddleLength
    };
    SDL_RenderFillRect(mRenderer, &paddle);

    SDL_RenderPresent(mRenderer);
}
