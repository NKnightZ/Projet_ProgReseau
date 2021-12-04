/* cree un server : ncat -l 123456 (ici le port = 123456)
cree client : nc localhost 123456

avec wireshark : tcp port == 123456 and tcp

netstat -nato
*/

// fwrite(&s, sizeof(s), 1, f) s: struct, f: client
// fread(&s, sizeof(s), f)
// a la fin fflush(f)

// struture donnee du client
// structure donnee du client sur le server account par exemple
// structure des paquets qu'on envoie

//client:
// envoie de 3 paquet
// rembourser
// depenser
// voir l'etat des comptes

// paquet du server : savoir l'etat des comptes par le server

// pas de creation de compte, pas rejoindre un compte de server

// pas d'interface graphique

// tri paul rembourse marc 25 ENTER
// tri toto depense 25 -> toto va depense 25€ 
// tri toto state