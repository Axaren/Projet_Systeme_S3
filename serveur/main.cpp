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
    int fd = socket(AF_INET, SOCK_DGRAM, 0); // SOCK_DGRAM pour datagramme UDP

    bind(fd, (sockaddr *) & adresse_locale, sizeof(adresse_locale)); // lie le server à l'adresse locale

    std::cout << "** Serveur UDP pour les balises port " << port << std::endl;

    while (true) {
        EnvoiDonnees m;
        if (read(fd, &m, sizeof m) == sizeof m) {
            if (m.id() == "123456") { // code secret pour arrêter le serveur
                break;
            }
            messagesBalises.push_back(m);
            std::cout << "Message reçu" << std::endl;
        }
    }
}

void TCPserv(std::string port) {
    auto adresse_locale = local_socket_address(SOCK_STREAM, port);
    int fd = socket(AF_INET, SOCK_STREAM, 0); // SOCK_STREAM pour flux TCP
    struct sockaddr_in cli_addr;
    socklen_t clilen;
    char buffer[256];

    if (fd < 0) {
        cerr << "ECHEC de l'ouverture du socket";
        exit(1);
    }

    bind(fd, (sockaddr *) & adresse_locale, sizeof(adresse_locale));

    std::cout << "** Serveur TCP pour les balises port " << port << std::endl;

    while(true) {
        listen(fd, 15);
        clilen = sizeof(cli_addr);

        // Accepte la connection du client
        int newfd = accept(fd, (struct sockaddr *)&cli_addr, &clilen);

        if (newfd < 0) {
            cerr << "ECHEC de l'accept" << endl;
            exit(1);
        }

        // Si la connection est établie, commence la communication
        bzero(buffer,256); // vide le buffer
        int n = read( newfd,buffer,255 );

        if (n < 0) {
            cerr << "ECHEC de la lecture depuis le socket" << endl;
            exit(1);
        }

        // Ecrit la réponse au client
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
            cerr << "ECHEC impossible d'ecrire au socket" << endl;
            exit(1);
        }
    }
}

int main(int argc,  char* argv[])
{
    // Base de donnée des positions :
    // — un objet messageBalise avec un identifiant, une position et un cap
    // — une structure de donnée à taille variable pour stocker les messages -> vecteur

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " port" << std::endl;
        return EXIT_FAILURE;
    }

    // Thread 1 :
    // Le serveur UDP -> réutilisation du serveur téléthon
    std::thread UDPServer(UDPserv, argv[1]); // invoque un nouveau thread qui lance serverUDP()

    // Thread 2 :
    // Le serveur TCP
    std::thread TCPServer(TCPserv, argv[1]);  // invoque un nouveau thread qui lance serverTCP()

    // synchronize threads:
    UDPServer.join();               // attend que le premier thread se finisse
    TCPServer.join();               // attend que le second  thread se finisse

    std::cout << "FIN" << std::endl;
}
