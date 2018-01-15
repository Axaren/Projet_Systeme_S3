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
    struct hostent *serveur;

    char buffer[256];

    if (argc < 3) {
        cerr << argv[0] << "Usage : %s adresse port" << endl;
        exit(EXIT_FAILURE);
    }

    port = atoi(argv[2]);

    // On crée le socket de connection
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        cerr << "ERREUR : Impossible d'ouvrir le socket" << endl;
        exit(EXIT_FAILURE);
    }

    serveur = gethostbyname(argv[1]);

    if (serveur == NULL) {
        cerr << "Serveur introuvable" << endl;
        exit(EXIT_SUCCESS);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)serveur->h_addr, (char *)&serv_addr.sin_addr.s_addr, serveur->h_length);
    serv_addr.sin_port = htons(port);

    // Connexion au serveur
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        cerr << "ERREUR : Impossible de se connecter" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Entrez \"balises\" Pour accéder à la liste des bateaux dans la base de données" << endl;
    bzero(buffer,256);
    fgets(buffer,255,stdin);

    // On demande une connexion au serveur
    n = write(sockfd, buffer, strlen(buffer));

    if (n < 0) {
        cerr << "ERREUR : Impossible d'ecrire vers le serveur" << endl;
        exit(EXIT_FAILURE);
    }

    // On récupère la reponse du serveur
    bzero(buffer,256);
    n = read(sockfd, buffer, 255);

    if (n < 0) {
        cerr << "ERREUR : Impossible de récupérer la reponse du serveur" << endl;
        exit(EXIT_FAILURE);
    }

    cout << buffer << endl;
    return EXIT_SUCCESS;
}

