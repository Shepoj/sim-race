#include "Boat.h"

// Constructeur : initialise le bateau dans le moteur physique avec une couleur donnée
Boat::Boat(sim::physics::PhysicsEngine& engine, sim::physics::DynamicModel& model, sf::Color color)
    : modele(model), couleur(color), poissonsTransportes(0), timerFlash(0.f) {
    corps = engine.createGenericBoat();
}

// Gère la création et la mise à jour des bulles de sillage derrière le bateau
void Boat::updateTrail(float dt) {
    // Ajout d'un nouveau cercle de sillage si le bateau bouge
    if (corps->getState().velocity_body.x != 0 || corps->getState().velocity_body.y != 0) {
        Trail nouveau;
        nouveau.forme.setRadius(2.f); // petite taille initiale
        nouveau.forme.setOrigin(2.f, 2.f); // centré
        nouveau.forme.setFillColor(sf::Color(255, 255, 255, 100)); // blanc translucide

        auto& position = corps->getState().position;
        float orientation = corps->getState().yaw;

        // Position derrière le bateau
        sf::Vector2f decalage(-std::cos(orientation) * 30.f, -std::sin(orientation) * 30.f);
        nouveau.forme.setPosition(position.x + decalage.x, position.y + decalage.y);

        nouveau.duree = 0.2f;
        nouveau.dureeMax = 0.2f;
        sillages.push_back(nouveau);
    }

    // Mise à jour des sillages : grossissement + transparence
    for (auto it = sillages.begin(); it != sillages.end(); ) {
        it->duree -= dt;

        if (it->duree <= 0.f) {
            it = sillages.erase(it); // retirer si expiré
        } else {
            float ratio = it->duree / it->dureeMax;

            // Agrandissement progressif
            float nouveauRayon = 2.f + (1.f - ratio) * 5.f;
            it->forme.setRadius(nouveauRayon);
            it->forme.setOrigin(nouveauRayon, nouveauRayon);

            // Diminution de l'opacité
            sf::Color couleur = it->forme.getFillColor();
            couleur.a = static_cast<sf::Uint8>(100 * ratio);
            it->forme.setFillColor(couleur);

            ++it;
        }
    }
}

// Met à jour la physique du bateau selon l'accélération et la direction
void Boat::update(float throttle, float steering, float dt) {
    constexpr double rho = 1026.0; // densité de l'eau
    double rpm = throttle * 10000.0;
    double angleGouvernail = steering * 1.2;

    auto amortissement = modele.computeDamping(*corps, rho);
    auto propulsion = modele.computeActuators(*corps, rpm, angleGouvernail, rho);

    // Réduction du freinage naturel
    amortissement.x *= 0.1;
    amortissement.y *= 0.1;
    amortissement.z *= 0.1;

    // Amplification de la poussée
    propulsion.x *= 8.0;
    propulsion.y *= 8.0;

    sim::common::ForceTorque2D forceTotale;
    forceTotale.x = amortissement.x + propulsion.x;
    forceTotale.y = amortissement.y + propulsion.y;
    forceTotale.z = (amortissement.z + propulsion.z) * 4.0; // rotation renforcée

    corps->addForce(forceTotale);

    updateTrail(dt); // Mise à jour du sillage

    // Mise à jour du flash visuel s’il est actif
    if (timerFlash > 0.f) {
        timerFlash -= dt;
    }
}

// Dessine le bateau et son nombre de poissons à l'écran
void Boat::draw(sf::RenderWindow& fenetre, const sf::Font& police) const {
    auto& etat = corps->getState();

    // Dessin des bulles de sillage
    for (const auto& sillage : sillages) {
        fenetre.draw(sillage.forme);
    }

    // Forme du bateau : polygone avec pointe à l’avant
    sf::ConvexShape formeBateau;
    formeBateau.setPointCount(5);
    formeBateau.setPoint(0, sf::Vector2f(60.f, 15.f));
    formeBateau.setPoint(1, sf::Vector2f(45.f, 0.f));
    formeBateau.setPoint(2, sf::Vector2f(0.f, 0.f));
    formeBateau.setPoint(3, sf::Vector2f(0.f, 30.f));
    formeBateau.setPoint(4, sf::Vector2f(45.f, 30.f));

    formeBateau.setOrigin(30.f, 15.f);
    formeBateau.setPosition(etat.position.x, etat.position.y);
    formeBateau.setRotation(etat.yaw * 180.f / 3.14159f);

    // Choix de la couleur selon l'effet flash
    sf::Color couleurAffichee = (timerFlash > 0.f) ? couleurFlash : couleur;
    formeBateau.setFillColor(couleurAffichee);

    fenetre.draw(formeBateau);

    // Affichage du nombre de poissons transportés
    sf::Text texte;
    texte.setFont(police);
    texte.setCharacterSize(14);
    texte.setFillColor(sf::Color::Black);
    texte.setString(std::to_string(poissonsTransportes));

    // Centrage sur le bateau
    sf::FloatRect limites = texte.getLocalBounds();
    texte.setOrigin(limites.width / 2.f, limites.height / 2.f);
    texte.setPosition(etat.position.x, etat.position.y);

    fenetre.draw(texte);
}

// Positionne le bateau à l'écran
void Boat::setPosition(float x, float y) {
    corps->setPosition(x, y);
}

// Applique une rotation initiale
void Boat::setRotation(float angle) {
    corps->setRotation(angle);
}

// Renvoie la position actuelle du bateau
sf::Vector2f Boat::getPosition() const {
    const auto& pos = corps->getState().position;
    return sf::Vector2f(static_cast<float>(pos.x), static_cast<float>(pos.y));
}

// Définit le nombre de poissons en cours de transport
void Boat::setFishCount(int count) {
    poissonsTransportes = count;
}

// Déclenche un effet visuel temporaire (ex: flash doré)
void Boat::flash(const sf::Color& couleurEffet) {
    timerFlash = 0.2f;
    couleurFlash = couleurEffet;
}

// Empêche le bateau de sortir de l'écran
void Boat::clampToWindow(float largeur, float hauteur) {
    auto& pos = corps->getState().position;
    float x = std::clamp(static_cast<float>(pos.x), 0.f, largeur);
    float y = std::clamp(static_cast<float>(pos.y), 0.f, hauteur);
    corps->setPosition(x, y);
}
