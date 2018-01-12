#include <iostream>
#include <cstring>
#include <unistd.h>
#include <cstdlib>
#include <vector>
#include <thread>

#include "../utils/make-sockaddr.h"
#include "../utils/EnvoiDonnees.h"

using namespace std;

//  Le programme “serveur”
//  —  d́etient la “base de donńees” des positions
//  —  fait tourner un serveur UDP qui reçoi les messages des balises et met à jour la base
//  —  fait tourner un serveur TCP qui transmet la liste des positions aux clients qui se connectent.

std::vector<EnvoiDonnees> messagesBalises;

void UDPserv(std::string port) {
    auto adresse_locale = local_socket_address(SOCK_DGRAM, port);
    int fd = socket(AF_INET, SOCK_DGRAM, 0); // adresse et socket

    bind(fd, (sockaddr *) & adresse_locale, sizeof(adresse_locale)); // lie le server à l'adresse locale

    std::cout << "** Serveur UDP pour les balises port " << port << std::endl;

    while (true) {
        EnvoiDonnees m;
        if (read(fd, &m, sizeof m) == sizeof m) {
            if (m.id() == "123456") { // code secret pour arrêter le serveur
                break;
            }
            messagesBalises.push_back(m);
            std::cout << "Message reçu" << std::endl; // TODO ajouter le detail
        }
    }
}

void TCPserv(std::string port) {
    auto adresse_locale = local_socket_address(SOCK_STREAM, port);
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in cli_addr;
    socklen_t clilen;
    char buffer[256];

    if (fd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    bind(fd, (sockaddr *) & adresse_locale, sizeof(adresse_locale));

    std::cout << "** Serveur TCP pour les balises port " << port << std::endl;

    listen(fd, 15);
    clilen = sizeof(cli_addr);

    /* Accept actual connection from the client */
    int newfd = accept(fd, (struct sockaddr *)&cli_addr, &clilen);

    if (newfd < 0) {
        perror("ERROR on accept");
        exit(1);
    }

    /* If connection is established then start communicating */
    bzero(buffer,256);
    int n = read( newfd,buffer,255 );

    if (n < 0) {
        perror("ERROR reading from socket");
        exit(1);
    }

    /* Write a response to the client */
    string response;

    if(string(buffer) == "balises\n") {

        if(messagesBalises.size() == 0)
            response = "aucunes balises";
        else
            for(auto balise : messagesBalises) {
                response +=
                        balise.id() + " " +
                        to_string(balise.getX()) + " " +
                        to_string(balise.getY()) + " " +
                        to_string(balise.cap()) + "\n";
            }
    }

    n = write(newfd,response.c_str(),255); // 255 -> taille max du string

    if (n < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }
}

int main(int argc,  char* argv[])
{
    // Base de donnée des positions :
    // Il me faut :
    // — un objet messageBalise avec un identifiant, une position (de la forme X,Y),  ?
    // — une structure de donnée à taille variable pour stocker les messages -> vector

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " port" << std::endl;
        return EXIT_FAILURE;
    }

    // Thread 1 :
    // Le serveur UDP -> réutilisation du serveur téléthon
    std::thread UDPServer(UDPserv, argv[1]); // spawn new thread that calls serverUDP()

    // Thread 2 :
    // Le serveur TCP
    std::thread TCPServer(TCPserv, argv[1]);  // spawn new thread that calls serverTCP()

    // synchronize threads:
    UDPServer.join();               // pauses until first finishes
    TCPServer.join();               // pauses until second finishes

    std::cout << "FIN" << std::endl; // TODO afficher tous les messages des balises
}
