#include <stdio.h>
#include <stdlib.h>
#include <string.h> //définit fonct manipulation string
#include <ctype.h>  //définit toupper() et tolower()
#include <stdbool.h> 
#include<time.h>

typedef char* string;

//definition structure machine
typedef struct machine{
	char name[50];
	char MacAddr[6];
	char IpAddr[4];
	char mask[4];
	bool state; //connecté ou !connecté
	string *logiciels_client; //liste logiciel client
	string *logiciels_server;
	//struct machine *next;	
}machine; 

//definition structure parc
typedef struct parc{
	machine* machine;
	struct parc* next;
}parc;

//fonction verifie nombre est un octet
bool isOctet(int x){
	if(x>=0 && x<256)
		return true;
	else
		return false;
}

//fonction qui récupere addresse mac
void MacSaisi(char mac[6]){
	int a, b, c, d, e, f, i;
	FILE *fichier;
	do{	
		i=scanf("%x:%x:%x:%x:%x:%x", &a, &b, &c, &d, &e, &f);
		if(i<6){
			puts("Adresse incorrecte! Veuillez entrer une bonne adresse!");
			while(getchar()!='\n');
				continue;
		}
		if(!(isOctet(a) && isOctet(b) && isOctet(c) && isOctet(d) &&isOctet(e) && isOctet(f)))
			puts("Adresse incorrecte! Veuillez entrer une bonne adresse!");
		else
			break;
	}while(1);
	mac[0]=(unsigned char)a;
	if((fichier= fopen("save.txt","a"))!=NULL){
		fprintf(fichier, "%X:",a);
		fclose(fichier);
	}
	mac[1]=(unsigned char)b;
	if((fichier= fopen("save.txt","a"))!=NULL){
		fprintf(fichier, "%X:",b);
		fclose(fichier);
	}
	mac[2]=(unsigned char)c;
	if((fichier= fopen("save.txt","a"))!=NULL){
		fprintf(fichier, "%X:",c);
		fclose(fichier);
	}
	mac[3]=(unsigned char)d;
	if((fichier= fopen("save.txt","a"))!=NULL){
		fprintf(fichier, "%X:",d);
		fclose(fichier);
	}
	mac[4]=(unsigned char)e;
	if((fichier= fopen("save.txt","a"))!=NULL){
		fprintf(fichier, "%X:",e);
		fclose(fichier);
	}
	mac[5]=(unsigned char)f;
	if((fichier= fopen("save.txt","a"))!=NULL){
		fprintf(fichier, "%X\n",f);
		fclose(fichier);
	}
}

//fonction qui récupère addresse Ip
void IpSaisi(char ip[4]){
	int a, b, c, d, i;
	FILE *fichier;
	do{
		i=scanf("%d.%d.%d.%d",&a,&b,&c,&d); //recupere addresse forma a.b.c.d
		if(i<4){ //verifie addresse complete
			puts("Adresse incorrecte! Veuillez entrer une bonne adresse!");
			while(getchar()!='\n');
				continue;
		}
		if(!(isOctet(a) && isOctet(b) && isOctet(c) && isOctet(d))) //verifie element est un octet
			puts("Adresse incorrecte! Veuillez entrer une bonne adresse!");
		else
			break;
	}while(1);
	ip[0]=(unsigned char)a;
	if((fichier= fopen("save.txt","a"))!=NULL){
		fprintf(fichier, "%d.",a);
		fclose(fichier);
	}
	ip[1]=(unsigned char)b;
	if((fichier= fopen("save.txt","a"))!=NULL){
		fprintf(fichier, "%d.",b);
		fclose(fichier);
	}
	ip[2]=(unsigned char)c;
	if((fichier= fopen("save.txt","a"))!=NULL){
		fprintf(fichier, "%d.",c);
		fclose(fichier);
	}
	ip[3]=(unsigned char)d;
	if((fichier= fopen("save.txt","a"))!=NULL){
		fprintf(fichier, "%d\n",d);
		fclose(fichier);
	}
}


