#include "EnvoiDon.h"
#include <arpa/inet.h>
#include <cstring>

EnvoiDon::EnvoiDon (const std::string & un_nom, unsigned int un_montant)
  : m_montant { htonl(un_montant) }
  {
    strncpy(m_nom, un_nom.c_str(), TAILLE_MAX);
  }

std::string EnvoiDon::nom() const {
  return { m_nom };
}

unsigned int  EnvoiDon::montant() const {
  return  ntohl(m_montant) ;
}
