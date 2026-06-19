#!/bin/bash

SERVER="127.0.0.1"
PORT=6667
PASS="pass" # Modifie si ton mot de passe est différent

echo "=================================================="
echo "🔥 LABO DE DESTRUCTION DE FT_IRC : STRESS TESTS 🔥"
echo "=================================================="

# --- TEST 1 : Le Ctrl+D / Commande tronquée (Pas de \r\n finaux) ---
echo -e "\n💥 Test 1 : Envoi partiel sans saut de ligne + coupure nette..."
(
    echo -n "PASS $PASS"
    sleep 0.2
    echo -n "NICK TintinCtrlD"
    sleep 0.2
    # On coupe le flux sans envoyer de \r\n pour simuler un Ctrl+D à mi-chemin
) | nc -w 1 $SERVER $PORT
echo "✅ Test 1 fini. (Vérifie que le serveur n'a pas bouclé à 100% CPU)"

# --- TEST 2 : Le Poison Post-QUIT (Commandes après le QUIT) ---
echo -e "\n💥 Test 2 : Injection de commandes post-QUIT..."
(
    echo "PASS $PASS"
    echo "NICK TintinPoison"
    echo "USER quentin 0 * :Quentin"
    sleep 0.2
    # On envoie QUIT et immédiatement une autre commande dans le même buffer
    echo -e "QUIT :Bye\r\nNICK ZombiePseudo\r\nPRIVMSG #canal :Je suis mort ?"
    sleep 0.5
) | nc -w 1 $SERVER $PORT
echo "✅ Test 2 fini. (Le serveur doit ignorer 'ZombiePseudo')"

# --- TEST 3 : Le Flood et coupure sauvage (Simule un gros Ctrl+C) ---
echo -e "\n💥 Test 3 : Gros flood de texte + déconnexion brutale (Crash Test)..."
(
    echo "PASS $PASS"
    echo "NICK TintinFlood"
    echo "USER quentin 0 * :Quentin"
    sleep 0.2
    echo "JOIN #floodzone"
    
    # On bombarde le serveur avec 200 messages très vite
    for i in {1..200}; do
        echo "PRIVMSG #floodzone :SPAM MESSAGE NUMERO $i SHOTGUN !!!"
    done
    # Pas de QUIT, on coupe la socket brutalement au milieu du flux
) | nc -w 1 $SERVER $PORT
echo "✅ Test 3 fini. (Vérifie la libération de la mémoire/FDs sur ton serveur)"

# --- TEST 4 : Multi-connexions simultanées (Attaque de clones) ---
echo -e "\n💥 Test 4 : Lancement de 10 clients éphémères en même temps..."
for id in {1..10}; do
    (
        echo "PASS $PASS"
        echo "NICK Clone$id"
        echo "USER clone 0 * :Clone"
        sleep 0.5
        echo "QUIT :Disparition"
    ) | nc $SERVER $PORT > /dev/null &
done

# Attente que les clients en tâche de fond se terminent
sleep 2
echo "✅ Test 4 fini."

echo -e "\n=================================================="
echo "🎉 Tous les stress tests sont passés côté script !"
echo "Regarde ton terminal Valgrind pour traquer les fuites."
echo "=================================================="