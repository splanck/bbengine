#pragma once

#include <string>
#include "PlayerAttributes.h" // Assumes this file defines the PlayerAttributes class.
#include "PlayerStats.h"      // Assumes this file defines the PlayerStats class.

namespace BBEngine
{

    class Player
    {
    public:
        // Default constructor.
        Player();

        // Constructor that initializes the player with a name.
        Player(const std::string & name);

        // Get the player's name.
        const std::string & getName() const;

        // Set the player's name.
        void setName(const std::string & name);

        // Accessor for the player's attributes.
        PlayerAttributes & getAttributes();
        const PlayerAttributes & getAttributes() const;

        // Accessor for the player's statistics.
        PlayerStats & getStats();
        const PlayerStats & getStats() const;

    private:
        std::string name;
        PlayerAttributes attributes;
        PlayerStats stats;
    };

} // namespace BBEngine
