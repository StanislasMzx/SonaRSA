#include "ip_scan.h"
#include "ping.h"
#include <stdio.h>

int main(void)
{
    // ask user for an IP address
    char ip[16];
    printf("Enter an IP address: ");
    scanf("%s", ip);
    // call the ping function
    if (ping(ip) == 0)
    {
        printf("Host is reachable\n");
    }
    else
    {
        printf("Host is unreachable\n");
    }
    /* code */
    return 0;
}
