#include "Boat.h"

Boat::Boat(sim::physics::PhysicsEngine& engine, sim::physics::DynamicModel& model, sf::Color color)
    : mModel(model), mColor(color)
{
    mBody = engine.createGenericBoat();
}

void Boat::update(float throttle, float steering, float dt) {
    constexpr double rho = 1026.0;
    double RPM = throttle * 10000.0;
    double helmAngle = steering * 1.2;

    auto damping = mModel.computeDamping(*mBody, rho);
    auto actuators = mModel.computeActuators(*mBody, RPM, helmAngle, rho);

    damping.x *= 0.1;
    damping.y *= 0.1;
    damping.z *= 0.1;
    actuators.x *= 8.0;
    actuators.y *= 8.0;

    sim::common::ForceTorque2D total;
    total.x = damping.x + actuators.x;
    total.y = damping.y + actuators.y;
    total.z = (damping.z + actuators.z) * 4.0;

    mBody->addForce(total);
}

void Boat::draw(sf::RenderWindow& window) const {
    auto& state = mBody->getState();
    sf::RectangleShape shape(sf::Vector2f(60.f, 30.f));
    shape.setOrigin(30.f, 15.f);
    shape.setPosition(state.position.x, state.position.y);
    shape.setRotation(state.yaw * 180.f / 3.14159f);
    shape.setFillColor(mColor);
    window.draw(shape);
}

void Boat::setPosition(float x, float y) {
    mBody->setPosition(x, y);
}

sf::Vector2f Boat::getPosition() const {
    const auto& state = mBody->getState();
    return { state.position.x, state.position.y };
}
