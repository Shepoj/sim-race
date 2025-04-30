#include "Game.h"
#include <iostream>
#include <cmath>
#include <algorithm>

Game::Game()
    : mWindow(sf::VideoMode(1280, 720), "Course de Bateaux !"),
      mState(GameState::Menu),
      mPlayerCount(1),
      mWinner(0),
      mElapsedTime(0.0f),
      mBoat1(nullptr),
      mBoat2(nullptr),
      mThrottle1(0.f), mSteering1(0.f),
      mThrottle2(0.f), mSteering2(0.f),
      mGameTime(0.f), mMaxGameTime(10.f)
{
    if (!mFont.loadFromFile("CalSans-Regular.ttf")) {
        std::cerr << "Erreur chargement police CalSans-Regular.ttf" << std::endl;
    }
    if (!mFishTexture.loadFromFile("fish.png")) {
        std::cerr << "Erreur chargement fish.png" << std::endl;
    }

    mTimerText.setFont(mFont);
    mTimerText.setCharacterSize(28);
    mTimerText.setFillColor(sf::Color::Black);
    mTimerText.setPosition(560.f, 20.f);

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
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) mThrottle2 = 1.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) mSteering2 = 1.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) mSteering2 = -1.0f;
        }
    }
}

void Game::startGame(int playerCount) {
    mPlayerCount = playerCount;
    mState = GameState::Playing;
    mWinner = 0;
    mElapsedTime = 0.0f;
    mGameTime = 0.0f;
    mBoatsWereTouching = false;

    mObstacles.clear();
    for (int i = 0; i < 10; ++i) {
        mObstacles.emplace_back();
    }

    mBoat1 = new Boat(mEngine, mModel, sf::Color(60, 180, 75));
    mBoat1->setPosition(50.f, 360.f);
    mFishCarried1 = 0;

    if (mPlayerCount == 2) {
        mBoat2 = new Boat(mEngine, mModel, sf::Color(200, 60, 60));
        mBoat2->setPosition(1230.f, 360.f);
        mBoat2->setRotation(3.14159f); // vers la gauche
        mFishCarried2 = 0;
    }

    mPort1 = new Port(sf::Vector2f(50.f, 360.f), sf::Color(60, 180, 75), mFont);
    if (mPlayerCount == 2)
        mPort2 = new Port(sf::Vector2f(1230.f, 360.f), sf::Color(200, 60, 60), mFont);

    mFishSpawnTimer = 0.f;
    mFishSpawnInterval = 2.0f; // toutes les 2 secondes on peut essayer de faire apparaître un poisson
    mMaxFishOnScreen = 2;      // max 2 poissons visibles à la fois

    mFishes.clear(); // ← ne plus créer 10 dès le début

}

