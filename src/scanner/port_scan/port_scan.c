#include "port_scan.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

void port_scan(const char *host, int start_port, int end_port)
{
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host, NULL, &hints, &res) != 0)
    {
        fprintf(stderr, "Failed to resolve hostname\n");
        return;
    }

    pid_t child_pids[(end_port - start_port) + 1];
    int i = 0;

    for (int port = start_port; port <= end_port; port++)
    {
        pid_t pid = fork();

        if (pid == -1)
        {
            perror("fork");
            return;
        }
        else if (pid == 0)
        {
            // Child process
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)res->ai_addr;
            ipv4->sin_port = htons(port);

            int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
            if (sockfd == -1)
            {
                perror("socket");
                exit(1);
            }

            // Set a timeout on the socket connection
            struct timeval tv;
            tv.tv_sec = 1; // 1 second timeout
            tv.tv_usec = 0;
            if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof tv) < 0)
            {
                perror("setsockopt");
                exit(1);
            }

            if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1)
            {
                close(sockfd);
                exit(1);
            }

            printf("Port %d is open\n", port);
            close(sockfd);
            exit(0);
        }
        else
        {
            // Parent process
            child_pids[i++] = pid;
        }
    }

    for (int j = 0; j < i; j++)
    {
        waitpid(child_pids[j], NULL, 0);
    }

    freeaddrinfo(res);
}

void port_scan_1000(const char *host)
{
    FILE *file = fopen("src/scanner/port_scan/top-1000-most-popular-tcp-ports-nmap-sorted.csv", "r");
    if (file == NULL)
    {
        perror("fopen");
        return;
    }

    char line[8192];
    char *token;
    const char *delimiter = ",";

    pid_t child_pids[1000];

    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "\n")] = '\0';

        token = strtok(line, delimiter);
        while (token != NULL)
        {
            int port = atoi(token);

            pid_t pid = fork();

            if (pid == -1)
            {
                perror("fork");
                return;
            }
            else if (pid == 0)
            {
                port_scan(host, port, port);
                exit(0);
            }
            else
            {
                // Parent process
                waitpid(pid, NULL, 0);
            }

            token = strtok(NULL, delimiter);
        }
    }

    for (int i = 0; i < 1000; i++)
    {
        waitpid(child_pids[i], NULL, 0);
    }

    // Close the file
    fclose(file);
}

int main(void)
{
    port_scan_1000("scanme.nmap.org");
    return 0;
}
