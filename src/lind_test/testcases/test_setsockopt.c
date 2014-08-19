/*
 * test_setsockopt.c
 *
 *  Created on: Jun 24, 2014
 *      Author:  Ali Gholami
 */


#include "testcases.h"



#define SENDER_PORT_NUM 6000
#define SENDER_IP_ADDR "127.0.0.1"

int main(int argc, char *argv[]) {
	test_setsockopt();
	return 0;
}

void test_setsockopt() {
	/*http://my.fit.edu/~vkepuska/ece3551/ADI_Speedway_Golden/Blackfin%20Speedway%20Manuals/LwIP/socket-api/setsockopt_exp.html */
    int socket_fd;
    int option = 1; struct sockaddr_in sa;

    socket_fd = socket(PF_INET, SOCK_STREAM, 0);

    if (socket_fd < 0 ) {
        fprintf(stderr, "socket()= %d failed \n", socket_fd);
        return;
    }

    memset(&sa, 0, sizeof(struct sockaddr_in));
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr(SENDER_IP_ADDR);
    sa.sin_port = htons(SENDER_PORT_NUM);

    if (bind(socket_fd, (struct sockaddr *)&sa, sizeof(struct sockaddr_in)) < 0) {

        fprintf(stderr, "Bind to Port Number %d ,IP address %s failed\n",SENDER_PORT_NUM,SENDER_IP_ADDR);
        close(socket_fd);
        return;

    }

    if(setsockopt(socket_fd,SOL_SOCKET,(SO_REUSEPORT | SO_REUSEADDR),(char*)&option,sizeof(option)) < 0) {
        fprintf(stderr,"setsockopt() failed\n");
        close(socket_fd);
        return;
    }
    close(socket_fd);

}
