#include "EnvoiDonnees.h"

EnvoiDonnees::EnvoiDonnees(const std::string & id, int x, int y, unsigned int cap)
    :
      m_positionX { htonl(x) }, m_positionY { htonl(y) }, m_cap { htonl(cap) }
{
    strncpy(m_id, id.c_str(), TAILLE_MAX);
}

std::string EnvoiDonnees::id() const
{
    return { m_id };
}

int EnvoiDonnees::getX() const
{
    return ntohl(m_positionX);
}

int EnvoiDonnees::getY() const
{
    return ntohl(m_positionY);
}

unsigned int EnvoiDonnees::cap() const
{
    return ntohl(m_cap);
}

std::string EnvoiDonnees::to_string()
{
    std::string donnees;
    donnees += "ID : " + std::string(m_id) + " "
            +  "Position : " + std::to_string(ntohl(m_positionX)) + "," + std::to_string(ntohl(m_positionY)) + " "
            +  "Cap : " + std::to_string(ntohl(m_cap)) + " degrés\n";
    return donnees;
}
