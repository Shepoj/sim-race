#include "Game.h"
#include <iostream>
#include <cmath>
#include <algorithm>

// Constructeur de la classe Game
Game::Game()
    : fenetre(sf::VideoMode(1280, 720), "Course de Bateaux !"),
      etat(GameState::Menu),
      nombreJoueurs(1),
      gagnant(0),
      tempsEcoule(0.0f),
      bateau1(nullptr),
      bateau2(nullptr),
      acceleration1(0.f), rotation1(0.f),
      acceleration2(0.f), rotation2(0.f),
      tempsPartie(0.f), dureeMaxPartie(60.f) {

    // Chargement de la police pour afficher les textes à l’écran
    if (!police.loadFromFile("CalSans-Regular.ttf")) {
        std::cerr << "Erreur chargement police CalSans-Regular.ttf" << std::endl;
    }

    // Chargement de l’image de poisson
    if (!texturePoisson.loadFromFile("fish.png")) {
        std::cerr << "Erreur chargement fish.png" << std::endl;
    }

    // Initialisation du texte du chronomètre en haut de l’écran
    texteChrono.setFont(police);
    texteChrono.setCharacterSize(28);
    texteChrono.setFillColor(sf::Color::Black);
    texteChrono.setPosition(560.f, 20.f);

    // Définition de la ligne d’arrivée (c'etait pour l'ancien jeu)
    ligneArrivee.setSize(sf::Vector2f(10.f, 200.f));
    ligneArrivee.setFillColor(sf::Color::Green);
    ligneArrivee.setOrigin(5.f, 100.f);
    ligneArrivee.setPosition(1180.f, 360.f);
}

void Game::run() {
    // Boucle principale du jeu : tourne tant que la fenêtre est ouverte
    while (fenetre.isOpen()) {
        // Calcule le temps écoulé depuis la dernière image
        float dt = horloge.restart().asSeconds();
        
        // Gère les entrées clavier et les événements (fermeture, touches, etc.)
        processEvents();
        
        if (etat == GameState::Playing) {
            // Met à jour les positions, collisions, scores, poissons, etc
            update(dt);
            
            // Ajoute le temps écoulé à la durée totale de la partie
            tempsEcoule += dt;
        }
        // Affiche le contenu à l’écran
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    // Boucle qui traite tous les événements en attente
    while (fenetre.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            fenetre.close();

        // Si on est dans le menu principal
        if (etat == GameState::Menu) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Num1) {
                    startGame(1);
                } else if (event.key.code == sf::Keyboard::Num2) {
                    startGame(2);
                }
            }
        }
            
        // Si on est dans l'écran de fin de partie
        else if (etat == GameState::GameOver) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                etat = GameState::Menu;
            }
        }
    }

    // Si on est en train de jouer
    if (etat == GameState::Playing) {
        acceleration1 = rotation1 = acceleration2 = rotation2 = 0.0f;

        // Commandes joueur 1 (flèches)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) acceleration1 = 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) rotation1 = 1.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) rotation1 = -1.0f;

        // Commandes joueur 2 (ZQSD)
        if (nombreJoueurs == 2) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) acceleration2 = 1.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) rotation2 = 1.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) rotation2 = -1.0f;
        }
    }
}

void Game::startGame(int nbJoueurs) {
    nombreJoueurs = nbJoueurs;
    etat = GameState::Playing;
    gagnant = 0;
    tempsEcoule = 0.0f;
    tempsPartie = 0.0f;
    bateauxSeTouchaient = false;

    // Réinitialisation du terrain
    obstacles.clear();
    for (int i = 0; i < 10; ++i)
        obstacles.emplace_back();

    // Création du bateau du joueur 1
    bateau1 = new Boat(moteurPhysique, modelePhysique, sf::Color(60, 180, 75));
    bateau1->setPosition(50.f, 360.f);
    poissonsTransportes1 = 0;

    // Création du bateau du joueur 2 si nécessaire
    if (nombreJoueurs == 2) {
        bateau2 = new Boat(moteurPhysique, modelePhysique, sf::Color(200, 60, 60));
        bateau2->setPosition(1230.f, 360.f);
        bateau2->setRotation(3.14159f); // vers la gauche
        poissonsTransportes2 = 0;
    }

    // Ports
    port1 = new Port(sf::Vector2f(50.f, 360.f), sf::Color(60, 180, 75), police);
    if (nombreJoueurs == 2)
        port2 = new Port(sf::Vector2f(1230.f, 360.f), sf::Color(200, 60, 60), police);

    // Poissons
    poissons.clear();
    timerApparitionPoisson = 0.f;
    intervallePoisson = 2.0f;
    maxPoissonsEcran = 2;
}

