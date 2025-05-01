#pragma once

#include <SFML/Graphics.hpp>

/// Classe représentant un "port" où les poissons sont déposés
class Port {
public:
    /// Constructeur du port
    /// @param position Position du port dans la fenêtre
    /// @param couleur Couleur du port (liée au joueur)
    /// @param police Police utilisée pour l'affichage du score
    Port(const sf::Vector2f& position, const sf::Color& couleur, const sf::Font& police);

    /// Affiche le port et le score à l'écran
    void draw(sf::RenderWindow& fenetre) const;

    /// Retourne les limites du port (utilisé pour les collisions avec le bateau)
    sf::FloatRect getBounds() const;

    /// Dépose les poissons ramenés : incrémente le score
    void depositFish();

    /// Retourne le score actuel (poissons sécurisés)
    int getScore() const;

private:
    sf::RectangleShape forme;    // Forme graphique du port
    int score;                   // Nombre de poissons déposés
    sf::Text texteScore;         // Texte affichant le score sur le port
};
