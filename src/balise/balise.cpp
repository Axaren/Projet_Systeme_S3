#include <iostream>
#include <cstring>
#include <unistd.h>

#include "../utils/EnvoiDonnees.h"
#include "../utils/make-sockaddr.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc != 7) {
        std::cerr << "Usage: " << argv[0]
              << " serveur port identifiant X Y cap" << std::endl;
        return EXIT_FAILURE;
    }

        auto adresse_serveur = remote_socket_address(argv[1], SOCK_DGRAM, argv[2]);
          int fd = socket(AF_INET, SOCK_DGRAM, 0);

                             // id     , X            , Y            , cap
          EnvoiDonnees message (argv[3], stoi(argv[4]), stoi(argv[5]), stoi(argv[6]));
          int n = sendto(fd, &message, sizeof message,
                         0,
                         (sockaddr *) &adresse_serveur, sizeof adresse_serveur);

          if (n == sizeof message)
              std::cout << "Coordonnées envoyées" << std::endl;
          else
              std::cout << "Erreur lors de l'envoi" << std::endl;

          close(fd);

}
