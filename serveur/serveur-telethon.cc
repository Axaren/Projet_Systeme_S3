#include <iostream>
#include <cstring>
#include <unistd.h>
#include <cstdlib>

#include "make-sockaddr.h"
#include "EnvoiDon.h"

using namespace std;

typedef uint32_t in_addr_t;

//struct in_addr
//{
//    in_addr_t s_addr;
//};

//struct sockaddr_in
//{
//    short sin_family;
//    u_short sin_port;
//    struct in_addr sin_addr;
//    char sin_zero[8];
//};

/*
    est lancé avec le numéro de port en paramètre de la ligne de commande
    —  il crée un socket UDP et le lie à une adresse locale
    —  il effectue une boucle
    —  lecture dans un tampon d’un message reçu (une structure)
    —  calcule et affiche le total
*/

int main(int argc,  char* argv[]) 
{

    // récupération des paramètres
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " port" << std::endl;
        return EXIT_FAILURE;
    }

    //  crée un socket UDP et le lie à une adresse locale
    auto adresse_locale = local_socket_address(SOCK_DGRAM, argv[1]);
    int fd = socket(AF_INET, SOCK_DGRAM, 0);

    bind(fd, (sockaddr *) & adresse_locale, sizeof(adresse_locale));

    std::cout << "** Serveur UDP port " << argv[1] << std::endl;

    int total = 0;

    // Boucle d'écoute

    while (true) {
        infosBalise m;
        if (read(fd, &m, sizeof m) == sizeof m) {
            unsigned int montant = m.montant();
            if (montant == 123456) { // code secret pour arrêter le serveur
                break;
            }
            total += montant;
            std::cout << "Merci à " << m.nom()
                      << " qui a envoyé " << montant << " €. "
                      << " total = " << total << " €"
                      << std::endl;
        }
    }
    std::cout << "Et voila. Nous avons recueilli " << total << " €"
              << std::endl;
}

