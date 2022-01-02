Sujet du projet: **Tricount**

pour compiler taper la commande `make`

pour lancer le serveur: 
```sh
./server
```

pour lancer le client il y a 3 commandes i.e le client peut envoyer 3 paquets: 
- faire une depense: 
```sh
./tricount user spend amount
``` 
- faire un remboursement: 
```sh
./tricount user1 refund user2 amount 
```
- voir l'etat des comptes: 
```sh
./tricount state
```

une fois un client accepté on a la possibilité d'arreter le serveur en tapant la commande `stop`
