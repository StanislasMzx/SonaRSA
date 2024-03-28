#ifndef PING_H
#define PING_H

/**
 * @brief Ping a target_host with ICMP protocol
 *
 * @param target_host IP address of the host to ping
 * @return int 0 if host is reachable else 1
 */
int ping(char *target_host);

/**
 * @brief Calculate checksum of a buffer
 *
 * @param buf Buffer to calculate checksum
 * @param len Length of the buffer
 * @return unsigned short Checksum
 */
unsigned short calculate_checksum(unsigned short *buf, int len);

#endif /* PING_H */
