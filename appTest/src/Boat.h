#pragma once

#include <SFML/Graphics.hpp>
#include <physics/physicsEngine.h>
#include <physics/dynamicModel.h>
#include <cmath>
#include <list>
#include <algorithm>
#include <iostream>

/// Représente une bulle de sillage (trail) derrière un bateau
struct Trail {
    sf::CircleShape forme;  // Cercle affiché
    float duree;            // Durée restante d'affichage
    float dureeMax;         // Durée totale initiale
};

/// Représente un bateau contrôlé par un joueur
class Boat {
public:
    /// Constructeur
    /// @param engine Référence vers le moteur physique
    /// @param model  Référence vers le modèle dynamique (calculs de forces)
    /// @param color  Couleur du bateau
    Boat(sim::physics::PhysicsEngine& engine, sim::physics::DynamicModel& model, sf::Color color);

    /// Met à jour le sillage en fonction du temps
    void updateTrail(float dt);

    /// Met à jour la physique et le mouvement du bateau
    /// @param throttle accélération (0 ou 1)
    /// @param steering rotation (1 gauche, -1 droite)
    /// @param dt temps écoulé
    void update(float throttle, float steering, float dt);

    /// Dessine le bateau et les sillages
    void draw(sf::RenderWindow& window, const sf::Font& font) const;

    /// Place le bateau à une position donnée
    void setPosition(float x, float y);

    /// Oriente le bateau selon un angle en radians
    void setRotation(float angleRad);

    /// Renvoie la position actuelle du bateau
    sf::Vector2f getPosition() const;

    /// Définit le nombre de poissons transportés (affiché sur le bateau)
    void setFishCount(int count);

    /// Déclenche un effet visuel de "flash" (par exemple quand on gagne un échange)
    void flash(const sf::Color& color);

    /// Empêche le bateau de sortir de l'écran
    void clampToWindow(float width, float height);

private:
    sim::physics::RigidBody* corps;             // Représentation physique
    sim::physics::DynamicModel& modele;         // Modèle dynamique
    sf::Color couleur;                          // Couleur du bateau
    std::list<Trail> sillages;                  // Liste de bulles représentant le sillage
    int poissonsTransportes;                    // Nombre de poissons actuellement portés
    float timerFlash;                           // Temps restant pour l'effet de flash
    sf::Color couleurFlash;                     // Couleur du flash
};
