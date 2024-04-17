#ifndef PORT_SCAN_H
#define PORT_SCAN_H

/**
 * @brief Scan a range of ports on a host
 *
 * @param host The host to scan
 * @param start_port The first port to scan
 * @param end_port The last port to scan
 */
void port_scan(const char *host, int start_port, int end_port);

/**
 * @brief Scan the 1000 most common ports on a host
 *
 * @param host The host to scan
 */
void port_scan_1000(const char *host);

#endif /* PORT_SCAN_H */
