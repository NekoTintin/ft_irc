#!/bin/bash
{
  echo "PASS pass"
  echo "NICK Tintin"
  echo "USER t * 0 :t"
  sleep 1
  
  echo "--- 1. Test commande inconnue ---" >&2
  echo "GIBBERISH_COMMAND"
  
  echo "--- 2. Test manque de paramètres ---" >&2
  echo "PRIVMSG"
  echo "JOIN"
  
  echo "--- 3. Test message à un user qui n'existe pas ---" >&2
  echo "PRIVMSG PersonneQuiNExistePas :Coucou"
  
  echo "--- 4. Test double pseudo ---" >&2
  echo "NICK Tintin" 
  
  sleep 1
  echo "QUIT"
} | nc localhost 6667