#include <iostream>
#include <cstring>
#include <unistd.h>

#include "make-sockaddr.h"
#include "EnvoiDonnees.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 7) {
        std::cerr << "Usage: " << argv[0]
              << " serveur port identifiant position (X,Y) cap" << std::endl;
        return EXIT_FAILURE;
    }

        auto adresse_serveur = remote_socket_address(argv[1], SOCK_DGRAM, argv[2]);
          int fd = socket(AF_INET, SOCK_DGRAM, 0);

          EnvoiDonnees message (argv[3], argv[4], atoi(argv[5]));

          int n = sendto(fd, &message, sizeof message,
                         0,
                         (sockaddr *) &adresse_serveur, sizeof adresse_serveur);

          if (n == sizeof message)
              std::cout << "Coordonnées envoyées" << std::endl;
          else
              std::cout << "Erreur lors de l'envoi" << std::endl;

          close(fd);

}

