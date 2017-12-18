#ifndef MAKE_SOCKADDR_H
#define MAKE_SOCKADDR_H

#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/*
 * retourne une adresse de sockets IPv4
 * pour une machine et un service donnés
 * Le service peut-être un nom "ftp" ou une chaine numérique "5678"
 */

struct sockaddr_in remote_socket_address(
        const std::string &hostname,
        int socktype,                 // 0, SOCK_DGRAM, ou SOCK_STREAM
        const std::string &service);

/*
 * retourne une adresse de sockets IPv4
 * pour la machine locale et un service donnés
 * Le service peut-être un nom "ftp" ou une chaine numérique "5678"
 */

struct sockaddr_in local_socket_address(
        int socktype,                 // 0, SOCK_DGRAM, ou SOCK_STREAM
        const std::string &service);

#endif
