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
  int m_positionX;        // caractère pour la position en X
  int m_positionY;        // caractère pour la position en Y
  unsigned int m_cap;     // entre 0 et 360 degrés

 public:
  // constructeurs
  EnvoiDonnees () = default;
  EnvoiDonnees (const std::string & id, int x, int y, unsigned int cap);

  // accesseurs
  std::string id()     const;
  int getX()           const;
  int getY()           const;
  unsigned int cap()   const;
};

#endif
