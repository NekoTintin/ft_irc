_This project has been created as part of the 42 curriculum by mlelu, qupollet, tmalkawi._

# ft_irc

Recreate a IRC Internet Relay Chat, a text based chat protocol.

Server : 
   - receives the connexion requests from several clients

Clients : 
   - each client is a user with a nickname
   - all the clients are stored in a Map which as an attribute of the Server

How to start :
   - Make
   - Open a terminal and ./ircserv 6667 pass
   - Arg 1 = port
   - Arg 2 = password of the server

Need to be careful about open FDs and make sure that they are all properly closed


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

- IRC Commands :
   https://fr.wikipedia.org/wiki/Aide:IRC/commandes
   https://dd.ircdocs.horse/refs/commands/user


----------------------------------------------------------------------------

GRANDES LIGNES DU PROJET :

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

Partie B :

1.Créer le CommandParser------------> ok
Le parser reçoit une ligne IRC complète :
"NICK malu"
Il doit identifier :
la commande,
les arguments.
Première étape :
découper correctement la ligne
-> Objectif :
avoir une architecture propre pour traiter les commandes IRC.

2.Découper les commandes IRC-------------->ok

Séparer :
commande = NICK 
args = malu
Comprendre les cas IRC :
PRIVMSG #42 :hello world
Le : signifie :
le reste est un seul argument
-> Objectif :
parser correctement les lignes IRC réelles.

3.Créer le système de dispatch
Le parser détecte :------------------>ok
PASS
NICK
USER
JOIN

Puis appelle : Handler ----> ok
rester avec un enum

handlePass() -> parametre un mot de passe 
handleNick() -> definir le pseudo
handleJoin() -> rejoindre un salon
-> Objectif :
éviter un énorme if/else illisible.

4.Ajouter l’état IRC dans Client
Ajouter :
nickname
username
authenticated
registered
Le Client devient un vrai utilisateur IRC.
-> Objectif :
stocker l’identité et l’état IRC du client.

UNE LIGNE PAR COMMANDE -> si plusieurs commandes dans une ligne, il s'agit d'une erreur
Il faut systematiquement verifier le nombre d'arguments pour la commande

5.Implémenter PASS--------> OK
Vérifier :
- mot de passe correct -> verifier avec le mot de passe du serveur (_password) -> OK
- nombre d'arguments -> 1 arguments max sinon erreur -> OK
- client deja enregistre ou non ? -> OK
PASS envoyé avant registration.
Refuser si incorrect.
-> Objectif :
sécuriser l’accès au serveur.

6.Implémenter NICK-----------------------> OK
Vérifier :
nickname valide:
- ne peut pas commencer par un chiffre
- alphanum ok + ces caracteres : _ - [ ] \ ` ^ { }
nickname unique.
- parcourir _clients dans server et verifier que le nickname n'est pas deja pris
Stocker dans Client.
-> Objectif :
donner une identité IRC au client.

7.Implémenter USER - NE PAS OUBLIER DE CHERCKER SI TOKEN[1] N'EST PAS VIDE DANS NICK
Stocker :
username---------> ok
realname---------> ok
Vérifier syntaxe minimale.
-> Objectif :
compléter l’identité IRC.

8.Gérer la registration IRC
Quand :
PASS ok
NICK ok
USER ok

Le client devient : ------------> ok
registered ---------------------> ok
Le serveur envoie :
001 Welcome J'EN SUIS LA => COMMENT FAIRE POUR QUE SERVER REPONDE AU BON CLIENT
-> Objectif :
permettre au client d’utiliser IRC.

9.Créer sendToClient()

Wrapper autour de :
send()
Envoyer proprement :
réponses,
erreurs,
messages IRC.
-> Objectif :
centraliser tous les envois réseau.

10.Créer la classe Channel
Un channel contient :
nom
membres
operators
topic
modes
-> Objectif :
représenter les salons IRC.

11.Implémenter JOIN
Créer le channel si nécessaire.
Ajouter le client.
Broadcast le JOIN.
-> Objectif :
permettre aux users d’entrer dans des channels.

12.Implémenter PRIVMSG
Envoyer :
user → user,
user → channel.
Le serveur redistribue les messages.
-> Objectif :
faire fonctionner le chat IRC.

13.Implémenter le broadcast
Envoyer un message à :
tous les membres du channel,
sauf éventuellement l’émetteur.
-> Objectif :
distribuer les messages IRC.

14.Implémenter QUIT
Broadcast du départ.
Retirer le client :
des channels,
du serveur.
removeClient(fd).
-> Objectif :
gérer les déconnexions IRC proprement.

15.Implémenter PING / PONG
Le serveur envoie :
PING
Le client répond :
PONG
Sinon timeout.
-> Objectif :
vérifier qu’un client est toujours vivant.

16.Gérer les erreurs IRC
Nickname déjà utilisé.
Pas enregistré.
Channel inexistant.
Etc.
Envoyer les bons numeric replies IRC.
-> Objectif :
respecter le protocole IRC.

17.Tester avec un vrai client IRC
irssi
HexChat
WeeChat
Tester :
connexion,
JOIN,
messages,
QUIT,
erreurs.
-> Objectif :
vérifier que le serveur respecte le protocole IRC réel.