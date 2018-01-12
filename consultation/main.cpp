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
    //    bind listen accept -> nouveau socket dans un int
    //    read un 0 -> fin de la connection
    /*
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

        // si on ecrit coords -> while cin != coords
        // envoie la demande des coordonnées -> bind
        // ecoute la réponse -> listen
        // accepte la réponse -> accept
    }
        */


    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];

    if (argc < 3) {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);

    /* Create a socket point */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    server = gethostbyname(argv[1]);

    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    /* Now connect to the server */
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR connecting");
        exit(1);
    }

    /* Now ask for a message from the user, this message
     * will be read by server
    */

    printf("Please enter the message: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);

    /* Send message to the server */
    n = write(sockfd, buffer, strlen(buffer));

    if (n < 0) {
        perror("ERROR writing to socket");
        exit(1);
    }

    /* Now read server response */
    bzero(buffer,256);
    n = read(sockfd, buffer, 255);

    if (n < 0) {
        perror("ERROR reading from socket");
        exit(1);
    }

    printf("%s\n",buffer);
    return 0;
}

