#include <iostream>
#include <cstring>
#include <unistd.h>
#include <cstdlib>
#include <vector>
#include <thread>
#include <sys/stat.h>
#include <ctime>
#include <fstream>


#include "../utils/make-sockaddr.h"
#include "../utils/EnvoiDonnees.h"

using namespace std;

//  Le programme “serveur”
//  —  d́etient la “base de donńees” des positions
//  —  fait tourner un serveur UDP qui reçoi les messages des balises et met à jour la base
//  —  fait tourner un serveur TCP qui transmet la liste des positions aux clients qui se connectent.

const string cheminFichierLogs = "../../logs/server-log.txt";
vector<EnvoiDonnees> messagesBalises; // Contient les informations envoyées par les balises
bool loggingActivated = false;

void add_log(string message)
{
    // Crée le dossier contenant les logs du serveur pour Windows ou Linux
#ifdef __MINGW32__
    if (!mkdir("logs"))
#elif __linux__
    if (mkdir("../../logs", ACCESSPERMS) == 0)
#endif
    {
        cerr << "Dossier de logs crée" << endl;
    }

    if (loggingActivated)
    {

    std::ofstream fichierLogs(cheminFichierLogs, std::ios::out | std::ios::app);

    if (fichierLogs.is_open())
    {
        time_t horodate_courante = time(0);
        char* horodate_log = ctime(&horodate_courante);
        fichierLogs << "\n" << horodate_log << message << "\n";
    }

    fichierLogs.close();
    }
}

void UDPserv(string port) {
    auto adresse_locale = local_socket_address(SOCK_DGRAM, port);
    int fd = socket(AF_INET, SOCK_DGRAM, 0); // adresse et socket

    bind(fd, (sockaddr *) & adresse_locale, sizeof(adresse_locale)); // lie le server à l'adresse locale

    cout << "** Serveur UDP pour les balises port " << port << endl;

    while (true) {
        EnvoiDonnees m;
        if (read(fd, &m, sizeof m) == sizeof m) {

            messagesBalises.push_back(m);
            cout << "Informations d'une balise recue" << endl;
        }
    }
}

void TCPserv(string port) {
    auto adresse_locale = local_socket_address(SOCK_STREAM, port);
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in cli_addr;
    socklen_t clilen;
    char buffer[256];

    if (fd < 0) {
        add_log("ERREUR : Impossible d'ouvrir le socket de connection");
        exit(EXIT_FAILURE);
    }

    add_log("SUCCES : Socket de connection ouvert");

    bind(fd, (sockaddr *) & adresse_locale, sizeof(adresse_locale));

    cout << "** Serveur TCP pour la consultation port " << port << endl;

    while(true)
    {

    listen(fd, 15);
    clilen = sizeof(cli_addr);

    // On accepte la requête du client
    int newfd = accept(fd, (struct sockaddr *)&cli_addr, &clilen);

    if (newfd < 0) {
        add_log("ERREUR : Impossible d'accepter la requete du client");
        exit(EXIT_FAILURE);
    }

    add_log("SUCCES : Requete du client acceptee");

    // On lit la requête du client
    bzero(buffer,256);
    int n = read( newfd,buffer,255 );

    if (n < 0) {
        add_log("ERREUR : Impossible de lire le message recu");
        exit(EXIT_FAILURE);
    }

    add_log("SUCCES : Message recu = " + string(buffer));

    // On envoie la liste des balises au client consultation si il a inscrit
    string reponse;

    if(string(buffer) == "balises\n") {

        if(messagesBalises.size() == 0)
            reponse = "aucunes balises";
        else
            for(auto balise : messagesBalises)
                reponse += balise.to_string();
    }
    else if (string(buffer) == "stop\n")
    {
        reponse += "Serveur stoppé";
        write(newfd,reponse.c_str(),255);
        break;
    }
    else
        reponse = "ERREUR : Reponse Inconnue";

    // On envoie la reponse au client
    n = write(newfd,reponse.c_str(),255); // 255 -> taille max du string

    if (n < 0) {
        add_log("ERREUR : Impossible d'envoyer la réponse au client");
        exit(EXIT_FAILURE);
    }

    add_log("Consultation de la liste des balises effectuée");
    cout << "Consultation de la liste des balises effectuée" << endl;

    }

    add_log("Serveur stoppé");
    cout << "Serveur stoppé" << endl;
    terminate();
}

int main(int argc,  char* argv[])
{
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " port" << endl
             << "Pour activer le logging ajoutez -log après le port " << endl;
        return EXIT_FAILURE;
    }

    if (strcmp(argv[2],"-log") == 0)
        loggingActivated = true;


    // Thread 1 :
    // Le serveur UDP -> Sert à obtenir les informations des balises
    thread UDPServer(UDPserv, argv[1]);

    // Thread 2 :
    // Le serveur TCP -> Sert a la consultation des données des balises
    thread TCPServer(TCPserv, argv[1]);

    // Synchronisation des threads
    UDPServer.join();
    TCPServer.join();

    cout << "FIN" << endl;
}
