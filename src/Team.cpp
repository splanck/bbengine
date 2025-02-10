#include "Team.h"
#include <algorithm>  // for std::find

namespace BBEngine
{
    // Constructor
    Team::Team(const std::string& teamName, const std::string& levelIndicator)
        : name(teamName), level(levelIndicator), rotation(nullptr)
    {
    }

    // -----------------------------------------------------------
    // Roster Management
    // -----------------------------------------------------------
    void Team::addPlayer(Player* p)
    {
        if (!p) return; // or throw an exception if you disallow null
        // optional check if already in the roster:
        auto it = std::find(roster.begin(), roster.end(), p);
        if (it == roster.end())
        {
            roster.push_back(p);
        }
    }

    void Team::removePlayer(Player* p)
    {
        auto it = std::find(roster.begin(), roster.end(), p);
        if (it != roster.end())
        {
            // Erase from the main roster
            roster.erase(it);

            // Also remove from lineups if present
            auto removeFromLineup = [&](std::vector<Player*>& lineup) {
                auto lit = std::find(lineup.begin(), lineup.end(), p);
                if (lit != lineup.end())
                    lineup.erase(lit);
                };
            removeFromLineup(lineupVsRHP);
            removeFromLineup(lineupVsLHP);

            // If the rotation includes that pitcher, remove them as well
            if (rotation)
            {
                auto& pitchers = rotation->getPitchers();
                for (size_t i = 0; i < pitchers.size(); ++i)
                {
                    if (pitchers[i] == p)
                    {
                        rotation->removePitcher(static_cast<int>(i));
                        break;
                    }
                }
            }
        }
    }

    // -----------------------------------------------------------
    // Lineup Methods
    // -----------------------------------------------------------
    void Team::setLineupVsRHP(const std::vector<Player*>& lineup)
    {
        lineupVsRHP = lineup;
    }

    void Team::setLineupVsLHP(const std::vector<Player*>& lineup)
    {
        lineupVsLHP = lineup;
    }

    const std::vector<Player*>& Team::getLineupVsRHP() const
    {
        return lineupVsRHP;
    }

    const std::vector<Player*>& Team::getLineupVsLHP() const
    {
        return lineupVsLHP;
    }

    // -----------------------------------------------------------
    // Rotation Methods
    // -----------------------------------------------------------
    StartingRotation* Team::getRotation() const
    {
        return rotation;
    }

    void Team::setRotation(StartingRotation* newRotation)
    {
        rotation = newRotation;
    }

    // -----------------------------------------------------------
    // Basic Accessors
    // -----------------------------------------------------------
    const std::string& Team::getName() const
    {
        return name;
    }

    void Team::setName(const std::string& newName)
    {
        name = newName;
    }

    const std::string& Team::getLevel() const
    {
        return level;
    }

    void Team::setLevel(const std::string& newLevel)
    {
        level = newLevel;
    }

} // namespace BBEngine
