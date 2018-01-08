#include <iostream>

#include "make-sockaddr.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cstring>

/*
 * retourne une adresse de socket IPv4
 * pour une machine et un service donnés
 * Le service peut-être un nom "ftp" ou une chaine numérique "5678"
 * Levée d'exception si introuvable.
 */

struct sockaddr_in remote_socket_address(
       const std::string &hostname,
       int socktype,                 // 0, SOCK_DGRAM, ou SOCK_STREAM
       const std::string &service)
{
    struct addrinfo hints;
    struct addrinfo *result;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family   = AF_INET;   // IPv4 seulement
    hints.ai_socktype = socktype;
    hints.ai_flags    = 0;
    hints.ai_protocol = 0;
    hints.ai_canonname = nullptr;
    hints.ai_addr      = nullptr;
    hints.ai_next      = nullptr;

    if (getaddrinfo(hostname.c_str(), service.c_str(),
            &hints, &result) != 0) {
      throw "getaddrinfo() failed";
    }
    if (result == nullptr) {
      throw "Not found";
    }
    auto address = *((sockaddr_in *)(result->ai_addr));

    freeaddrinfo(result);
    return address;
}

struct sockaddr_in local_socket_address(
    int socktype,                 // 0, SOCK_DGRAM, ou SOCK_STREAM
    const std::string &service)
{
  struct addrinfo hints;
  struct addrinfo *result;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family   = AF_INET;   // IPv4 seulement
  hints.ai_socktype = socktype;
  hints.ai_flags    = AI_PASSIVE;  // INADDR_ANY
  hints.ai_protocol = 0;
  hints.ai_canonname = nullptr;
  hints.ai_addr      = nullptr;
  hints.ai_next      = nullptr;

  if (getaddrinfo(nullptr, service.c_str(),
          &hints, &result) != 0) {
    throw "Unknown service";
  }
  auto address =  *((sockaddr_in *)(result->ai_addr));

  freeaddrinfo(result);
  return address;
}

