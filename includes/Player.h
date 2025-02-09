#pragma once

#include <string>
#include <memory>   // if you want to use smart pointers
#include <cassert>  // or <stdexcept> for validations

namespace BBEngine
{
    class PlayerAttributes;
    class PlayerStats;

    // Simple enum for batting/throwing handedness
    enum class Handedness
    {
        Left,
        Right,
        Switch
    };

    /**
     * The Player class represents a single real-world (or fictional) baseball player
     * with personal info, skill ratings (PlayerAttributes), and performance data (PlayerStats).
     */
    class Player
    {
    public:
        // ----------------------------------------------------------------
        // Constructor(s)
        // ----------------------------------------------------------------
        /**
         * Main constructor that accepts name, age, handedness,
         * plus pointers to PlayerAttributes & PlayerStats.
         *
         * If your design demands non-nullptr for attributes/stats,
         * consider throwing if they're null.
         */
        Player(const std::string& name,
            int age,
            Handedness handedness,
            PlayerAttributes* attr,
            PlayerStats* stats);

        /**
         * Optional constructor that sets default PlayerAttributes / PlayerStats
         * if none are provided. If you want a simpler approach.
         */
        Player(const std::string& name,
            int age,
            Handedness handedness);

        // ----------------------------------------------------------------
        // Accessors / Mutators for Personal Info
        // ----------------------------------------------------------------
        const std::string& getName() const;
        void setName(const std::string& newName);

        int getAge() const;
        void setAge(int newAge);

        Handedness getHandedness() const;
        void setHandedness(Handedness newHandedness);

        // ----------------------------------------------------------------
        // Accessors for Attributes & Stats
        // ----------------------------------------------------------------
        PlayerAttributes* getAttributes() const;
        void setAttributes(PlayerAttributes* newAttr);

        PlayerStats* getStats() const;
        void setStats(PlayerStats* newStats);

        // ----------------------------------------------------------------
        // Optional Fields (position, isActive, etc.)
        // ----------------------------------------------------------------
        const std::string& getPosition() const;
        void setPosition(const std::string& pos);

        bool isActive() const;
        void setActive(bool activeStatus);

        // ----------------------------------------------------------------
        // Optional: If you track aging or development within the Player
        // ----------------------------------------------------------------
        void incrementAge();

    private:
        // Basic personal info
        std::string  name;
        int          age;
        Handedness   handedness;

        // Skill ratings and stats
        PlayerAttributes* attributes;  ///< pointer to the player's skill ratings
        PlayerStats* stats;       ///< pointer to the player's performance data

        // Optional fields
        std::string position;  ///< e.g., "Pitcher", "Catcher", or "1B"
        bool active;           ///< if you want to mark the player as active/injured/retired, etc.
    };

} // namespace BBEngine
