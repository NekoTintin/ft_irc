# ft_irc
IRC

# Sources

- Poll():
    https://bousk.developpez.com/cours/reseau-c++/TCP/05-envoi-reception-serveur/
    https://www.ibm.com/docs/fr/i/7.5.0?topic=designs-using-poll-instead-select
    


- Sockets:
    https://youtu.be/gntyAFoZp-E?si=g4ap0MMGicZ-UDn7


1. Initialiser le serveur ----> OK
   socket → bind → listen

2. Ajouter le socket serveur dans _pollfds ------> OK

3. Lancer la boucle infinie 

4. À chaque tour :
   poll(_pollfds)

5. Si activité sur le socket serveur :
   accepter une nouvelle connexion avec accept()

6. Pour ce nouveau client :
   - créer/stocker un Client
   - ajouter son fd dans _pollfds

7. Si activité sur un socket client :
   recv()

8. Si recv reçoit des données :
   les ajouter au buffer du client

9. Si recv retourne 0 :
   le client est déconnecté
   → close
   → remove de _pollfds
   → remove de _clients