void Game::update(float dt) {
    // Mise à jour des bateaux (mouvement et contraintes écran)
    if (bateau1) {
        bateau1->update(acceleration1, rotation1, dt);
        bateau1->clampToWindow(1280.f, 720.f);
    }
    if (nombreJoueurs == 2 && bateau2) {
        bateau2->update(acceleration2, rotation2, dt);
        bateau2->clampToWindow(1280.f, 720.f);
    }

    // Gestion des collisions bateau <-> obstacle
    auto checkCollision = [](Boat* b, const std::vector<Obstacle>& obs) {
        sf::Vector2f posBateau = b->getPosition();
        for (const auto& o : obs) {
            sf::Vector2f posObs = o.getPosition();
            float dist = std::hypot(posBateau.x - posObs.x, posBateau.y - posObs.y);
            float rayonMin = o.getRadius() + 20.f; // 20 = demi-longueur bateau
            if (dist < rayonMin && dist != 0.f) {
                sf::Vector2f repulsion((posBateau.x - posObs.x) / dist, (posBateau.y - posObs.y) / dist);
                b->setPosition(posObs.x + repulsion.x * rayonMin, posObs.y + repulsion.y * rayonMin);
            }
        }
    };
    if (bateau1) checkCollision(bateau1, obstacles);
    if (nombreJoueurs == 2 && bateau2) checkCollision(bateau2, obstacles);

    // Simulation physique générale
    moteurPhysique.step(dt);

    // Mise à jour du chrono restant
    tempsPartie += dt;
    float tempsRestant = std::max(0.f, dureeMaxPartie - tempsPartie);
    texteChrono.setString("Temps : " + std::to_string(static_cast<int>(tempsRestant)) + "s");

    // Ramassage des poissons
    auto ramassePoisson = [](Boat* b, std::vector<Fish>& liste, int& compteur) {
        sf::Vector2f pos = b->getPosition();
        for (auto& f : liste) {
            if (!f.isCollected() && std::hypot(pos.x - f.getPosition().x, pos.y - f.getPosition().y) < 30.f) {
                f.markCollected();
                ++compteur;
            }
        }
    };
    if (bateau1) ramassePoisson(bateau1, poissons, poissonsTransportes1);
    if (nombreJoueurs == 2 && bateau2) ramassePoisson(bateau2, poissons, poissonsTransportes2);

    // Dépôt dans les ports
    if (port1->getBounds().contains(bateau1->getPosition())) {
        for (int i = 0; i < poissonsTransportes1; ++i) port1->depositFish();
        poissonsTransportes1 = 0;
    }
    if (nombreJoueurs == 2 && port2->getBounds().contains(bateau2->getPosition())) {
        for (int i = 0; i < poissonsTransportes2; ++i) port2->depositFish();
        poissonsTransportes2 = 0;
    }

    // Échange de poissons si les bateaux se touchent
    if (nombreJoueurs == 2) {
        float dx = bateau1->getPosition().x - bateau2->getPosition().x;
        float dy = bateau1->getPosition().y - bateau2->getPosition().y;
        float dist = std::sqrt(dx * dx + dy * dy);
        bool enContact = (dist < 40.f);

        if (enContact && !bateauxSeTouchaient) {
            int avant1 = poissonsTransportes1;
            int avant2 = poissonsTransportes2;
            std::swap(poissonsTransportes1, poissonsTransportes2);
            if (poissonsTransportes1 > avant1) bateau1->flash(sf::Color(255, 215, 0));
            if (poissonsTransportes2 > avant2) bateau2->flash(sf::Color(255, 215, 0));
        }
        bateauxSeTouchaient = enContact;
    }

    // Fin du jeu si temps écoulé
    if (tempsPartie >= dureeMaxPartie) {
        etat = GameState::GameOver;
    }

    // Apparition dynamique des poissons
    timerApparitionPoisson += dt;
    int nbActifs = std::count_if(poissons.begin(), poissons.end(), [](const Fish& f) {
        return !f.isCollected();
    });
    if (timerApparitionPoisson >= intervallePoisson && nbActifs < maxPoissonsEcran) {
        std::vector<sf::FloatRect> zonesAeviter;
        zonesAeviter.push_back(ligneArrivee.getGlobalBounds());
        for (const auto& o : obstacles) zonesAeviter.push_back(o.getGlobalBounds());
        zonesAeviter.push_back(port1->getBounds());
        if (nombreJoueurs == 2) zonesAeviter.push_back(port2->getBounds());

        poissons.emplace_back(texturePoisson, zonesAeviter);
        timerApparitionPoisson = 0.f;
    }
}

