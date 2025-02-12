#pragma once

#include <string>
#include <vector>
#include "Player.h"           // needed for Player*
#include "StartingRotation.h" // needed for StartingRotation*

namespace BBEngine
{
    class Team
    {
    public:
        /**
         * Constructor taking a team name and a level (like "MLB", "AAA", etc.).
         * The rotation is initially null, lineups and roster empty.
         */
        Team(const std::string& teamName, const std::string& levelIndicator = "MLB");

        // ----------------------------
        // Roster Management
        // ----------------------------
        void addPlayer(Player* p);
        void removePlayer(Player* p);
        // NEW: Checks if a player is on the team's roster
        bool hasPlayer(Player* p) const;

        // ----------------------------
        // Lineup Methods
        // ----------------------------
        void setLineupVsRHP(const std::vector<Player*>& lineup);
        void setLineupVsLHP(const std::vector<Player*>& lineup);

        const std::vector<Player*>& getLineupVsRHP() const;
        const std::vector<Player*>& getLineupVsLHP() const;

        // ----------------------------
        // Rotation Methods
        // ----------------------------
        StartingRotation* getRotation() const;
        void setRotation(StartingRotation* newRotation);

        // ----------------------------
        // Basic Accessors
        // ----------------------------
        const std::string& getName() const;
        void setName(const std::string& newName);

        const std::string& getLevel() const;
        void setLevel(const std::string& newLevel);

        // If you want direct access to the entire roster:
        const std::vector<Player*>& getRoster() const { return roster; }

    private:
        std::string name;
        std::string level;

        // A list of all players on the team
        std::vector<Player*> roster;

        // Offensive lineups for different pitcher handedness
        std::vector<Player*> lineupVsRHP;
        std::vector<Player*> lineupVsLHP;

        // Pointer to a StartingRotation that enumerates who starts next
        StartingRotation* rotation;
    };

} // namespace BBEngine
