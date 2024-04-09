#include "ip_scan.h"
#include "ping.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <string.h>
#include "../../../libs/log/log.h"

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

void _ping_sweep_subnet(in_addr_t network, in_addr_t mask)
{
    // Compute the number of hosts in the subnet
    in_addr_t broadcast = network | ~mask;
    int num_hosts = ntohl(broadcast) - ntohl(network) + 1;

    // Allocate memory for the array of IP addresses
    in_addr_t *ip_addrs = malloc((num_hosts - 2) * sizeof(in_addr_t));
    if (!ip_addrs)
    {
        fprintf(stderr, "Failed to allocate memory\n");
        return;
    }

    // Generate the list of IP addresses in the subnet
    int j = 0;
    for (int i = 1; i < num_hosts - 1; i++, j++)
    {
        ip_addrs[j] = (network & mask) + htonl(i);
    }

    // Call the ping_sweep function with the list of IP addresses
    ping_sweep(ip_addrs, num_hosts - 2);

    free(ip_addrs);
}

void ping_sweep_subnet(void)
{
    struct ifaddrs *ifap, *ifa;
    int found = 0;
    getifaddrs(&ifap);
    for (ifa = ifap; ifa; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET)
        {
            // Skip loopback interface
            if (strcmp(ifa->ifa_name, "lo") == 0 || strcmp(ifa->ifa_name, "lo0") == 0)
            {
                continue;
            }
            // Found non-loopback interface
            if (!found)
            {
                struct sockaddr_in *sa = (struct sockaddr_in *)ifa->ifa_addr;
                log_info("Interface: %s", ifa->ifa_name);
                log_info("IP address: %s", inet_ntoa(sa->sin_addr));
                log_info("Netmask: %s", inet_ntoa(((struct sockaddr_in *)ifa->ifa_netmask)->sin_addr));
                _ping_sweep_subnet(sa->sin_addr.s_addr, ((struct sockaddr_in *)ifa->ifa_netmask)->sin_addr.s_addr);
                found = 1;
                break;
            }
        }
    }
    freeifaddrs(ifap);
}
