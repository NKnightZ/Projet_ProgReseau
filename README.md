Sujet du projet: **Tricount**

pour compiler taper la commande `make`

pour lancer le serveur: `./server`

pour lancer le client il y a 3 commandes i.e le client peut envoyer 3 paquets: 
- faire une depense: `./tricount user spend amount`
par exemple: 
```sh
./tricount Damien spend 20
```
- faire un remboursement: `./tricount user1 refund user2 amount`
par exemple: 
```sh
./tricount barry refund Damien 5
```
- voir l'etat des comptes: `./tricount state`
