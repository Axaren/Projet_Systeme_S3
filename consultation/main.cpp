#include <iostream>
#include <cstring>
#include <unistd.h>
#include <cstdlib>
#include <vector>

#include "../utils/make-sockaddr.h"
#include "../utils/EnvoiDonnees.h"

using namespace std;

int main(int argc,  char* argv[])
{
    int sockfd, port, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

    if (argc != 3) {
        std::cerr << "Usage: "
                  << "serveur port" << std::endl;
        return EXIT_FAILURE;
    }

    port = atoi(argv[2]);

    // Créée le socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        std::cerr << "ECHEC de la création du socket" << std::endl;
        return EXIT_FAILURE;
    }

    server = gethostbyname(argv[1]);

    if (server == NULL) {
        std::cerr << "ECHEC ce server n'existe pas" << std::endl;
        return EXIT_FAILURE;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(port);

    // Connection au serveur
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "ECHEC impossible de se connecter" << std::endl;
        return EXIT_FAILURE;
    }

    while(true) {

        // Demande à l'utilisateur d'entrer une commande
        // qui sera lue par le serveur

        cout << "Veuillez entrer une commande: " << endl;
        bzero(buffer,256); // vide le buffer
        fgets(buffer,255,stdin);

        // Envoi du message au server
        n = write(sockfd, buffer, strlen(buffer));

        if (n < 0) {
            cerr << "ECHEC lors de l'ecriture au socket" << endl;
            return EXIT_FAILURE;
        }

        // Lecture de la réponse du serveur
        bzero(buffer,256);
        n = read(sockfd, buffer, 255);

        if (n < 0) {
            cerr << "ECHEC lecture du socket" << endl;
            return EXIT_FAILURE;
        }

        printf("%s\n",buffer);
    }

    return EXIT_SUCCESS;
}

