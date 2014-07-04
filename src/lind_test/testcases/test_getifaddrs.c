/*
 * test_getifaddrs.c
 *
 *  Created on: Jun 25, 2014
 *      Author:  Ali Gholami
 */

#include "testcases.h"

int main(int argc, char **argv)
{
	test_getifaddrs();
	return 0;
}

void test_getifaddrs()
{
	/* taken from GETIFADDRS(3) Linux Programmer's Manual */
	struct ifaddrs *ifaddr, *ifa;
	int family, s;
	char host[NI_MAXHOST];

	if (getifaddrs(&ifaddr) == -1) {
		fprintf(stderr, "getifaddrs");
		return;
	}

	/* Walk through linked list, maintaining head pointer so we
	 can free list later */

	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr == NULL)
			continue;

		family = ifa->ifa_addr->sa_family;

		/* Display interface name and family (including symbolic
		 form of the latter for the common families) */

		fprintf(stdout, "%s  address family: %d%s\n", ifa->ifa_name, family,
				(family == AF_PACKET) ? " (AF_PACKET)" :
				(family == AF_INET) ? " (AF_INET)" :
				(family == AF_INET6) ? " (AF_INET6)" : "");

		/* For an AF_INET* interface address, display the address */
		if (family == AF_INET || family == AF_INET6) {
			s = getnameinfo(ifa->ifa_addr,
					(family == AF_INET) ?
							sizeof(struct sockaddr_in) :
							sizeof(struct sockaddr_in6), host, NI_MAXHOST, NULL,
					0, NI_NUMERICHOST);
			if (s != 0) {
				fprintf(stderr, "getnameinfo() failed: %s\n", gai_strerror(s));
				return;
			}
			fprintf(stdout, "\taddress: <%s>\n", host);
		}
	}

	freeifaddrs(ifaddr);
}
