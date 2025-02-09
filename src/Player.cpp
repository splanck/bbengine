#include "Player.h"

namespace BBEngine
{
    // ---------------------------------------------------------------
    // Constructor(s)
    // ---------------------------------------------------------------
    Player::Player(const std::string& name,
        int age,
        Handedness handedness,
        PlayerAttributes* attr,
        PlayerStats* stats)
        : name(name),
        age(age),
        handedness(handedness),
        attributes(attr),
        stats(stats),
        position("Unknown"),
        active(true) // default active
    {
        // If you require non-null attributes/stats, you could do:
        // if (!attributes || !stats)
        // {
        //     throw std::invalid_argument("PlayerAttributes and PlayerStats cannot be null");
        // }
    }

    Player::Player(const std::string& name,
        int age,
        Handedness handedness)
        : name(name),
        age(age),
        handedness(handedness),
        attributes(nullptr),
        stats(nullptr),
        position("Unknown"),
        active(true)
    {
        // You could create default attributes/stats internally if you want:
        // attributes = new PlayerAttributes();
        // stats = new PlayerStats();
    }

    // ---------------------------------------------------------------
    // Accessors / Mutators - Personal Info
    // ---------------------------------------------------------------
    const std::string& Player::getName() const
    {
        return name;
    }
    void Player::setName(const std::string& newName)
    {
        name = newName;
    }

    int Player::getAge() const
    {
        return age;
    }
    void Player::setAge(int newAge)
    {
        // Optionally check validity if you want (no negative, etc.)
        if (newAge < 0) {
            // Could clamp or throw
            // throw std::invalid_argument("Age cannot be negative");
            age = 0;
        }
        else {
            age = newAge;
        }
    }

    Handedness Player::getHandedness() const
    {
        return handedness;
    }
    void Player::setHandedness(Handedness newHandedness)
    {
        handedness = newHandedness;
    }

    // ---------------------------------------------------------------
    // Accessors / Mutators - Attributes & Stats
    // ---------------------------------------------------------------
    PlayerAttributes* Player::getAttributes() const
    {
        return attributes;
    }
    void Player::setAttributes(PlayerAttributes* newAttr)
    {
        attributes = newAttr;
        // if (!newAttr) throw ...
    }

    PlayerStats* Player::getStats() const
    {
        return stats;
    }
    void Player::setStats(PlayerStats* newStats)
    {
        stats = newStats;
        // if (!newStats) throw ...
    }

    // ---------------------------------------------------------------
    // Optional Fields (position, isActive, etc.)
    // ---------------------------------------------------------------
    const std::string& Player::getPosition() const
    {
        return position;
    }
    void Player::setPosition(const std::string& pos)
    {
        position = pos;
    }

    bool Player::isActive() const
    {
        return active;
    }
    void Player::setActive(bool activeStatus)
    {
        active = activeStatus;
    }

    // ---------------------------------------------------------------
    // Example of a method to increment age
    // ---------------------------------------------------------------
    void Player::incrementAge()
    {
        setAge(age + 1);  // or just age++;
        // If you want to also degrade or improve attributes, do so:
        // if (attributes) attributes->ageOneYear();
    }

} // namespace BBEngine
