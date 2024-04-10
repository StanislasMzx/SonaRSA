#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>


void sendMessage(int sockfd, const char *message) {
    // Envoi du message au serveur
    if (send(sockfd, message, strlen(message), 0) < 0) {
        perror("Erreur lors de l'envoi du message au serveur");
        exit(EXIT_FAILURE);
    }
}


int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server IP address> <server port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int sockfd;
    struct sockaddr_in serv_addr;

    // Création du socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erreur lors de la création du socket");
        exit(EXIT_FAILURE);
    }

    // Initialisation de l'adresse du serveur
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

    // Connexion au serveur
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Erreur lors de la connexion au serveur");
        exit(EXIT_FAILURE);
    }

    printf("> Connected to \x1b[1m\x1b[35mManager %s:%s\x1b[0m\n\n", argv[1], argv[2]);
    printf("--------------------\n");
    printf("Welcome to \e[1mSonaRSA\e[m !\n");
    printf("--------------------\n\n\n");

    printf("Here are the available commands:\n");
    printf("  - \e[1mscan\e[m: Scan your network\n");
    printf("\t options :\n");
    printf("\t\t - \e[1m-a\e[m : auto scan\n");
    printf("\t\t - \e[1m-p ip1 ip2 ... ipn\e[m : scan a list of address\n");
    printf("  - \e[1mexit\e[m: Exit the program\n\n");

    
    int RUNNING = 1;

    while(RUNNING){
        char message[1024];
        printf("> enter your command: ");
        fgets(message, sizeof(message), stdin);

        // Supprimer le caractère de nouvelle ligne ajouté par fgets
        size_t len = strlen(message);
        if (len > 0 && message[len - 1] == '\n') {
            message[len - 1] = '\0';
            printf("Message envoyé : %s\n", message);
        }

        if (strcmp(message, "exit") == 0)
        {
            sendMessage(sockfd, message);
            RUNNING = 0;
        } else {
            // Envoyer le message au serveur
            sendMessage(sockfd, message);
        }
        
        // Envoyer le message au serveur
    }

   

    // Fermeture du socket
    close(sockfd);

    return 0;
}
