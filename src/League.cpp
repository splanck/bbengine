#include "League.h"
#include <iostream>  // debug prints
#include <cassert>   // for asserts
#include <stdexcept> // if we throw exceptions

// We assume "Season.h", "Schedule.h", "Standings.h" etc. are included
// inside "League.h" or "Season.h".

namespace BBEngine
{
    League::League()
        : currentSeason(nullptr),
        currentDate(1)
    {
        // Possibly initialize manager pointers here if needed
        // statsManager = new StatsManager();
        // etc.
    }

    League::~League()
    {
        // Clean up if you're dynamically allocating Seasons
        // If we allocated currentSeason, we should delete it or move it to pastSeasons
        if (currentSeason)
        {
            // optional: if you also push it to pastSeasons or destroy
            // for demonstration, let's just do:
            delete currentSeason;
            currentSeason = nullptr;
        }

        // If we have a list of dynamic Seasons in pastSeasons, free them:
        for (auto* s : pastSeasons)
        {
            delete s;
        }
        pastSeasons.clear();

        // If you allocated manager objects here, free them as well.
        // e.g. delete statsManager; ...
    }

    void League::addTeam(Team* team)
    {
        if (!team)
        {
            std::cerr << "[League] addTeam called with null team.\n";
            return;
        }
        // optionally check for duplicates
        allTeams.push_back(team);
    }

    void League::startNewSeason()
    {
        // If there's already a currentSeason, check if it's completed
        if (hasActiveSeason() && !currentSeason->isSeasonOver())
        {
            std::cerr << "[League] There's already an active season that's not finished.\n";
            return;
        }

        // if we have a finished currentSeason, we can push it to pastSeasons
        if (hasActiveSeason() && currentSeason->isSeasonOver())
        {
            pastSeasons.push_back(currentSeason);
            currentSeason = nullptr;
        }

        if (allTeams.empty())
        {
            std::cerr << "[League] No teams found. Can't start new season.\n";
            return;
        }

        // Create a new Season. We'll also create a new Schedule & Standings for it.
        // Or we can do it outside. For demonstration:
        Schedule* sched = new Schedule();
        sched->generateSchedule(allTeams);

        Standings* stand = new Standings(allTeams);

        currentSeason = new Season(allTeams, sched, stand);
        currentSeason->startSeason();

        // reset the league's currentDate if you want
        currentDate = 1;

        std::cout << "[League] New season started with "
            << allTeams.size() << " teams.\n";
    }

    void League::advanceOneDay()
    {
        if (!hasActiveSeason())
        {
            std::cerr << "[League] No active season to advance.\n";
            return;
        }
        if (currentSeason->isSeasonOver())
        {
            std::cerr << "[League] Season is already over.\n";
            return;
        }

        std::cout << "[League] Advancing day " << currentDate << "\n";

        // call season->simulateDay(currentDate)
        currentSeason->simulateDay(currentDate);

        // after we simulate, if the season ended, we can finalize or not
        if (currentSeason->isSeasonOver())
        {
            finishSeason();
            return;
        }

        // increment day
        currentDate++;
    }

    void League::finishSeason()
    {
        if (!hasActiveSeason())
        {
            std::cerr << "[League] finishSeason() but no active season.\n";
            return;
        }
        if (!currentSeason->isSeasonOver())
        {
            // forcibly finalize it anyway, or do a check first
            currentSeason->finalizeSeason();
        }
        // Move it to pastSeasons
        pastSeasons.push_back(currentSeason);
        currentSeason = nullptr;

        std::cout << "[League] Season finished and archived. Ready for next.\n";
    }

    Season* League::getCurrentSeason() const
    {
        return currentSeason;
    }

    const std::vector<Season*>& League::getPastSeasons() const
    {
        return pastSeasons;
    }

    int League::getCurrentDate() const
    {
        return currentDate;
    }

    void League::setCurrentDate(int d)
    {
        if (d < 1)
        {
            std::cerr << "[League] setCurrentDate: invalid day.\n";
            return;
        }
        currentDate = d;
    }

    bool League::hasActiveSeason() const
    {
        return (currentSeason != nullptr);
    }
}
