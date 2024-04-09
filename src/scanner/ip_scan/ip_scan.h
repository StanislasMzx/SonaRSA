#ifndef IP_SCAN_H
#define IP_SCAN_H

#include <netinet/in.h>

/**
 * @brief Ping a target_host with ICMP protocol
 *
 * @param ip_addrs IP address of the host to ping
 * @param len Length of the ip_addrs array
 */
void ping_sweep(in_addr_t ip_addrs[], int len);

/**
 * @brief Ping a subnet with ICMP protocol
 *
 * @param network IP address of the network to ping
 * @param mask Subnet mask of the network to ping
 */
void _ping_sweep_subnet(in_addr_t network, in_addr_t mask);

/**
 * @brief Ping a subnet with ICMP protocol using the default gateway
 */
void ping_sweep_subnet(void);

#endif /* IP_SCAN_H */