//fonction initialiser machine		
machine *initMachine(){
	machine *m= malloc(sizeof(machine));
	int i;
	FILE *fichier;
	if(m!=NULL){
		//récupère nom machine
		char name[50];
		printf("Entrer le nom de la machine:\t");
		scanf("%s",name);
		//enregistrement du nom de la machine dans le fichier de sauvegarde
		if((fichier= fopen("save.txt","a"))!=NULL){
			fputs("\n********* MACHINE *********\n",fichier);
			fprintf(fichier, "Hostname: %s\n",name);
			fclose(fichier);
		}
		strcpy(m->name, name);	//copie nom machine

		//initialise état à false
		m->state=false;
		
		//initialiser address mac
		printf("Entrer l'addresse Mac(format: x:x:x:x:x:x)\t");
		if((fichier= fopen("save.txt","a"))!=NULL){
			fputs("MAC Address: ", fichier);
			fclose(fichier); 
		}
		MacSaisi(m->MacAddr);
		
		//alloue espace pour tableau de string
		m->logiciels_client=(string*) malloc(sizeof(string));
		if (m->logiciels_client==NULL)
			return NULL;
		
		//alloue espace pour tableau de string
		m->logiciels_server=(string*) malloc(sizeof(string));
		if (m->logiciels_server==NULL)
			return NULL;		
	}
	    return m;
}

//ajouter machine au parc
parc *addMachine(parc *list){
	parc *p=malloc(sizeof(parc));	
	if(p!=NULL){
		p->machine=initMachine();//initialiser machine
		p->next=NULL;
	}
	if(list == NULL)	//verifie si parc contient aucun machine
		list=p;
	else{ //sinon
		parc *tmp=list;	//pointeur sur list machine
		while(tmp->next!=NULL)	//verifie prochain machine existe
			tmp=tmp->next;
		tmp->next=p;
		tmp=tmp->next;
	}
	return list;
}

//fonction qui affiche nom machine
void afficheName(machine *m){
	printf("HostName:\t");
	printf("%s",m->name);
	puts("");
}

//fonction affiche liste logiciel client
void afficheLogicielClient(machine *m){
	string* client=m->logiciels_client;
	printf("Liste Logiciels clients installés sur cette machine: ");
	for(int j=0; client[j]!=NULL; j++){
		printf("%s",client[j]);
		if(client[j+1]!=NULL)
			printf("; ");
	}
	puts("");
}

//fonction affiche liste logiciel serveur
void afficheLogicielServer(machine *m){
	string* serveur=m->logiciels_server;
	printf("Softwares installed in this machine: ");
	for(int j=0; serveur[j]!=NULL; j++){
		printf("%s",serveur[j]);
		if(serveur[j+1]!=NULL)
			printf("; ");
	}
	puts("");
}

//fonction affiche addresse MAC

void afficheMacAddr(machine *m){
	printf("MAC Address:\t");
	unsigned char *mac=m->MacAddr;
	for(int i=0; i<6; i++){
		printf("%x", mac[i]);
		if(i<5)
			printf(":");
	}
	puts("");
}

//fonction affiche addresse IP
void afficheIpAddr(machine *m){
	printf("IP Address:\t");
	unsigned char *ip= m->IpAddr;
	for(int i=0; i<4; i++){
		printf("%d", ip[i]);
		if(i<3)
			printf(".");
	}
	puts("");
}

//fonction affiche Mask réseau
void afficheMask(machine *m){
	printf("Mask:\t");
	unsigned char *mask= m->mask;
	for(int i=0; i<4; i++){
		printf("%d", mask[i]);
		if(i<3)
			printf(".");
	}
	puts("");
}

//fonction affiche état
void afficheState(machine *m){
	printf("State:\t");
	if(m->state)
		printf("CONNECTED!");
	else
		printf("NOT CONNECTED!");
	puts("");
}

//fonction affiche machine
void afficheMachine(machine *m){
	puts("-----------------------------------------------------------------------------");
	afficheName(m);
	afficheLogicielClient(m);
	afficheLogicielServer(m);
	afficheMacAddr(m);
	afficheIpAddr(m);
	afficheMask(m);
	afficheState(m);
	puts("-----------------------------------------------------------------------------");
}

//afficher parc
void afficheParc(parc *p){
	puts("____START____");
	// for(parc *tmp=p; tmp!=NULL; tmp=tmp->next)
	// 	afficheMachine(tmp->machine);
	system("cat save.txt");
	puts("\n____END____");
}
	
//fonction recherche machine
machine *searchMachine(string hostname, parc *p){
	machine *m=NULL;
	for (parc *tmp=p; tmp!=NULL; tmp=tmp->next){
		if((strcmp(tmp->machine->name, hostname))==0){//comparer les noms machines 
			m=tmp->machine;
			break;
		}
	}
	return m;
}

