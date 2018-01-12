#include "EnvoiDonnees.h"

EnvoiDonnees::EnvoiDonnees(const std::string & id, int x, int y, unsigned int cap)
    :
      m_cap { htonl(cap) }, m_positionX { htonl(x) }, m_positionY { htonl(y) }
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
