#include "EnvoiDonnees.h"
#include <arpa/inet.h>
#include <cstring>

EnvoiDonnees::EnvoiDonnees(const std::string & id, std::string position, unsigned int cap)
    :
      m_cap { htonl(cap) }
{
    strncpy(m_id, id.c_str(), TAILLE_MAX);
    strncpy(m_position, position.c_str(), 3);
}

std::string EnvoiDonnees::id() const
{
    return { m_id };
}

std::string EnvoiDonnees::position() const
{
    return { m_position };
}

unsigned int EnvoiDonnees::cap() const
{
    return ntohl(m_cap);
}
