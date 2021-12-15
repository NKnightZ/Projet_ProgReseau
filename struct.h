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


// faire de l'UDP, (et sinon mettre un compteur de paquet pour verifier qu'il n'y a pas de paquet perdu ça sera un bonus) 
// ne pas oublier fflush apres un fwrite pour forcer l'envoie

/* void init_account(struct account *a1, struct user *u1){
    printf("To start you must to create a account\n");
    
    //struct account a1;
    printf("enter the title of account: \n");
    scanf("%s", a1->title);
    a1->user_account = *u1;
    char res;
    printf("Do you want to enter the description of your account: y/n\n");
    do{
        scanf("%c", &res);
        if(res == 'y'){
            printf("Enter the description please (140 characters max): \n");
            scanf("%s", a1->description);
            break;
        }else if(res == 'n'){
            printf("okay, your description will be empty, if you want to change your description later you can do it in the menu \n");
            break;
        }else{
            printf("invalid input, please enter 'y' or 'n'\n");
        }
    }while(res != 'y' || res != 'n');
    printf("Done ! Your account has been successfully created\n");
} */
