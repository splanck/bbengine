#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <utility>
#include "Player.h"
#include "Team.h"

namespace BBEngine
{
    /**
     * The StatsManager class aggregates stats from many players (and possibly teams),
     * generates leaderboards for hitting/pitching, can track all-time records,
     * and provides optional multi-season or advanced metrics.
     */
    class StatsManager
    {
    public:
        // A structure to hold an all-time record for a specific stat
        // (We only do numeric "max" approach as an example)
        struct AllTimeRecord
        {
            double recordValue;
            Player* recordHolder;
        };

        // Construct empty manager
        StatsManager();

        // If you want to load players upfront:
        StatsManager(const std::vector<Player*>& initialPlayers);

        // Add or remove a player from the StatsManager’s tracking
        void registerPlayer(Player* player);
        void unregisterPlayer(Player* player);

        /**
         * Return a sorted list of (Player*, value) for a given stat, with the topN or bottomN depending on
         * ascendingOrDescending (true => ascending, false => descending).
         * E.g., for "ERA" you might want ascending = true so best ERA is 1st.
         */
        std::vector<std::pair<Player*, double>>
            getLeaders(const std::string& stat,
                int topN = 10,
                bool ascending = false);

        /**
         * (Optional) sum or average that stat for all players on a team
         * pass 'aggregateMethod' like "SUM" or "AVG" if you want either approach
         */
        double getTeamStat(Team* team,
            const std::string& stat,
            const std::string& aggregateMethod = "SUM");

        /**
         * Check and update all-time records if the player’s new stat surpasses it.
         * For demonstration, we only do the "highest" approach.
         */
        void checkAndUpdateAllTimeRecord(Player* player, const std::string& stat);

        /**
         * Retrieve the stored all-time record for a given stat.
         * If not found, return a default of {0.0, nullptr} or throw.
         */
        AllTimeRecord getAllTimeRecord(const std::string& stat) const;

        /**
         * If you keep multi-season data, you might also provide a function to specify "year"
         * or "season index" for partial stats. This example does not fully implement year-based logic,
         * but we show how you might handle it.
         */
         // double getStatForSeason(Player* player, const std::string& stat, int seasonYear);

    private:
        // The list of all players we track
        std::vector<Player*> allPlayers;

        // A map of statName => AllTimeRecord. For example, "HR" => {73, pointerToPlayer}.
        std::unordered_map<std::string, AllTimeRecord> allTimeRecords;

        /**
         * Return the correct numeric value of 'stat' from the player's stats object.
         * We'll handle known stats with if-else or a small dictionary approach.
         */
        double getStatValue(Player* player, const std::string& stat) const;

        /**
         * A small helper to do the sorting logic for getLeaders.
         * We can store ascending in the lambda or capture it.
         */
    };
}