void Game::render() {
    fenetre.clear(sf::Color(40, 120, 200)); // fond bleu

    if (etat == GameState::Menu) {
        menu.draw(fenetre, police);

    } else if (etat == GameState::Playing || etat == GameState::GameOver) {
        // Dessin des obstacles
        for (const auto& obs : obstacles) obs.draw(fenetre);

        // Dessin des poissons
        for (const auto& fish : poissons) fish.draw(fenetre);

        // Dessin des ports
        if (port1) port1->draw(fenetre);
        if (nombreJoueurs == 2 && port2) port2->draw(fenetre);

        // Dessin des bateaux avec le nombre de poissons portés
        if (bateau1) {
            bateau1->setFishCount(poissonsTransportes1);
            bateau1->draw(fenetre, police);
        }
        if (nombreJoueurs == 2 && bateau2) {
            bateau2->setFishCount(poissonsTransportes2);
            bateau2->draw(fenetre, police);
        }

        // Chronomètre en haut au centre
        fenetre.draw(texteChrono);

        // Affichage de l'écran de fin
        if (etat == GameState::GameOver) {
            sf::Text titre("FIN DE LA PARTIE", police, 40);
            titre.setFillColor(sf::Color::Black);
            sf::FloatRect tb = titre.getLocalBounds();
            titre.setOrigin(tb.width / 2.f, tb.height / 2.f);
            titre.setPosition(640.f, 200.f);
            fenetre.draw(titre);

            sf::Text resultat;
            resultat.setFont(police);
            resultat.setCharacterSize(30);
            resultat.setFillColor(sf::Color::Black);

            if (nombreJoueurs == 1) {
                resultat.setString("Poissons attrapes : " + std::to_string(port1->getScore()));
            } else {
                int score1 = port1->getScore();
                int score2 = port2->getScore();
                std::string gagnantText = (score1 > score2) ? "      Vert gagne !" : (score2 > score1) ? "     Rouge gagne !" : "            Egalite !";
                resultat.setString("Vert : " + std::to_string(score1) + "  -  Rouge : " + std::to_string(score2) + "\n" + gagnantText);
            }
            sf::FloatRect rb = resultat.getLocalBounds();
            resultat.setOrigin(rb.width / 2.f, rb.height / 2.f);
            resultat.setPosition(640.f, 300.f);
            fenetre.draw(resultat);

            sf::Text instructions("Appuyez sur ENTREE pour revenir au menu", police, 22);
            instructions.setFillColor(sf::Color::Black);
            sf::FloatRect ib = instructions.getLocalBounds();
            instructions.setOrigin(ib.width / 2.f, ib.height / 2.f);
            instructions.setPosition(640.f, 400.f);
            fenetre.draw(instructions);
        }
    }

    fenetre.display();
}
