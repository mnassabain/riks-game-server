ALGO
====

    INITIALISATION
    --------------

    Détérminer nombre units pour chaque joueur
    Détérminer le premier joueur = firstPlayer
    activePlayer := firstPlayer

    Tant qu'il y a des territoires vierges
        activePlayer place un unit sur le territoire
        activePlayer := prochain joueur

    Tant qu'il y a des unités à placer
        activePlayer place unité sur son territoire
        activePlayer := prochain joueur


    GAME LOOP
    ---------

    Tant que nombre de joueurs > 1
        // activePlayer turn

        // phase 1
        activePlayer.receiveUnits(p/r aux territoires & continents)
        Tant que player.availableUnits > 0 || player.tokens > 4 
            Si activePlayer peut utiliser tokens
                activePlayer.receiveUnits(n)
                activePlayer.loseTokens(m)
                activePlayer.placeUnits()

        // phase 2
        Tant que activePlayer peut attaquer
            // attaque
            Choisir territoire allié = ta
                Si ta.units > 1 && ennemi adj.
                    Choisir ennemi adj. = te
                    Choisir units(1..min(3, ta.units-1))
                    Prevenir joueur envahi = pEnvahi
                    pEnvahi choisit units (1..min(2, te.units))
                    Algo combat
                    Si 1er territoire envahi
                        Game.giveToken(activePlayer)
                    Si pEnvahi eliminé
                        activePlayer.takeTokens(pEnvahi.tokens)
                        nombre de joueurs--

        // phase 3
        Tant que possibilité de fortification
            Choisir territoire allié = ta
            Si ta.units > 1 && allié adj.
                Chosir allié adj.
                Choisir units (1..ta.units-1)
                Placer units

        // fin du tour
        activePlayer := next