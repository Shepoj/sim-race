#pragma once

#include <SFML/Graphics.hpp>

/// Classe représentant le menu principal du jeu
class Menu {
public:
    /// Affiche le menu dans la fenêtre
    /// @param fenetre La fenêtre de rendu
    /// @param police La police utilisée pour le texte
    void draw(sf::RenderWindow& fenetre, const sf::Font& police);
};