//fonction ajouter logiciel à la liste
void addLogiciel(string *list_logiciel){
	char logiciel[15];
	FILE *fichier;
	printf("Enter the software you want to install :\t");
	scanf("%s", logiciel);
	int i=0;
	while(list_logiciel[i]!=NULL)
		i++;
	list_logiciel[i]=(string)malloc(sizeof(strlen(logiciel)+1)); //Alloue espace correspondant à la taille de l'entrée
	if(list_logiciel[i]!=NULL){
		if(i==0) //vérifie si premier logiciel de la liste
			strcpy(list_logiciel[i], logiciel);	//copie le logiciel dans la liste
		else{
			list_logiciel=realloc(list_logiciel, (i+1)*sizeof(string)); //réalloue espace mémoire
			strcpy(list_logiciel[i], logiciel);
		}
		if((fichier= fopen("save.txt","a"))!=NULL){
		fprintf(fichier,"%s,	", logiciel);
		fclose(fichier);
		}
	}
	if((fichier= fopen("save.txt","a"))!=NULL){
		fputs("\n",fichier);
		fclose(fichier);
	}
}

//fonction qui vérifie si logiciel existe
bool existLogiciel(string logiciel, string *list_logiciel){
	bool exist=false;
	int i=0;
	while(list_logiciel[i]!=NULL){
		if(strcmp(list_logiciel[i], logiciel)==0){
			exist=true;
			break;
		}
		i++;
	}
	return exist;
}

//fonction supprimer logiciel
void deleteLogiciel(string *list_logiciel){
	char logiciel[15];
	printf("Enter the name of the software you want to delete\t");
	scanf("%s", logiciel);
	if(existLogiciel(logiciel, list_logiciel)){//vérifie logiciel existe
		int i=0;
		while(list_logiciel[i]!=NULL){
			if(strcmp(list_logiciel[i],logiciel)==0){
				for(int j=i; list_logiciel[j]!=NULL; j++)
					list_logiciel[j]=list_logiciel[j+1]; //décalage
			}
			i++;
		}
		printf("\"%s\" SUCCESSFULLY DELETED !!!\n",logiciel); 
	}
	else
		printf(" Sorry but \"%s\" is not installed in this machine !\n",logiciel); 
}

//fonction install logiciel client
void installLogicielClient(machine *m){
	addLogiciel(m->logiciels_client);
	puts("SUCCESSFULLY INSTALLED !!!");
}

//fonction install logiciel serceur
void installLogicielServer(machine *m){
	addLogiciel(m->logiciels_server);
	if(existLogiciel("dhcp", m->logiciels_server) && !m->state){ //si on installe serveur dhcp installation
		//initialiser address Ip
		printf("Enter an IP Address (format: a.b.c.d)\n");
		IpSaisi(m->IpAddr);

		//initialiser mask
		printf("Enter a subnet mask (format: a.b.c.d)\n");
		IpSaisi(m->mask);
		m->state=true;
		printf("THE MACHINE  %s  HAS BEEN SUCCESSFULLY CONNECTED !\n", m->name);
		puts("INSTALLATION COMPLETED !!!");
	}
	else
		puts("INSTALLATION COMPLETED !!!");
}

//fonction désinstaller logiciel client
void desinstallLogicielClient(machine *m){
	deleteLogiciel(m->logiciels_client);
}

//fonction desinstaller logiciel serveur
void desinstallLogicielServer(machine *m){
	deleteLogiciel(m->logiciels_server);
}
	
//fonction compare addresse ip
bool ipCmp(char ip1[4], char ip2[4]){
	bool same=true;
	for(int i=0; i<4; i++){
		if(ip1[i]!=ip2[i]){
			same=false;
			break;
		}
	}
	return same;
}

//fonction recherche addresse ip dans le réseau
machine *searchReseau(char ip[4], parc *p){
	machine *m=NULL;
	parc *tmp=p;
	while(tmp!=NULL){
		if(ipCmp(ip, tmp->machine->IpAddr)){
			m=tmp->machine;
			break;
		}
		tmp=tmp->next;
	}
	return m;
}

//fonction qui cherche adresse Rx
void ipRx(char ip[4], char mask[4], char ipReseau[4]){
	for(int i=0; i<4; i++)
		ipReseau[i]=ip[i] & mask[i]; //ET logique entre les différents bits de l'adresse ip et du masque
}

