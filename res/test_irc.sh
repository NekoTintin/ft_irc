#!/bin/bash

# Configuration - Adapte le port si nécessaire
SERVER="127.0.0.1"
PORT=6667
PASS="pass" # Laisse vide si ton serveur n'a pas de pass global

echo "=== Début des tests automatiques pour ft_irc ==="

# 1. Test de robustesse : Envoi de commandes incomplètes et d'espaces
echo "-> Test 1 : Commande malformée et espaces..."
(
    echo "PASS $PASS"
    echo "   NICK    Tintin   "
    echo "USER quentin 0 * :Quentin"
    sleep 1
    echo "PRIVMSG #canal_inexistant :Hého ?" # Ne doit pas faire planter le serveur
    echo "QUIT :Bye"
) | nc -w 2 $SERVER $PORT > /dev/null
echo "✅ Test 1 terminé (Vérifie que le serveur n'a pas crashé)."

# 2. Scénario de communication entre deux utilisateurs (Tintin et Neko)
echo "-> Test 2 : Scénario Canaux et Messages..."

# On lance Tintin en tâche de fond, mais on maintient son nc ouvert pendant 3 secondes
# AVANT qu'il ne tape QUIT, pour laisser le temps à Neko de faire ses tests.
(
    echo "PASS $PASS"
    echo "NICK Tintin"
    echo "USER quentin 0 * :Quentin"
    echo "JOIN #secret"
    echo "MODE #secret +k chaton"
    echo "PRIVMSG #secret :J'attends Neko..."
    sleep 3  # <-- Tintin reste connecté et garde le canal actif !
    echo "QUIT :Leaving"
) | nc $SERVER $PORT > /tmp/irc_out1.log &
pid1=$!

# On attend 1 seconde que Tintin ait bien configuré le canal +k
sleep 1

# On lance Neko
(
    echo "PASS $PASS"
    echo "NICK Neko"
    echo "USER neko 0 * :Neko"
    echo "JOIN #secret"         # Doit échouer (ERR_BADCHANNELKEY 475)
    echo "JOIN #secret chaton"  # Doit réussir
    echo "PRIVMSG #secret :Salut Tintin !"
    echo "QUIT :Leaving"
) | nc $SERVER $PORT > /tmp/irc_out2.log &
pid2=$!

wait $pid1 2>/dev/null
wait $pid2 2>/dev/null

echo "✅ Test 2 terminé."

# 3. Affichage des logs obtenus pour vérification visuelle
echo -e "\n=== LOGS DU CLIENT 2 (Neko) ==="
if [ -f /tmp/irc_out2.log ]; then
    cat /tmp/irc_out2.log
else
    echo "Aucun log généré."
fi

# Nettoyage des fichiers temporaires
rm -f /tmp/irc_pipe1 /tmp/irc_pipe2 /tmp/irc_out1.log /tmp/irc_out2.log
echo -e "\n=== Nettoyage effectué. Tests terminés ! ==="
