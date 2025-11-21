/**
 * Copyright (c) 2015, Martin Roth (mhroth@gmail.com)
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
 * INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
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

#include "tinyosc.h"

static volatile bool keepRunning = true;

// handle Ctrl+C
static void sigintHandler(int x) {
  keepRunning = false;
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
  int fd_send = socket(AF_INET, SOCK_DGRAM, 0);

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
  // sout.sin_addr.s_addr = "localhost";
  // int resb = bind(fd_send, (struct sockaddr *) &sout, sizeof(struct sockaddr_in));
  int resb = connect(fd_send, (struct sockaddr*)&sout, sizeof(struct sockaddr_in));
  // assert(resb != -1);
  printf("Connecting to %s:%d\n", out_addr, out_port);
  if(resb < 0){
    printf("Failed to connect, result: %d\n", resb);

    // close the UDP out socket
    #ifdef _WIN32
      closesocket(fd_send);
    #else
      close(fd_send);
    #endif

    exit(1);
  }

  char buffer[2048]; // declare a 2Kb buffer to read packet data into

  printf("Starting write tests:\n");
  int len = 0;
  // char blob[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
  // len = tosc_writeMessage(buffer, sizeof(buffer), "/address", "fsibTFNI",
  //     1.0f, "hello world", -1, sizeof(blob), blob);
  // char blob[8] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF};
  len = tosc_writeMessage(buffer, sizeof(buffer), "/hello", "fi", 1.0f, 2);
  tosc_printOscBuffer(buffer, len);

  printf("Sending to %s:%d\n", out_addr, out_port);

  int res = send(fd_send, buffer, len, 0);
  printf("total bytes sent: %d\n", res);

  printf("done.\n");

  // register the SIGINT handler (Ctrl+C)
  signal(SIGINT, &sigintHandler);

  // // open a socket to listen for datagrams (i.e. UDP packets) on port 9000
  // const int fd = socket(AF_INET, SOCK_DGRAM, 0);

//   // set the socket to non-blocking
// #ifndef _WIN32
//   fcntl(fd, F_SETFL, O_NONBLOCK); 
// #else
//   u_long mode = 1; // non blocking
//   ioctlsocket(fd, FIONBIO, &mode);
// #endif

  // struct sockaddr_in sin;
  // sin.sin_family = AF_INET;
  // sin.sin_port = htons(9000);
  // sin.sin_addr.s_addr = INADDR_ANY;
  // bind(fd, (struct sockaddr *) &sin, sizeof(struct sockaddr_in));
  // printf("tinyosc is now listening on port 9000.\n");

  printf("Press Ctrl+C to stop.\n");

  while (keepRunning) {
    // fd_set readSet;
    // FD_ZERO(&readSet);
    // FD_SET(fd, &readSet);
    // struct timeval timeout = {1, 0}; // select times out after 1 second
    // if (select(fd+1, &readSet, NULL, NULL, &timeout) > 0) {
    //   struct sockaddr sa; // can be safely cast to sockaddr_in
    //   socklen_t sa_len = sizeof(struct sockaddr_in);
    //   int len = 0;
    //   while ((len = (int) recvfrom(fd, buffer, sizeof(buffer), 0, &sa, &sa_len)) > 0) {
    //     if (tosc_isBundle(buffer)) {
    //       tosc_bundle bundle;
    //       tosc_parseBundle(&bundle, buffer, len);
    //       const uint64_t timetag = tosc_getTimetag(&bundle);
    //       tosc_message osc;
    //       while (tosc_getNextMessage(&bundle, &osc)) {
    //         tosc_printMessage(&osc);
    //       }
    //     } else {
    //       tosc_message osc;
    //       tosc_parseMessage(&osc, buffer, len);
    //       tosc_printMessage(&osc);
    //     }
    //   }
    // }
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