//fonction qui cherche broadcast
void ipBrd(char ip[4], char mask[4], char brcst[4]){
	char ipReseau[4];
	//char *mask_bar;
	ipRx(ip, mask, ipReseau);
	/*for(int i=0; i<4; i++)
		mask[i]=~mask[i];*/	//complément à 1 masque
	for(int i=0; i<4; i++)
		brcst[i]=(~mask[i]) | ipReseau[i];	//ou inclusif entre cà1 mask et @ Rx
}

//fonction dhcp
void dhcp(char ip_client[4], char mask_client[4], machine *serveur, parc *p){
	unsigned char ipReseau[4];
	unsigned char brdcst[4];
	machine *m=NULL;
	FILE *fichier;
	ipRx(serveur->IpAddr, serveur->mask, ipReseau); //@ Rx
	ipBrd(serveur->IpAddr, serveur->mask, brdcst); //@ brd
	unsigned char *mask=serveur->mask;	//pointeur sur masque serveur
	int i=0;
	puts("CONNECTION TO DHCP SERVER... WELL ESTABILISHED\n");
	puts("ADDRESS REQUEST IN PROCESS...\t");
	if ((fichier=fopen("save.txt","a"))!=NULL){
		fputs("IP Address: ",fichier);
		fclose(fichier);
	}
	while(mask[i]==255){//verifier si mask =255 octet @client= octet @ Rx
		ip_client[i]=ipReseau[i];
		if ((fichier=fopen("save.txt","a"))!=NULL){
			fprintf(fichier,"%d.", ip_client[i]);
			fclose(fichier);
		}
		i++;
		//printf("%d\n", ip_client[i]);
	}
	int a=i;
	do{
		int j;
		if(i<4)
			j= brdcst[i]-ipReseau[i]; //plage @ hote
		//printf("j=%d\n",j);
		ip_client[i]=ipReseau[i]+1+rand()%j; //choix au hasard
		if ((fichier=fopen("save.txt","a"))!=NULL){
			fprintf(fichier,"%d.", ip_client[i]);
			fclose(fichier);
		}
		i++;
		if(i==4){
			m=searchReseau(ip_client, p);	//verifie si @ disponible
			i=a; //réinitialise i avant boucle do while
		}
	}while(m==NULL);
	
	puts("SUCCESSFULL !!!\n");
	puts("SUBNET MASK REQUEST IN PROCESS...\n");
	//initialise mask
	if ((fichier=fopen("save.txt","a"))!=NULL){
		fputs("Subtnet Mask: ",fichier);
		fclose(fichier);
	}
	for(i=0; i<4; i++)
		mask_client[i]=mask[i];
	if ((fichier=fopen("save.txt","a"))!=NULL){
		if(i!=4){
		fprintf(fichier,"%d.", mask_client[i]);
		fclose(fichier);
		}
		else{
		fprintf(fichier,"%d",mask_client[i]);
		fclose(fichier);
		}
	}
	puts("OKAY !");
}

