#pragma once

#include <SFML/Graphics.hpp>
#include <physics/physicsEngine.h>
#include <physics/dynamicModel.h>
#include <cmath>
#include <list>
#include <algorithm>
#include <iostream>

struct Trail {
    sf::CircleShape shape;
    float lifetime;
    float maxLifetime; // <-- ajouter cette variable
};



class Boat {
public:
    Boat(sim::physics::PhysicsEngine& engine, sim::physics::DynamicModel& model, sf::Color color);
    void updateTrail(float dt);
    void update(float throttle, float steering, float dt);
    void draw(sf::RenderWindow& window, const sf::Font& font) const;
    void setPosition(float x, float y);
    void setRotation(float angleRad);
    sf::Vector2f getPosition() const;
    void setFishCount(int count);
    void flash(const sf::Color& color); // on passe la couleur du flash
    void clampToWindow(float width, float height);


private:
    sim::physics::RigidBody* mBody;
    sim::physics::DynamicModel& mModel;
    sf::Color mColor;
    std::list<Trail> mTrail; // Liste des sillages
    int mFishCount;
    float mFlashTimer;
    sf::Color mFlashColor; // couleur du flash (doré, blanc...)

};
