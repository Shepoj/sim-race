#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Boat.h"
#include "Obstacle.h"
#include "Menu.h"
#include "Fish.h"
#include "Port.h"
#include <physics/physicsEngine.h>
#include <physics/dynamicModel.h>

// États possibles du jeu
enum class GameState {
    Menu,
    Playing,
    GameOver
};

// Classe principale de gestion du jeu
class Game {
public:
    Game();               // Constructeur
    void run();           // Lance la boucle principale du jeu

private:
    void processEvents();             // Gère les événements SFML
    void update(float dt);           // Met à jour la logique du jeu à chaque frame
    void render();                   // Dessine tous les éléments du jeu à l'écran
    void startGame(int nbJoueurs);   // Initialise une nouvelle partie avec 1 ou 2 joueurs

private:
    // Gestion de la fenêtre et du temps
    sf::RenderWindow fenetre;    // Fenêtre de jeu
    sf::Clock horloge;           // Chronomètre interne pour le delta time
    sf::Font police;             // Police d'écriture pour les textes

    // Gestion de l'état du jeu
    GameState etat;              // Menu, jeu en cours ou fin
    int nombreJoueurs;           // 1 ou 2 joueurs
    int gagnant;                 // Identifiant du joueur gagnant
    float tempsEcoule;          // Temps total depuis le début de la partie

    // Interface et décor
    Menu menu;                               // Menu principal
    sf::RectangleShape ligneArrivee;         // Ligne d'arrivée (mode course)
    std::vector<Obstacle> obstacles;         // Obstacles générés

    // Moteur physique
    sim::physics::PhysicsEngine moteurPhysique;   // Gestion des corps rigides
    sim::physics::DynamicModel modelePhysique;    // Calcul des forces appliquées

    // Bateaux
    Boat* bateau1;                 // Bateau du joueur 1
    Boat* bateau2;                 // Bateau du joueur 2 (s'il existe)
    float acceleration1, rotation1; // Commandes du joueur 1
    float acceleration2, rotation2; // Commandes du joueur 2
    bool bateauxSeTouchaient;     // Indique si les bateaux étaient en collision (évite les échanges multiples)

    // Mode pêche
    std::vector<Fish> poissons;        // Liste des poissons actuellement affichés
    Port* port1;                       // Port du joueur 1
    Port* port2;                       // Port du joueur 2 (s'il existe)
    int poissonsTransportes1;         // Nombre de poissons en cours de transport par le joueur 1
    int poissonsTransportes2;         // Nombre de poissons en cours de transport par le joueur 2

    // Chronométrage et score
    float tempsPartie;                // Temps écoulé de la partie actuelle
    float dureeMaxPartie;             // Durée totale allouée à la partie
    sf::Text texteChrono;             // Texte affichant le compte à rebours

    // Apparition des poissons
    sf::Texture texturePoisson;       // Texture utilisée pour représenter les poissons
    float timerApparitionPoisson;     // Temps depuis la dernière apparition
    float intervallePoisson;          // Délai entre deux apparitions
    int maxPoissonsEcran;             // Nombre maximal de poissons à l'écran
};