//fonction cherche serveur dhcp
machine *serveurDhcp(parc *p){
	machine *serveur=NULL;
	parc *tmp=p;
	while(tmp!=NULL){
		if(existLogiciel("dhcp",tmp->machine->logiciels_server))
			serveur=tmp->machine;
		tmp=tmp->next;
	}
	return serveur;
}
//fonction add machine to network
void connectMachine(string hostname, parc *p){
	machine *m=searchMachine(hostname, p);
	machine *serveur=serveurDhcp(p);
	FILE *fichier;
	int choix;
	if(m==NULL)
		printf("SORRY BUT THE MACHINE  %s  DOES NOT EXIST IN THE INFRASTRUCTURE!\n", hostname);
	else if(serveur==NULL){
		puts("DHCP SERVER NOT AVAILABLE \n");
		printf("Tap 1 for a static address or else for exit\n");
		scanf("%d", &choix);
		//renseigner @ statique
		if(choix==1){
			//initialiser address Ip
			printf("Enter an IP Address (format: a.b.c.d)\n");
			if ((fichier=fopen("save.txt","a"))!=NULL){
				fputs("IP Address: ",fichier);
				fclose(fichier);
			}		
			IpSaisi(m->IpAddr);

			//initialiser mask
			printf("Enter the Subnet Mask (format: a.b.c.d)\n");
			if ((fichier=fopen("save.txt","a"))!=NULL){
				fputs("Subtnet Mask: ",fichier);
				fclose(fichier);
			}
			IpSaisi(m->mask);
			m->state=true;
			if ((fichier=fopen("save.txt","a"))!=NULL){
				fputs("State: CONNECTED\n",fichier);
				fclose(fichier);
			}
			printf("THE MACHINE  %s   HAS BEEN WELL CONNECTED !\n", hostname);
				
		}
		else
			puts("BYE");
		
	}
	//utiliser serveur dhcp
	else{
		puts("DHCP SERVER AVAILABLE !\n");
		printf("1: obtain an address dynamically \n2: enter a static address \n else: quit\n");
		scanf("%d", &choix);
		switch(choix){
			case 1:
				puts("DHCP SERVER CONNECTION TRIAL ...\n");
				
				dhcp(m->IpAddr, m->mask, serveur, p);
				
				m->state=true;
				if ((fichier=fopen("save.txt","a"))!=NULL){
					fputs("State: CONNECTED\n",fichier);
					fclose(fichier);
				}
				printf("THE MACHINE  %s  HAS BEEN SUCCESSFULLY CONNECTED !\n", hostname);
				break;
			case 2:
				//initialiser address Ip
				printf("Entrer une adresse Ip(format: a.b.c.d)\t");
				IpSaisi(m->IpAddr);

				//initialiser mask
				printf("Entrer le masque de réseau(format: a.b.c.d)\t");
				IpSaisi(m->mask);
				m->state=true;
				if ((fichier=fopen("save.txt","a"))!=NULL){
					fputs("State: CONNECTED\n",fichier);
					fclose(fichier);
				}
				printf("THE MACHINE  %s  HAS BEEN WELL CONNECTED !\n", hostname);
				break;
			default:
				puts("BYE");
		}
	}
}

//foction delete machine to network
void disconnectMachine(string hostname, parc *p){
	machine *m=searchMachine(hostname, p);
	if(m==NULL)
		printf("SORRY BUT THE MACHINE  %s  DOES NOT EXIST !\n", hostname);
	else{
		char *ip=m->IpAddr;
		for(int i=0; i<4; i++)
			ip[i]=0;
		char *mask=m->mask;
		for(int i=0; i<4; i++)
			mask[i]=0;
		m->state=false;
		printf("THE MACHINE  %s  HAS BEEN WELL DISCONNECTED !\n", hostname);
	}
}	

//fonction ping
void ping(char ip[4], machine *m1, parc *p){
	machine *m2=searchReseau(ip, p);
	unsigned char ipReseau1[4];
	unsigned char ipReseau2[4];
	if(m2!=NULL && m1->state && m2->state){
		ipRx(m1->IpAddr, m1->mask, ipReseau1);
		ipRx(m2->IpAddr, m2->mask, ipReseau2);
		if(ipCmp(ipReseau1, ipReseau2)){
			for(int i=0; i<4; i++)
				printf("%d packet transmitted \n",i);
				puts("\nPing SUCCESSED !!!\n PING STATISTICS ----\n 4 packets transmitted, 4 received, 0%% packet loss ");
		}
		else{
			for(int i=0; i<4; i++)
				printf("%d packet transmitted \n",i);
				puts("\nPing FAILED !!!\n PING STATISTICS ----\n 4 packets transmitted, 0 received, 100%% packet loss ");
		}
	}
	else{
		for(int i=0; i<4; i++)
				puts("\nADDRESS UNREACHABLE !!!\n PING STATISTICS ----\n 4 packets transmitted, 0 received, 100%% packet loss ");	
		}
}	
//fonction suprimer machine dans le parc
parc *deleteMachine(string hostname, parc *p){
	machine *m =searchMachine(hostname, p);
	if(m==NULL)
		printf("SORRY BUT THE MACHINE  %s   DOES NOT EXIST !\n", hostname);
	else{
		parc *tmp= p;
		while(tmp!=NULL){
			if(tmp->machine->name==m->name){
				for(parc *t=tmp; t!=NULL; t=t->next){
					if(t->next!=NULL)
						t->machine=t->next->machine;
				}
			}
			tmp=tmp->next;
		}
		for(parc *tmp=p; tmp !=NULL; tmp=tmp->next){
			if(tmp->next->next==NULL)
				tmp->next=NULL;
		}
		
		printf("THE MACHINE   %s   HAS BEEN SUCCESFULLY DELETED !\n", hostname);
	}
	return p;
}
				

