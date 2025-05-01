#include "Menu.h"

/// Affiche le menu principal avec un titre et les options de jeu
void Menu::draw(sf::RenderWindow& fenetre, const sf::Font& police) {
    // Titre centré
    sf::Text titre("Course de Bateaux", police, 40);
    sf::FloatRect limitesTitre = titre.getLocalBounds();
    titre.setOrigin(limitesTitre.width / 2.f, limitesTitre.height / 2.f);
    titre.setPosition(640.f, 200.f);
    titre.setFillColor(sf::Color::Black);
    fenetre.draw(titre);

    // Instructions pour choisir le nombre de joueurs
    sf::Text options("1 joueur : Appuyer sur 1\n2 joueurs : Appuyer sur 2", police, 30);
    sf::FloatRect limitesOptions = options.getLocalBounds();
    options.setOrigin(limitesOptions.width / 2.f, limitesOptions.height / 2.f);
    options.setPosition(640.f, 320.f);
    options.setFillColor(sf::Color::Black);
    fenetre.draw(options);
}
