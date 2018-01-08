#ifndef ENVOI_DONNEES_H
#define ENVOI_DONnEES_H

#include <arpa/inet.h>
#include <cstring>
#include <string>

class EnvoiDonnees
{
private:

  static const int TAILLE_MAX = 10;
  char m_id[TAILLE_MAX];  // tableau de caractères
  char m_position[3]; // tableau de caractères
  unsigned int m_cap; // entre 0 et 360 degrés

 public:
  // constructeurs
  EnvoiDonnees () = default;
  EnvoiDonnees (const std::string & id, std::string position, unsigned int cap);

  // accesseurs
  std::string id()     const;
  std::string position() const;
  unsigned int cap() const;
};

#endif
