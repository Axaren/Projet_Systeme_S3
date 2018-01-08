#ifndef ENVOI_DON_H
#define ENVOI_DON_H

#include <string>

class EnvoiDon
{
  static const int TAILLE_MAX = 20;
  char m_nom[TAILLE_MAX];  // tableau de caractères
  unsigned int  m_montant; // dans ordre réseau, pour compatibilité

 public:
  // constructeurs
  EnvoiDon ()                                   = default;
  EnvoiDon (const std::string & un_nom, unsigned int un_montant);

  // accesseurs
  std::string   nom()     const;
  unsigned int  montant() const;     
};

#endif
