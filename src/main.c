#ifdef _WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#else
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#endif

#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "tinyosc.h"
#include "manymouse.h"

static volatile bool keepRunning = true;

// handle Ctrl+C
static void sigintHandler(int x) {
  keepRunning = false;
}

int osc_send(SOCKET* fd, unsigned int device, unsigned int axis, int delta){
  char buffer[2048]; // declare a 2Kb buffer to read packet data into

  int len = 0;
  len = tosc_writeMessage(buffer, sizeof(buffer), "/manymouse/relmotion", "iii", device, axis, delta);

  // tosc_printOscBuffer(buffer, len); // for debug

  int res = send(fd, buffer, len, 0);
  return res;
}

/**
 * A basic program to listen to port 9000 and print received OSC packets.
 */
int main(int argc, char *argv[]) {

#ifdef _WIN32
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        return 1;  // Return immediately if WSAStartup fails
    }
#endif

  // open a socket to send for datagrams (i.e. UDP packets) on port 9001
  SOCKET* fd_send = socket(AF_INET, SOCK_DGRAM, 0);

  // set the socket to non-blocking
#ifndef _WIN32
  fcntl(fd_send, F_SETFL, O_NONBLOCK); 
#else
  u_long mode_s = 1; // non blocking
  ioctlsocket(fd_send, FIONBIO, &mode_s);
#endif

  struct sockaddr_in sout;
  sout.sin_family = AF_INET;
  // sout.sin_addr.s_addr = "127.0.0.1";
  // sout.sin_addr = "127.0.0.1";
  const char* out_addr = "127.0.0.1";
  int out_port = 9001;
  struct hostent *hostinfo;
  hostinfo = gethostbyname(out_addr);
  memcpy(&sout.sin_addr, hostinfo->h_addr_list[0], hostinfo->h_length);
  sout.sin_port = htons(out_port);

  int resb = connect(fd_send, (struct sockaddr*)&sout, sizeof(struct sockaddr_in));

  printf("Connecting to %s:%d\n", out_addr, out_port);

  if(resb < 0){
    printf("Failed to connect, result: %d\n", resb);

    // close the UDP out socket
    #ifdef _WIN32
      closesocket(fd_send);
    #else
      close(fd_send);
    #endif

    // exit(1);
    return 1;
  }

  // char buffer[2048]; // declare a 2Kb buffer to read packet data into

  // printf("Starting write tests:\n");
  // int len = 0;
  // // char blob[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
  // // len = tosc_writeMessage(buffer, sizeof(buffer), "/address", "fsibTFNI",
  // //     1.0f, "hello world", -1, sizeof(blob), blob);
  // // char blob[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
  // len = tosc_writeMessage(buffer, sizeof(buffer), "/hello", "fi", 1.0f, 2);
  // tosc_printOscBuffer(buffer, len);

  // printf("Sending to %s:%d\n", out_addr, out_port);

  // int res = send(fd_send, buffer, len, 0);
  // printf("total bytes sent: %d\n", res);

  // printf("done.\n");

  // many mouse init
  unsigned int device_count = ManyMouse_Init();
  if(device_count <= 0){
    printf("many mouse init failed\n");
    return 1;
  }

  printf("many mouse device count: %d\n", device_count);

  // register the SIGINT handler (Ctrl+C)
  signal(SIGINT, &sigintHandler);

  printf("Press Ctrl+C to stop.\n");

  ManyMouseEvent m;
  char buffer[2048]; // declare a 2Kb buffer to read packet data into

  while (keepRunning) {
    while(ManyMouse_PollEvent(&m)){
      if(m.type == MANYMOUSE_EVENT_RELMOTION){
        unsigned int device = m.device;
        unsigned int axis = m.item;
        int delta = m.value;

        osc_send(fd_send, device, axis, delta);
      }
    }
  }

  // close the UDP out socket
#ifdef _WIN32
  closesocket(fd_send);
#else
  close(fd_send);
#endif

//   // close the UDP in socket
// #ifdef _WIN32
//   closesocket(fd);
//   WSACleanup();
// #else
//   close(fd);
// #endif

  return 0;
}
