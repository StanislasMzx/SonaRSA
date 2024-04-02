#ifndef IP_SCAN_H
#define IP_SCAN_H

#include <netinet/in.h>

/**
 * @brief Ping a target_host with ICMP protocol
 *
 * @param target_host IP address of the host to ping
 * @param len Length of the ip_addrs array
 */
void ping_sweep(in_addr_t ip_addrs[], int len);

#endif /* IP_SCAN_H */
