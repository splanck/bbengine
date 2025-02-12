#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "Team.h"

namespace BBEngine
{
    // Each TeamRecord holds the stats for one team in the standings
    struct TeamRecord
    {
        Team* team;           // pointer to the actual Team
        int   wins;           // number of wins
        int   losses;         // number of losses
        std::string division; // optional: if you track divisions
        double gamesBehind;   // computed in updateStandings()

        // constructor
        TeamRecord(Team* t, const std::string& div = "")
            : team(t), wins(0), losses(0), division(div), gamesBehind(0.0) {
        }
    };

    class Standings
    {
    public:
        /**
         * Constructor: pass in a list of Team*.
         * Optionally store or detect divisions. For simplicity, we ignore divisions or pass them as the same.
         */
        Standings(const std::vector<Team*>& allTeams, const std::string& defaultDivision = "");

        // --- Updating Records ---
        /**
         * recordWin / recordLoss for a given Team.
         * If the team is not found, do nothing or throw.
         */
        void recordWin(Team* team);
        void recordLoss(Team* team);

        /**
         * Alternatively, you can do recordGameResult(winner, loser).
         */
        void recordGameResult(Team* winner, Team* loser);

        // --- Sorting & Ranking ---
        /**
         * updateStandings: sorts each division or the entire set by best record
         * also calls computeGamesBehind() to fill the gamesBehind field.
         */
        void updateStandings();

        /**
         * Return the entire sorted standings. If you have divisions, you can unify them or do them separately.
         */
        std::vector<TeamRecord> getOverallStandings() const;

        // If you track divisions, a method to get a sub-list:
        std::vector<TeamRecord> getStandingsForDivision(const std::string& division) const;

        // Accessor for a single team's wins/losses
        int getWins(Team* team) const;
        int getLosses(Team* team) const;

    private:
        // The container for all records
        std::vector<TeamRecord> records;

        // Helper to find a record for a team
        TeamRecord* findRecord(Team* team);

        // If you want to do advanced logic for each division, you might do it here
        void computeGamesBehind();

        // A custom sort function that sorts by W-L. We might do: sort by (wins descending, losses ascending).
        static bool teamRecordCompare(const TeamRecord& a, const TeamRecord& b);
    };

} // namespace BBEngine
