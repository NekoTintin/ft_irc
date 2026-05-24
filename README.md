_This project has been created as part of the 42 curriculum by qupollet, mlelu, tmalkawi._

# ft_irc
IRC

# Sources

- Poll():
    https://bousk.developpez.com/cours/reseau-c++/TCP/05-envoi-reception-serveur/
    https://www.ibm.com/docs/fr/i/7.5.0?topic=designs-using-poll-instead-select
    
- Sockets:
    https://youtu.be/gntyAFoZp-E?si=g4ap0MMGicZ-UDn7

- Recve():
   https://pubs.opengroup.org/onlinepubs/007904975/functions/recv.html
   https://www.ibm.com/docs/en/zos/2.5.0?topic=functions-recv-receive-data-socket

- Signal :
   https://www.geeksforgeeks.org/cpp/signal-handling-in-cpp/

- Project overview :
   https://nathaan.me/projects/irc
   https://modern.ircdocs.horse/
   https://mathieu-lemoine.developpez.com/tutoriels/irc/protocole/?page=connexion


Membre A → Réseau / serveur bas niveau

Responsable :

socket setup --> ok
poll() --> ok
event loop --> ok
accept clients --> ok
recv/send
gestion FD
buffers
parsing brut
Membre B → IRC logique métier

Details :

1.Sécuriser l’arrêt du serveur ----> ok
   Remplacer while (1) par while (_running). 
   Gérer SIGINT.
   Fermer proprement tous les fd.
   Objectif : plus d’arrêt brutal sous Valgrind.

2.Finaliser removeClient(fd) ----> ok
   close(fd)
   supprimer le fd de _pollfds
   supprimer le client de _clients
   tester les déconnexions avec plusieurs nc.

3.Améliorer receiveFromClient(fd) -----> ok
   recv()
   si recv == 0 → déconnexion
   si recv > 0 → ajouter au buffer du Client
   ne plus seulement afficher le message.

4.Gérer les buffers TCP
   Chaque client doit avoir son propre std::string _buffer. ---> ok
   Accumuler les morceaux reçus. -----> ok
   Chercher \r\n. ------> ok
   Extraire une commande complète. -----> ok
   Garder le reste du buffer s’il y a une commande incomplète. -----> ok
   Créer une fonction de traitement des lignes ---------> ok
   Quand une ligne complète est trouvée :
   l’extraire du buffer, ----------------> ok
   l’envoyer au futur parser, TO DONE -> NO PARSER YET
   pour l’instant juste l’afficher proprement.
   Tester les messages fragmentés ----------> ok
   Avec nc -C.

Tester le cas du sujet :

com^Dman^Dd
Objectif : reconstruire correctement une commande reçue en plusieurs morceaux.

5.Préparer l’interface avec le membre B

Ta partie doit pouvoir fournir :

fd client + ligne IRC complète
Son parser/command handler traitera ensuite :
PASS, NICK, USER, JOIN, etc.

6.Ajouter une file d’envoi plus tard
   Ne pas forcément faire send() directement partout.
   Prévoir un buffer d’écriture par client.
   Plus tard : surveiller POLLOUT si besoin.

7.Tests à faire maintenant
   1 client se connecte.
   2 clients se connectent.
   Un client envoie un message.
   Deux clients envoient en même temps.
   Un client quitte.
   Plusieurs clients quittent.
   Valgrind avec --track-fds=yes.

Objectif final de ta partie
   Serveur non bloquant stable.
   Multiples clients.
   Buffers par client.
   Commandes IRC complètes reconstruites.
   Déconnexions propres.
   Aucun crash.

Responsable :

commandes IRC
channels
users/operators
permissions
MODE
PRIVMSG
architecture objet