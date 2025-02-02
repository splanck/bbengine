#include "Player.h"

namespace BBEngine
{
    Player::Player()
    {
        name = "";
    }

    Player::Player(const std::string & name)
    {
        this->name = name;
    }

    const std::string & Player::getName() const
    {
        return name;
    }

    void Player::setName(const std::string & name)
    {
        this->name = name;
    }

    PlayerAttributes & Player::getAttributes()
    {
        return attributes;
    }

    const PlayerAttributes & Player::getAttributes() const
    {
        return attributes;
    }

    PlayerStats & Player::getStats()
    {
        return stats;
    }

    const PlayerStats & Player::getStats() const
    {
        return stats;
    }

} // namespace BBEngine
