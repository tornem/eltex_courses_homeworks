#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

#include <netinet/in.h>
#include <netinet/udp.h>
#include <sys/socket.h>
#include <sys/types.h>

#define PACKET_SIZE 500
#define IP_HEADER_LEN 20

  // This client app create raw socket and filling the formed
  // own udp header. Next step this app sending this datagram
  // (own udpheader + data) in usual UDP echo server. Next, 
  // receiving standart datagram from this server from UDP_SOCKET. 
  // Because we using raw sockets, all packets sended to all 
  // listener sockets in interface, consequently we need 
  // pick out exactly that packet, who has come from our
  // server. Therefore, all packets are checked for port matching.

  // For testing the work of the program you can use server from
  // ~NETS/UDP_ECHO ./server.c

int main () 
{
    int client_sock;
    char packet[PACKET_SIZE], *data;
    struct sockaddr_in server_addr;
    socklen_t server_addr_len;
    struct udphdr* my_udp_packet;

    client_sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (client_sock == -1) {
        perror("socket");
        exit (EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server_addr.sin_port = htons(2395);

    // filling udp header
    my_udp_packet = (struct udphdr*) packet;

    my_udp_packet->uh_sport = htons(2395);
    my_udp_packet->uh_dport = htons(1414);
    my_udp_packet->uh_ulen = htons(PACKET_SIZE);
    my_udp_packet->uh_sum = 0;

    data = packet + sizeof(struct udphdr);  // address data part
    strcpy(data, "Hi server!");

    if (sendto(client_sock, packet, sizeof(packet), 0, 
        (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        perror("sendto");
        exit(EXIT_FAILURE);
    } 

    puts("Packet sended");

    while (true) {
        if (recvfrom(client_sock, packet, sizeof(packet), 0, NULL, NULL) == -1) {
            perror("recvfrom");
            exit(EXIT_FAILURE);
        }
        // if packet has come from destination port
        if ( ((struct udphdr*)(packet+IP_HEADER_LEN))->uh_sport == htons(1414)) {
            break;
        }
    }

    printf("%s\n", data+IP_HEADER_LEN);  // cut off ip header

    exit(EXIT_SUCCESS);

}