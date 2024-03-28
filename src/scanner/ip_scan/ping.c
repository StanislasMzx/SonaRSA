#include "ping.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>

#define PACKET_SIZE 64
#define TIMEOUT_SEC 1
#define TIMEOUT_USEC 0

int ping(char *target_host)
{
    struct hostent *host_info;
    struct sockaddr_in dest_addr;

    // Get host entry
    if ((host_info = gethostbyname(target_host)) == NULL)
    {
        herror("gethostbyname");
        return 1;
    }

    // Fill in destination address
    memcpy(&dest_addr.sin_addr, host_info->h_addr_list[0], host_info->h_length);
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = 0;

    // Create ICMP socket
    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock == -1)
    {
        perror("socket");
        return 1;
    }

    // Allocate memory for packet
    char packet[PACKET_SIZE];
    memset(packet, 0, PACKET_SIZE);

    // Fill in ICMP header
    struct icmp *icmp_hdr = (struct icmp *)packet;
    icmp_hdr->icmp_type = ICMP_ECHO;
    icmp_hdr->icmp_code = 0;
    icmp_hdr->icmp_id = getpid();
    icmp_hdr->icmp_cksum = 0;

    // Calculate checksum
    icmp_hdr->icmp_cksum = calculate_checksum((unsigned short *)icmp_hdr, PACKET_SIZE);

    // Send packet
    if (sendto(sock, packet, PACKET_SIZE, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)) == -1)
    {
        perror("sendto");
        close(sock);
        return 1;
    }

    // Receive response
    struct sockaddr_in recv_addr;
    socklen_t recv_addr_len = sizeof(recv_addr);
    char recv_packet[PACKET_SIZE];
    struct timeval timeout;
    timeout.tv_sec = TIMEOUT_SEC;
    timeout.tv_usec = TIMEOUT_USEC;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    if (recvfrom(sock, recv_packet, PACKET_SIZE, 0, (struct sockaddr *)&recv_addr, &recv_addr_len) == -1)
    {
        close(sock);
        return 1;
    }

    close(sock);
    return 0;
}

unsigned short calculate_checksum(unsigned short *buf, int len)
{
    unsigned int sum = 0;
    for (int i = 0; i < len / 2; i++)
    {
        sum += buf[i];
    }

    if (len % 2)
    {
        sum += ((char *)buf)[len - 1];
    }

    while (sum >> 16)
    {
        sum = (sum & 0xffff) + (sum >> 16);
    }

    return ~sum;
}
