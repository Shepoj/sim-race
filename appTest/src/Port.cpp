#include "Port.h"
#include <string>

/// Constructeur du port : initialise sa forme, sa couleur, sa position et son texte
Port::Port(const sf::Vector2f& position, const sf::Color& couleur, const sf::Font& police)
    : score(0)
{
    forme.setSize(sf::Vector2f(40.f, 120.f));         // Taille du port
    forme.setOrigin(20.f, 60.f);                      // Origine centrée
    forme.setPosition(position);                      // Position fournie
    forme.setFillColor(couleur);                      // Couleur du port

    texteScore.setFont(police);                       // Police pour le score
    texteScore.setCharacterSize(20);                  // Taille de caractère
    texteScore.setFillColor(sf::Color::Black);        // Couleur du texte

    // Centrage approximatif du texte par rapport à la forme
    sf::FloatRect limitesTexte = texteScore.getLocalBounds();
    texteScore.setOrigin(limitesTexte.width / 2.f, limitesTexte.height / 2.f);
    texteScore.setPosition(position.x - 6.f, position.y - 10.f);
    texteScore.setString("0");
}

/// Affiche le port et le score sur la fenêtre
void Port::draw(sf::RenderWindow& fenetre) const {
    fenetre.draw(forme);
    fenetre.draw(texteScore);
}

/// Retourne la zone de collision du port (utilisée pour détecter les livraisons)
sf::FloatRect Port::getBounds() const {
    return forme.getGlobalBounds();
}

/// Incrémente le score quand un poisson est livré, et met à jour l'affichage
void Port::depositFish() {
    ++score;
    texteScore.setString(std::to_string(score));
}

/// Retourne le score actuel
int Port::getScore() const {
    return score;
}
