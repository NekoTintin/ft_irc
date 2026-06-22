#!/bin/bash

# Configuration
SERVER="localhost"
PORT=6667
LOG_FILE="server_test.log"

# On nettoie les anciens logs
> $LOG_FILE

echo "Lancement des tests sur le serveur..."

# Utilisation d'un heredoc pour envoyer les commandes en séquence
(
    echo "PASS pass"
    sleep 0.2
    echo "NICK NekoTintin"
    sleep 0.2
    echo "USER Tintin 0 * :Quentin Le Realname"
    sleep 0.5
    echo "JOIN #test"
    sleep 0.2
    echo "TOPIC #test :Bienvenue sur Rasbi"
    sleep 0.2
    echo "PRIVMSG #test :Ceci est un test de broadcast"
    sleep 0.2
    echo "KICK #test NekoTintin :Test de kick"
    sleep 0.2
    echo "QUIT :Au revoir !"
) | nc $SERVER $PORT > $LOG_FILE 2>&1

echo "Tests terminés. Analyse du fichier $LOG_FILE pour vérifier les réponses RPL (001, 332, etc.)."