#!/bin/bash

HOST="localhost"
PORT="6667"

echo "=== Lancement du Client 1 (Alice) ==="
(
  echo "PASS pass"
  echo "NICK Alice"
  echo "USER alice * 0 :Alice"
  sleep 1
  echo "JOIN #salon"
  sleep 2
  echo "PRIVMSG #salon :Salut tout le monde !"
  sleep 3
  echo "QUIT :Au revoir"
) | nc $HOST $PORT > alice.log &

# On attend un tout petit peu que le premier client soit connecté
sleep 0.5

echo "=== Lancement du Client 2 (Bob) ==="
(
  echo "PASS pass"
  echo "NICK Bob"
  echo "USER bob * 0 :Bob"
  sleep 1
  echo "JOIN #salon"
  sleep 2
  echo "PRIVMSG #salon :Hey Alice ça va ?"
  sleep 2
  echo "QUIT :Bye"
) | nc $HOST $PORT > bob.log &

# Attendre que les deux processus en arrière-plan se terminent
wait

echo "=== Tests terminés ! ==="
echo "--- Contenu de alice.log (ce qu'Alice a reçu) ---"
cat alice.log
echo "--- Contenu de bob.log (ce qu'Bob a reçu) ---"
cat bob.log