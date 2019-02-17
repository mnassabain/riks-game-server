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
        activePlayer place unité sur un de ses territoires
        activePlayer := prochain joueur


    GAME LOOP
    ---------

    Tant que nombre de joueurs > 1
        // activePlayer turn

        // phase 1
        activePlayer.receiveUnits(min(3,nbTerritoiresAllies/3 + bonus continents))
        Tant que player.availableUnits > 0 
            Tant que player.tokens > 4 
                Si activePlayer veut utiliser tokens
                    activePlayer.receiveUnits(n)
                    activePlayer.loseTokens(m)
                
            activePlayer.placeUnits()

        // phase 2
        Tant que activePlayer peut attaquer
            // attaque
            Si activePlayer veut attaquer
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
            Sinon
                phase suivante

        // phase 3
        Tant que activePlayer veut deplacer units
            Choisir territoire allié = ta
            Si ta.units > 1 && allié adj.
                Chosir allié adj.
                Choisir units (1..ta.units-1)
                Placer units

        // fin du tour
        activePlayer := next