#pragma once
#pragma once

#include <vector>
#include "Team.h"
#include "Schedule.h"
#include "Standings.h"

namespace BBEngine
{
    class Season
    {
    public:
        /**
         * Constructor: pass references to the teams, schedule, and standings for this season.
         */
        Season(const std::vector<Team*>& participatingTeams,
            Schedule* sched,
            Standings* stand);

        /**
         * Optional: mark that the season has started (set any flags).
         * Or you can do it all in the constructor.
         */
        void startSeason();

        /**
         * For a day-based sim: simulate the games on a particular date.
         * Usually you'd create a GameManager for each game and run it,
         * then record the final scores in schedule & standings.
         */
        void simulateDay(int date);

        /**
         * If you want to manually record a game result (bypassing a GameManager).
         */
        void recordGameResult(int gameID, int awayScore, int homeScore);

        /**
         * Check if the season is over (e.g., all scheduled games are completed).
         */
        bool isSeasonOver() const;

        /**
         * Called when the season is fully done: finalize standings, handle playoffs, etc.
         */
        void finalizeSeason();

        /**
         * Accessors for data
         */
        Schedule* getSchedule() const;
        Standings* getStandings() const;
        std::vector<Team*> getTeams() const;

    private:
        // Data members
        std::vector<Team*> teams;
        Schedule* schedule;
        Standings* standings;

        bool seasonStarted;
        bool seasonComplete;

        /**
         * Private helper: check if all schedule games are completed => can end season
         */
        bool allGamesCompleted() const;
    };

} // namespace BBEngine
