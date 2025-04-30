#include "Boat.h"

Boat::Boat(sim::physics::PhysicsEngine& engine, sim::physics::DynamicModel& model, sf::Color color)
    : mModel(model), mColor(color)
{
    mBody = engine.createGenericBoat();
    mFishCount = 0;

}

void Boat::updateTrail(float dt) {
    // Ajouter un nouveau cercle à l'arrière
    if (mBody->getState().velocity_body.x != 0 || mBody->getState().velocity_body.y != 0) {
        Trail newTrail;
        newTrail.shape.setRadius(2.f); // petite bulle au début
        newTrail.shape.setOrigin(2.f, 2.f);
        newTrail.shape.setFillColor(sf::Color(255, 255, 255, 100)); // blanc semi-transparent
        auto& pos = mBody->getState().position;
        float yaw = mBody->getState().yaw;

        sf::Vector2f decalage;
        decalage.x = -std::cos(yaw) * 30.f;
        decalage.y = -std::sin(yaw) * 30.f;

        newTrail.shape.setPosition(pos.x + decalage.x, pos.y + decalage.y);

        newTrail.lifetime = 0.2f;     // Durée actuelle
        newTrail.maxLifetime = 0.2f;  // Durée totale au départ
        mTrail.push_back(newTrail);

    }

    // Faire vieillir les sillages
    for (auto it = mTrail.begin(); it != mTrail.end(); ) {
    it->lifetime -= dt;
    if (it->lifetime <= 0.f) {
        it = mTrail.erase(it);
    } else {
        // Calculer combien de temps il reste (0.0 -> 1.0)
        float ratio = it->lifetime / it->maxLifetime;

        // Faire GROSSIR progressivement le cercle
        float newRadius = 2.f + (1.f - ratio) * 5.f; // grossit de 2px à 7px
        it->shape.setRadius(newRadius);
        it->shape.setOrigin(newRadius, newRadius); // recentrer à chaque fois

        // Rendre le cercle plus transparent
        sf::Color color = it->shape.getFillColor();
        color.a = static_cast<sf::Uint8>(100 * ratio); // 100 à 0 alpha
        it->shape.setFillColor(color);

        ++it;
    }
}

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

    updateTrail(dt); // ➔ Nouveau
    if (mFlashTimer > 0.f) {
        mFlashTimer -= dt;
    }

}


void Boat::draw(sf::RenderWindow& window, const sf::Font& font) const {
    auto& state = mBody->getState();
    for (const auto& trail : mTrail) {
    window.draw(trail.shape);
}

    sf::ConvexShape boat;
    boat.setPointCount(5);

    // Définir les points du bateau (plus long et fin)
    boat.setPoint(0, sf::Vector2f(60.f, 15.f)); // Pointe avant (droite au milieu)
    boat.setPoint(1, sf::Vector2f(45.f, 0.f));  // Coin haut
    boat.setPoint(2, sf::Vector2f(0.f, 0.f));   // Coin arrière haut
    boat.setPoint(3, sf::Vector2f(0.f, 30.f));  // Coin arrière bas
    boat.setPoint(4, sf::Vector2f(45.f, 30.f)); // Coin bas

    boat.setOrigin(30.f, 15.f); // Centre bien au milieu
    boat.setPosition(state.position.x, state.position.y);
    boat.setRotation(state.yaw * 180.f / 3.14159f);
    boat.setFillColor(mColor);
    sf::Color drawColor = (mFlashTimer > 0.f) ? mFlashColor : mColor;
    boat.setFillColor(drawColor);

    window.draw(boat);

    sf::Text fishText;
    fishText.setFont(font);
    fishText.setCharacterSize(14);
    fishText.setFillColor(sf::Color::Black);
    fishText.setString(std::to_string(mFishCount));

    sf::FloatRect bounds = fishText.getLocalBounds();
    fishText.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
    fishText.setPosition(state.position.x, state.position.y);

    window.draw(fishText);


}



void Boat::setPosition(float x, float y) {
    mBody->setPosition(x, y);
}

sf::Vector2f Boat::getPosition() const {
    const auto& state = mBody->getState();
    return sf::Vector2f(static_cast<float>(state.position.x), static_cast<float>(state.position.y));

}

void Boat::setFishCount(int count) {
    mFishCount = count;
}

void Boat::flash(const sf::Color& color) {
    mFlashTimer = 0.2f;
    mFlashColor = color;
}

void Boat::setRotation(float angleRad) {
    mBody->setRotation(angleRad);
}
