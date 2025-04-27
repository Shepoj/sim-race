#pragma once

#include <SFML/Graphics.hpp>
#include <physics/physicsEngine.h>
#include <physics/dynamicModel.h>

class Boat {
public:
    Boat(sim::physics::PhysicsEngine& engine, sim::physics::DynamicModel& model, sf::Color color);
    void update(float throttle, float steering, float dt);
    void draw(sf::RenderWindow& window) const;
    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;

private:
    sim::physics::RigidBody* mBody;
    sim::physics::DynamicModel& mModel;
    sf::Color mColor;
};