int main(void){
	parc *list=NULL; //list des machines
	machine *m=NULL;
    	int option, stop=1;
	char hostname[50];
	srand(time(NULL));
	system("clear");
	puts("\t\t **************************************************\n");
	puts("\t\t WELCOME TO OUR C LANGUAGE PROGRAMMATION PROJECT\n");
	puts("\t\t 					DESIGNED BY \n");
	puts("\t\t 		--> FAMARA BADJI\n");
	puts("\t\t 		--> FATOUMATA LAMARANA BARRY\n");
	puts("\t\t 		--> HAWA MAMADOU DIA\n");
	puts("\t\t 		--> MOHAMADOU LAMINE FALL\n");
	puts("\t\t 		--> FATOU MBOW\n");
	puts("\t\t 	 	--> ASTOU THIOYE\n");
	puts("\t\t **************************************************\n\n\n");

	puts("THIS IS ABOUT A SIMULATION OF A COMPUTER INFRASTRUCTURE  WITH SOME FEATURES\n LIKE SOFTWARE INSTALLATION, DHCP REQUEST, PING...\n\n ");

	do{

		puts("******************************************************************************\n");
		puts("1: Add a machin\n");
		puts("2: Global view of the infrastructure\n");
		puts("3: Search/view/ping a machine\n");
		puts("4: Delete a machine \n");
		puts("5: Install/Uninstall a Client/Server Software\n");
		puts("6: Connect a machine to the network\n");
		puts("7: Disconnect a machine from the network\n");
		puts("******************************************************************************\n");
		printf("PLEASE MAKE A CHOICE\n");
		scanf("%d", &option);
		switch(option){
			case 1:
				system("clear");
				list=addMachine(list);
				break;
			case 2:
				system("clear");
				afficheParc(list);
				break;
			case 3:
				system("clear");				
				printf("Enter the name of the machine you want to ad.d :\n");
				scanf("%s", hostname);
				m =searchMachine(hostname, list);
				if(m!=NULL){
					system("clear");
					afficheMachine(m);
					int i;
					puts("1: Ping");
					puts("2: Quit");
					scanf("%d", &i);
					if(i==1){
						char ip[4];
						printf("Which Address do you want to to reach ? (format a.b.c.d)\n");
						IpSaisi(ip);
						ping(ip, m, list);
						break;
					}
					else break;
				}				
				else
					printf("SORRY BUT THE MACHINE  %s  DOES NOT EXIST !\n", hostname);
				break;
			case 4:
				system("clear");
				printf("Enter the name of the machine you want to delete:\n");
				scanf("%s", hostname);
				list=deleteMachine(hostname, list);
				break;
			case 5:
				system("clear");
				printf("Enter the name of the machine in which you want to install/uninstall: \n");
				scanf("%s", hostname);
				m=searchMachine(hostname, list);
				if(m==NULL)
					printf("SORRY BUT THE MACHINE  %s  DOES NOT EXIST !\n", hostname);
				else{
					system("clear");					
					afficheMachine(m);
					puts("1: INSTALL a client software\n");
					puts("2: INSTALL a server software\n");
					puts("3: UNINSTALL a client software\n");
					puts("4: UNINSTALL a server software");
					int i;
					printf("Votre choix: ");
					scanf("%d", &i);
					switch(i){
						case 1:
							system("clear");
							installLogicielClient(m);
							break;
						case 2:
							system("clear");
							installLogicielServer(m);
							break;
						case 3:
							system("clear");
							desinstallLogicielClient(m);
							break;
						case 4:
							system("clear");
							desinstallLogicielServer(m);
							break;
						default:
							printf("OPTION NOT AVAILABLE\n");
					}
					
				}
				break;
						
			case 6:
				system("clear");
				printf("Enter the name of the machine that you want to connect:\n");
				scanf("%s", hostname);
				connectMachine(hostname, list);
				break;
			case 7:
				system("clear");
				printf("Enter the name of the machine that you want to disconnect:\n");
				scanf("%s", hostname);
				disconnectMachine(hostname, list);
				break;
			default:
				system("clear");
				puts("OPTION NOT AVAILABLE !");
				continue;
		}
		printf("DO YOU WANT TO CONTINUE ? 1:YES\t 2:NO\t");
		scanf("%d",&stop);
	}while(stop==1);
    	while (list != NULL){
		parc *tmp = list->next;
		free(list);
		list = tmp;
	}
	//printf("%s\n", list->machine->name);
	
	return 0;
}
