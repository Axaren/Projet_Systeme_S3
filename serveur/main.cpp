#include <iostream>
#include <cstring>
#include <unistd.h>
#include <cstdlib>
#include <vector>

#include "../utils/make-sockaddr.h"
#include "../utils/EnvoiDonnees.h"

using namespace std;

//  Le programme “serveur”
//  —  d́etient la “base de donńees” des positions
//  —  fait tourner un serveur UDP qui reçoi les messages des balises et met à jour la base
//  —  fait tourner un serveur TCP qui transmet la liste des positions aux clients qui se connectent.

int main(int argc,  char* argv[])
{
    // Base de donnée des positions :
    // Il me faut :
    // — un objet messageBalise avec un identifiant, une position (de la forme X,Y),  ?
    // — une structure de donnée à taille variable pour stocker les messages -> vector choisis

    // Thread 1 :
    // Le serveur UDP -> réutilisation du serveur téléthon

    // Thread 2 :
    // Le serveur TCP

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " port" << std::endl;
        return EXIT_FAILURE;
    }

    auto adresse_locale = local_socket_address(SOCK_DGRAM, argv[1]);
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    bind(fd, (sockaddr *) & adresse_locale, sizeof(adresse_locale));

    std::cout << "** Serveur UDP pour les balises port " << argv[1] << std::endl;

    std::vector<EnvoiDonnees> messagesBalises;
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
    std::cout << "FIN" << std::endl; // TODO afficher tous les messages des balises
}
