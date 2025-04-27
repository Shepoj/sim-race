#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Boat.h"
#include "Obstacle.h"
#include "Menu.h"
#include <physics/physicsEngine.h>
#include <physics/dynamicModel.h>

enum class GameState {
    Menu,
    Playing,
    GameOver
};

class Game {
public:
    Game();
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();
    void startGame(int playerCount);

private:
    sf::RenderWindow mWindow;
    sf::Clock mClock;
    sf::Font mFont;

    GameState mState;
    int mPlayerCount;
    int mWinner;
    float mElapsedTime;

    Menu mMenu;
    sf::RectangleShape mFinishLine;
    std::vector<Obstacle> mObstacles;

    sim::physics::PhysicsEngine mEngine;
    sim::physics::DynamicModel mModel;
    Boat* mBoat1;
    Boat* mBoat2;

    float mThrottle1, mSteering1;
    float mThrottle2, mSteering2;
};
