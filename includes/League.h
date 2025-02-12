#pragma once

#include <vector>
#include <string>
#include "Team.h"
#include "Season.h"
// If you have manager classes, e.g. #include "StatsManager.h"

namespace BBEngine
{
    class League
    {
    public:
        /**
         * Basic constructor: can be empty or can take some config arguments.
         */
        League();

        /**
         * destructor if needed
         */
        ~League();

        /**
         * Add a new Team to the league. If you track multiple levels,
         * you might have addMLBTeam(), addAAATeam(), etc.
         */
        void addTeam(Team* team);

        /**
         * Start a new season with the currently known teams.
         * This method creates a new Season object, sets it as currentSeason,
         * possibly generates a schedule, stands, etc.
         */
        void startNewSeason();

        /**
         * If you want day-by-day simulation, you can call this to move forward a day.
         * The league will ask currentSeason->simulateDay(currentDate).
         * Possibly increments currentDate or calls finalizeSeason if done.
         */
        void advanceOneDay();

        /**
         * If the current season is done, finalize it (move to pastSeasons, etc.)
         */
        void finishSeason();

        /**
         * Accessors
         */
        Season* getCurrentSeason() const;
        const std::vector<Season*>& getPastSeasons() const;

        // If you have manager references, e.g. StatsManager* getStatsManager()...

        /**
         * If you want to store day or year in the league:
         */
        int getCurrentDate() const;
        void setCurrentDate(int d);

    private:
        // Data members

        // The set of all teams in the league. Could subdivide by level if you want.
        std::vector<Team*> allTeams;

        // The current active season
        Season* currentSeason;

        // Archive of finished seasons
        std::vector<Season*> pastSeasons;

        // If you have manager references:
        // StatsManager* statsManager;
        // InjuryManager* injuryManager;
        // TradeManager* tradeManager;

        // Possibly keep track of current date / year
        int currentDate;

        // Helper to check if currentSeason is not null
        bool hasActiveSeason() const;
    };
}
