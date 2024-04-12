#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../scanner/ip_scan/ip_scan.h"
#include <unistd.h>

#define SERV_PORT 2222
#define BUFFER_SIZE 2000000

int main(void)
{

    struct sockaddr_in serv_addr;
    printf("-----------------------\n");
    printf("Running the \x1b[1m\x1b[35mTCP Manager\x1b[0m\n");
    printf("-----------------------\n\n");
    int server_soc;
    if ((server_soc = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket");
        exit(1);
    }
    printf("\x1b[33m\x1b[1m[LOG]\x1b[43m\x1b[0m Socket created\n");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(SERV_PORT);

    if (bind(server_soc, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("servecho: erreur bind\n");
        exit(1);
    }
    printf("\x1b[33m\x1b[1m[LOG]\x1b[43m\x1b[0m Socket binded\n");

    /*
    Paramétrer le nombre de connexion "pending"
    */
    if (listen(server_soc, SOMAXCONN) < 0)
    {
        perror("servecho: erreur listen");
        exit(1);
    }
    printf("\x1b[33m\x1b[1m[LOG]\x1b[43m\x1b[0m Server listening on port 2222\n");

    int dialogSocket;
    int clilen;
    struct sockaddr_in cli_addr;

    clilen = sizeof(cli_addr);

    dialogSocket = accept(server_soc, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);

    if (dialogSocket < 0)
    {
        perror("servecho : erreur accept\n");
        exit(1);
    }
    printf("\x1b[33m\x1b[1m[LOG]\x1b[43m\x1b[0m Connection accepted\n");

    char hello[100] = "bonjour\n\0";
    send(dialogSocket, hello, strlen(hello), 0);

    int RUNNING = 1;

    while (RUNNING)
    {
        char buffer[1024];
        int n = recv(dialogSocket, buffer, sizeof(buffer) - 1, 0);
        buffer[n] = '\0';
        if (n > 0)
        {
            printf("\x1b[34m\x1b[1m[RECEIVING]\x1b[44m\x1b[0m\n> %s\n", buffer);
            if (strcmp(buffer, "scan -a") == 0)
            {
                ping_sweep_subnet();
            }

            if (strncmp(buffer, "scan -p", 7) == 0)
            {
                // Store IP addresses in a table
                char *token = strtok(buffer + 7, " ");
                int numAddresses = 0;
                in_addr_t ipAddresses[10]; // Assuming a maximum of 10 IP addresses

                while (token != NULL && numAddresses < 10)
                {
                    ipAddresses[numAddresses] = inet_addr(token);
                    numAddresses++;
                    token = strtok(NULL, " ");
                }
                for (int i = 0; i < numAddresses; i++)
                {
                    printf("IP Address %d: %s\n", i + 1, inet_ntoa(*(struct in_addr *)&ipAddresses[i]));
                }
                printf("log : [scan] %d addresses\n", numAddresses);
                ping_sweep(ipAddresses, numAddresses);
            }

            if (strcmp(buffer, "exit") == 0)
            {
                printf("\x1b[33m\x1b[1m[LOG]\x1b[43m\x1b[0m Exit\n");
                RUNNING = 0;
            }
            else
            {
                send(dialogSocket, buffer, strlen(buffer), 0);
            }
        }
    }
    close(server_soc);
}
