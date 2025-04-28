#include "Game.h"
#include <iostream>
#include <cmath>

Game::Game()
    : mWindow(sf::VideoMode(1280, 720), "Course de Bateaux !"),
      mState(GameState::Menu),
      mPlayerCount(1),
      mWinner(0),
      mElapsedTime(0.0f),
      mBoat1(nullptr),
      mBoat2(nullptr),
      mThrottle1(0.f), mSteering1(0.f),
      mThrottle2(0.f), mSteering2(0.f)
{
    if (!mFont.loadFromFile("CalSans-Regular.ttf")) {
        std::cerr << "Erreur chargement police CalSans-Regular.ttf" << std::endl;
    }

    mFinishLine.setSize(sf::Vector2f(10.f, 200.f));
    mFinishLine.setFillColor(sf::Color::Green);
    mFinishLine.setOrigin(5.f, 100.f);
    mFinishLine.setPosition(1180.f, 360.f);
}

void Game::run() {
    while (mWindow.isOpen()) {
        float dt = mClock.restart().asSeconds();
        processEvents();
        if (mState == GameState::Playing) {
            update(dt);
            mElapsedTime += dt;
        }
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            mWindow.close();

        if (mState == GameState::Menu) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1) {
                    startGame(1);
                } else if (event.key.code == sf::Keyboard::Num2) {
                    startGame(2);
                }
            }
        } else if (mState == GameState::GameOver) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                mState = GameState::Menu;
            }
        }
    }

    if (mState == GameState::Playing) {
        mThrottle1 = mSteering1 = mThrottle2 = mSteering2 = 0.0f;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) mThrottle1 = 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) mSteering1 = 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) mSteering1 = -1.0f;

        if (mPlayerCount == 2) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) mThrottle2 = 1.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) mSteering2 = 1.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) mSteering2 = -1.0f;
        }
    }
}

void Game::startGame(int playerCount) {
    mPlayerCount = playerCount;
    mState = GameState::Playing;
    mWinner = 0;
    mElapsedTime = 0.0f;

    mObstacles.clear();
    for (int i = 0; i < 10; ++i) {
        mObstacles.emplace_back();
    }

    // Créer bateau 1 (joueur rouge)
    mBoat1 = new Boat(mEngine, mModel, sf::Color::Red);
    mBoat1->setPosition(50.f, 360.f); // tout à gauche, milieu

    if (mPlayerCount == 2) {
        // Créer bateau 2 (joueur bleu)
        mBoat2 = new Boat(mEngine, mModel, sf::Color::Blue);
        mBoat2->setPosition(50.f, 420.f); // légèrement décalé en bas pour ne pas se chevaucher
    }
}


void Game::update(float dt) {
    if (mBoat1) mBoat1->update(mThrottle1, mSteering1, dt);
    if (mPlayerCount == 2 && mBoat2) mBoat2->update(mThrottle2, mSteering2, dt);

    // Vérifier collisions bateaux <-> obstacles

    auto checkCollision = [](Boat* boat, const std::vector<Obstacle>& obstacles) {
        sf::Vector2f boatPos = boat->getPosition();

        for (const auto& obstacle : obstacles) {
            sf::Vector2f obsPos = obstacle.getPosition();
            float radius = obstacle.getRadius(); // On doit ajouter cette méthode dans Obstacle.h
            float minDist = radius + 20.f; // 30 = moitié longueur du bateau

            float dx = boatPos.x - obsPos.x;
            float dy = boatPos.y - obsPos.y;
            float distance = std::sqrt(dx * dx + dy * dy);

            if (distance < minDist) {
                // Collision : repousser juste dehors
                if (distance != 0.f) {
                    sf::Vector2f repelDir(dx / distance, dy / distance);
                    boat->setPosition(obsPos.x + repelDir.x * minDist, obsPos.y + repelDir.y * minDist);
                }
            }
        }
    };

    // Appliquer sur chaque bateau existant
    if (mBoat1)
        checkCollision(mBoat1, mObstacles);

    if (mPlayerCount == 2 && mBoat2)
        checkCollision(mBoat2, mObstacles);


    mEngine.step(dt);

    if (mBoat1 && mFinishLine.getGlobalBounds().contains(mBoat1->getPosition())) {
        mWinner = 1;
        mState = GameState::GameOver;
        return;
    }

    if (mPlayerCount == 2 && mBoat2 && mFinishLine.getGlobalBounds().contains(mBoat2->getPosition())) {
        mWinner = 2;
        mState = GameState::GameOver;
        return;
    }
}

void Game::render() {
    mWindow.clear(sf::Color::Cyan);

    if (mState == GameState::Menu) {
        mMenu.draw(mWindow, mFont);
    } else if (mState == GameState::Playing) {
        mWindow.draw(mFinishLine);

        for (const auto& obstacle : mObstacles)
            obstacle.draw(mWindow);

        if (mBoat1) mBoat1->draw(mWindow);
        if (mPlayerCount == 2 && mBoat2) mBoat2->draw(mWindow);
    } else if (mState == GameState::GameOver) {
        sf::Text text;
        text.setFont(mFont);
        text.setCharacterSize(30);
        text.setString("Joueur " + std::to_string(mWinner) + " gagne !\nAppuyez sur ENTREE pour revenir au menu.");
        text.setPosition(200.f, 300.f);
        text.setFillColor(sf::Color::Black);
        mWindow.draw(text);
    }

    // Afficher le chronomètre en haut
sf::Text chrono;
chrono.setFont(mFont);
chrono.setCharacterSize(24);
chrono.setFillColor(sf::Color::Black);
chrono.setPosition(20.f, 20.f);

// Mettre à jour le texte du chrono
chrono.setString("Temps : " + std::to_string(mElapsedTime).substr(0,5) + "s");

mWindow.draw(chrono);


    mWindow.display();
}
