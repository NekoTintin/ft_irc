#!/bin/bash

HOST="localhost"
PORT="6667"

{
  echo "PASS pass"
  echo "NICK Spammer"
  echo "USER spam * 0 :SpamBot"
  echo "JOIN #spamzone"
  sleep 1
  
  # Envoie 50 messages à la suite
  for i in {1..50}
  do
    echo "PRIVMSG #spamzone :Message numéro $i"
  done
  
  sleep 1
  echo "QUIT :Fin du flood"
} | nc $HOST $PORT