void Game::update(float dt) {
    if (mBoat1) mBoat1->update(mThrottle1, mSteering1, dt);
    if (mPlayerCount == 2 && mBoat2) mBoat2->update(mThrottle2, mSteering2, dt);
    if (mBoat1) mBoat1->clampToWindow(1280.f, 720.f);
    if (mPlayerCount == 2 && mBoat2) mBoat2->clampToWindow(1280.f, 720.f);


    auto checkCollision = [](Boat* boat, const std::vector<Obstacle>& obstacles) {
        sf::Vector2f boatPos = boat->getPosition();
        for (const auto& obstacle : obstacles) {
            sf::Vector2f obsPos = obstacle.getPosition();
            float radius = obstacle.getRadius();
            float minDist = radius + 20.f;
            float dx = boatPos.x - obsPos.x;
            float dy = boatPos.y - obsPos.y;
            float distance = std::sqrt(dx * dx + dy * dy);
            if (distance < minDist && distance != 0.f) {
                sf::Vector2f repelDir(dx / distance, dy / distance);
                boat->setPosition(obsPos.x + repelDir.x * minDist, obsPos.y + repelDir.y * minDist);
            }
        }
    };

    if (mBoat1) checkCollision(mBoat1, mObstacles);
    if (mPlayerCount == 2 && mBoat2) checkCollision(mBoat2, mObstacles);

    mEngine.step(dt);

    mGameTime += dt;
    float remaining = std::max(0.f, mMaxGameTime - mGameTime);
    mTimerText.setString("Temps : " + std::to_string(static_cast<int>(remaining)) + "s");

    // Ramasser poissons
    auto tryCollect = [](Boat* boat, std::vector<Fish>& fishes, int& count) {
        sf::Vector2f pos = boat->getPosition();
        for (auto& fish : fishes) {
            if (!fish.isCollected()) {
                sf::Vector2f fpos = fish.getPosition();
                float dist = std::hypot(pos.x - fpos.x, pos.y - fpos.y);
                if (dist < 30.f) {
                    fish.markCollected();
                    ++count;
                }
            }
        }
    };
    tryCollect(mBoat1, mFishes, mFishCarried1);
    if (mPlayerCount == 2) tryCollect(mBoat2, mFishes, mFishCarried2);

    // Dépôt au port
    if (mPort1->getBounds().contains(mBoat1->getPosition())) {
        for (int i = 0; i < mFishCarried1; ++i) mPort1->depositFish();
        mFishCarried1 = 0;
    }
    if (mPlayerCount == 2 && mPort2->getBounds().contains(mBoat2->getPosition())) {
        for (int i = 0; i < mFishCarried2; ++i) mPort2->depositFish();
        mFishCarried2 = 0;
    }

    // Échange en cas de collision
    if (mPlayerCount == 2) {
        float dx = mBoat1->getPosition().x - mBoat2->getPosition().x;
        float dy = mBoat1->getPosition().y - mBoat2->getPosition().y;
        float dist = std::sqrt(dx * dx + dy * dy);
        bool boatsTouchingNow = (dist < 40.f);

        if (boatsTouchingNow && !mBoatsWereTouching) {
            int before1 = mFishCarried1;
            int before2 = mFishCarried2;
            std::swap(mFishCarried1, mFishCarried2);

            if (mFishCarried1 > before1) {
                mBoat1->flash(sf::Color(255, 215, 0));
            }
            if (mFishCarried2 > before2) {
                mBoat2->flash(sf::Color(255, 215, 0));
            }
        }
        mBoatsWereTouching = boatsTouchingNow;
    }


    if (mGameTime >= mMaxGameTime) {
        mState = GameState::GameOver;
    }

    // === GÉNÉRATION DYNAMIQUE DE POISSONS ===
    mFishSpawnTimer += dt;

    // Compter combien de poissons ne sont pas encore ramassés
    int activeFish = std::count_if(mFishes.begin(), mFishes.end(), [](const Fish& f) {
        return !f.isCollected();
    });

    if (mFishSpawnTimer >= mFishSpawnInterval && activeFish < mMaxFishOnScreen) {
        std::vector<sf::FloatRect> avoidZones;
        avoidZones.push_back(mFinishLine.getGlobalBounds());
        for (const auto& obs : mObstacles) avoidZones.push_back(obs.getGlobalBounds());
        avoidZones.push_back(mPort1->getBounds());
        if (mPlayerCount == 2) avoidZones.push_back(mPort2->getBounds());

        mFishes.emplace_back(mFishTexture, avoidZones);
        mFishSpawnTimer = 0.f;
    }

}

void Game::render() {
    mWindow.clear(sf::Color(40, 120, 200)); // Bleu acier


    if (mState == GameState::Menu) {
        mMenu.draw(mWindow, mFont);
    } else if (mState == GameState::Playing || mState == GameState::GameOver) {
        for (const auto& obstacle : mObstacles) obstacle.draw(mWindow);
        for (const auto& fish : mFishes) fish.draw(mWindow);
        mPort1->draw(mWindow);
        if (mPlayerCount == 2) mPort2->draw(mWindow);
        if (mBoat1) {
            mBoat1->setFishCount(mFishCarried1);
            mBoat1->draw(mWindow, mFont);

        }
        if (mPlayerCount == 2 && mBoat2) {
            mBoat2->setFishCount(mFishCarried2);
            mBoat2->draw(mWindow, mFont);

        }
        mWindow.draw(mTimerText);

        if (mState == GameState::GameOver) {
            sf::Text title;
            title.setFont(mFont);
            title.setCharacterSize(40);
            title.setFillColor(sf::Color::Black);
            title.setString("FIN DE LA PARTIE");

            sf::FloatRect titleBounds = title.getLocalBounds();
            title.setOrigin(titleBounds.width / 2.f, titleBounds.height / 2.f);
            title.setPosition(640.f, 200.f); // centré en haut
            mWindow.draw(title);

            sf::Text text;
            text.setFont(mFont);
            text.setCharacterSize(30);
            text.setFillColor(sf::Color::Black);
            if (mPlayerCount == 1) {
                text.setString("Poissons attrapes : " + std::to_string(mPort1->getScore()));
            } else {
                int s1 = mPort1->getScore();
                int s2 = mPort2->getScore();
                std::string winner = (s1 > s2) ? "      Vert gagne !" : (s1 < s2) ? "     Rouge gagne !" : "            Egalite !";
                text.setString("Vert : " + std::to_string(s1) + "  -   Rouge : " + std::to_string(s2) + "\n" + winner);
            }
            sf::FloatRect bounds = text.getLocalBounds();
            text.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
            text.setPosition(640.f, 300.f); // centré en X (fenêtre 1280x720)

            mWindow.draw(text);
            sf::Text prompt;
            prompt.setFont(mFont);
            prompt.setCharacterSize(22);
            prompt.setFillColor(sf::Color::Black);
            prompt.setString("Appuyez sur ENTREE pour revenir au menu");

            sf::FloatRect pb = prompt.getLocalBounds();
            prompt.setOrigin(pb.width / 2.f, pb.height / 2.f);
            prompt.setPosition(640.f, 400.f);
            mWindow.draw(prompt);

        }
    }

    mWindow.display();
}
