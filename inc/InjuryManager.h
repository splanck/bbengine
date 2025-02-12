#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include "Player.h"
#include "Team.h"

namespace BBEngine
{
    /**
     * Manages injuries for players, tracking how many days remain until they recover,
     * their injury type, etc. Also can remove them from the active roster upon injury,
     * then re-activate them when healthy.
     */
    class InjuryManager
    {
    public:
        // A small struct to store info about a single player's injury
        struct InjuryInfo
        {
            std::string injuryType;
            int daysRemaining;
            int severityLevel; // optional, e.g. 1=minor, 2=moderate, 3=severe
        };

        /**
         * Construct with references to all teams if we need to remove them from rosters
         * upon injury. If you don't need that, you can omit "teams".
         */
        InjuryManager(std::vector<Team*>& allTeams);

        /**
         * Mark a player as injured, storing an InjuryInfo record in "injuries".
         * daysOut is how many days until they can be activated.
         * severityLevel is optional.
         */
        void injurePlayer(Player* player,
            const std::string& injuryType,
            int daysOut,
            int severityLevel = 1);

        /**
         * Mark the player as recovered, removing them from "injuries".
         * The external code can reinsert them in lineups if needed.
         */
        void activatePlayer(Player* player);

        /**
         * Decrement the day counters for each injured player by "daysPassed".
         * If daysRemaining <= 0, we can alert or auto-activate them (depending on your design).
         */
        void decrementInjuryTimers(int daysPassed = 1);

        /**
         * Check if a given player is currently injured.
         */
        bool isPlayerInjured(Player* player) const;

        /**
         * Retrieve the player's InjuryInfo if they are injured.
         * If not found, you can throw or return a default. Here we throw for demonstration.
         */
        InjuryInfo getInjuryInfo(Player* player) const;

    private:
        // The key data structure: map from Player* to InjuryInfo
        std::unordered_map<Player*, InjuryInfo> injuries;

        // We store references to all teams, so we can find which team the player belongs to.
        std::vector<Team*>& teams;

        // Helper to find which team a Player belongs to. Return nullptr if not found.
        Team* findPlayerTeam(Player* p);

        // Helper to forcibly remove the player from the roster. 
        // If the player is on a team's roster, remove them so they can't be used.
        void removePlayerFromRoster(Player* p);
    };
}
