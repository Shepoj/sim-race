#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

/// Classe représentant un poisson ramassable par les bateaux
class Fish {
public:
    /// Constructeur : place le poisson à une position aléatoire ne chevauchant aucune zone interdite
    /// @param texture Texture utilisée pour dessiner le poisson
    /// @param avoidZones Zones à éviter pour ne pas placer le poisson dessus
    Fish(const sf::Texture& texture, const std::vector<sf::FloatRect>& avoidZones);

    /// Dessine le poisson s'il n'a pas encore été ramassé
    void draw(sf::RenderWindow& window) const;

    /// Renvoie la position du poisson
    sf::Vector2f getPosition() const;

    /// Renvoie le rayon estimé du poisson (utile pour la détection de collisions)
    float getRadius() const;

    /// Indique si le poisson a déjà été ramassé
    bool isCollected() const;

    /// Marque ce poisson comme ramassé
    void markCollected();

private:
    sf::Sprite mSprite;  // Représentation graphique du poisson
    bool mCollected;     // Indique si ce poisson a été ramassé
};
