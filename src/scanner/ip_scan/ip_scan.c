#include "ip_scan.h"
#include "ping.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <arpa/inet.h>

void ping_sweep(in_addr_t ip_addrs[], int len)
{
    pid_t child_pids[len];

    for (int i = 0; i < len; i++)
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
            if (ping(ip_addrs[i]) == 0)
            {
                printf("Host %s is reachable\n", inet_ntoa(*(struct in_addr *)&ip_addrs[i]));
            }
            exit(0);
        }
        else
        {
            // Parent process
            child_pids[i] = pid;
        }
    }

    for (int i = 0; i < len; i++)
    {
        waitpid(child_pids[i], NULL, 0);
    }
}
