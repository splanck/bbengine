#pragma once

#include <vector>
#include <string>
#include "Team.h"

namespace BBEngine
{
    class TradeManager
    {
    public:
        /**
         * Constructor: pass references to the teams and the free agent pool,
         * plus any constraints (roster sizes, trade deadlines, etc.) you want.
         */
        TradeManager(std::vector<Team*>& leagueTeams,
            std::vector<Player*>& freeAgentsPool,
            bool deadlinePassed = false,
            int maxRosterSize = 26);

        /**
         * Propose a trade: fromTeam gives 'playersToGive' to toTeam,
         * and toTeam gives 'playersToReceive' to fromTeam.
         * Returns true if it is valid and was executed, false if invalid (deadline, roster, etc.).
         */
        bool proposeTrade(Team* fromTeam, Team* toTeam,
            const std::vector<Player*>& playersToGive,
            const std::vector<Player*>& playersToReceive);

        /**
         * Attempt to sign a free agent onto signingTeam.
         * Return true if success, false if constraints fail or the player is not in freeAgents.
         */
        bool signFreeAgent(Team* signingTeam, Player* freeAgent);

        /**
         * Release a player from a team. This player goes to the free agent list.
         * Return true if success, false if the player wasn't on that team, etc.
         */
        bool releasePlayer(Team* fromTeam, Player* p);

        /**
         * If you replicate an MLB-like scenario, you can toggle the trade deadline at runtime.
         */
        void setDeadlinePassed(bool isPassed);
        bool getDeadlinePassed() const;

    private:
        // The master list of teams, possibly from League
        std::vector<Team*>& teams;

        // The free agent pool
        std::vector<Player*>& freeAgents;

        // Constraints
        bool deadline;
        int  maxActiveRoster;

        // Internal helper to check rosters for capacity, etc.
        bool canTeamAcquirePlayers(Team* team, const std::vector<Player*>& newPlayers);

        /**
         * Actually perform the trade once validated.
         * Moves the given players between rosters.
         */
        void executeTrade(Team* fromTeam, Team* toTeam,
            const std::vector<Player*>& fromTeamPlayers,
            const std::vector<Player*>& toTeamPlayers);
    };
}
