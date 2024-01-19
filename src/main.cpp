#include <cstddef>
#include <iostream>
#include <netinet/in.h>
#include <strings.h>
#include <sys/_endian.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/event.h>
#include <sys/time.h>
#include <unistd.h>
#include <netdb.h>
#include <utility>

int main(void) {
	int listensocket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(80);
	server_address.sin_addr.s_addr = INADDR_ANY;

	bind(listensocket, (struct sockaddr*)&server_address, sizeof(server_address));
	listen(listensocket, 5);
	int kq = kqueue();
	struct kevent event;
	EV_SET(&event, listensocket, EVFILT_READ | EVFILT_WRITE, EV_ADD, 0, 0, NULL);
	kevent(kq, &event, 1, NULL, 0, NULL);
	struct kevent events[1];
	while (1) {
		int num_events = kevent(kq, NULL, 0, events, 1, NULL);
		if (num_events == -1) {
			perror("kevent");
		} else {
    	    // Handle events
			printf("Event occurred!\n");
		}
	}
	close(kq);
    close(listensocket);

    return 0;
}