#include "Season.h"
#include <iostream>  // for debug prints or logs
#include <cassert>

namespace BBEngine
{
    Season::Season(const std::vector<Team*>& participatingTeams,
        Schedule* sched,
        Standings* stand)
        : teams(participatingTeams),
        schedule(sched),
        standings(stand),
        seasonStarted(false),
        seasonComplete(false)
    {
        // Possibly validate that sched and stand are not null, 
        // that teams is not empty, etc.
    }

    void Season::startSeason()
    {
        if (seasonStarted)
        {
            std::cerr << "[Season] Already started.\n";
            return;
        }
        seasonStarted = true;
        seasonComplete = false;
        // We might do further initialization if needed.
        // E.g. "Day 1" or "currentDate = min date in schedule"
    }

    void Season::simulateDay(int date)
    {
        if (!schedule)
        {
            std::cerr << "[Season] No schedule assigned!\n";
            return;
        }
        if (!standings)
        {
            std::cerr << "[Season] No standings assigned!\n";
            return;
        }

        // 1. Retrieve the day's games
        auto todaysGames = schedule->getGamesOn(date);

        // 2. For each game, we "simulate" it. In a real system, we'd create a GameManager, run it, get final boxscore.
        // For demonstration, we'll do a mock approach: we just produce random or hard-coded final scores, 
        // then call recordGameResult.
        int i = 0;
        for (auto& g : todaysGames)
        {
            if (!g.completed)
            {
                // We'll produce some mock final score
                // or we might do real simulation with a GameManager.
                int awayScore = (rand() % 5); // random 0..4
                int homeScore = (rand() % 5);
                // We'll increment i in the scores just to differentiate.
                awayScore += i;
                homeScore += (i / 2);
                i++;

                recordGameResult(g.gameID, awayScore, homeScore);
            }
        }

        // after we do that, we can check if the season is over
        if (allGamesCompleted())
        {
            finalizeSeason();
        }
    }

    void Season::recordGameResult(int gameID, int awayScore, int homeScore)
    {
        // 1. Mark the game in schedule as completed
        if (schedule)
        {
            schedule->recordGameResult(gameID, awayScore, homeScore);
        }
        // 2. Update standings
        if (standings)
        {
            // If awayScore > homeScore => away wins
            // else home wins or tie => let's say tie goes to home if you don't allow ties.
            Team* awayTeam = nullptr;
            Team* homeTeam = nullptr;

            // We can find the schedule entry again to see who was away/home
            // or if schedule->recordGameResult also updates a reference
            // For demonstration, let's do this:
            if (!schedule)
                return;

            auto allG = schedule->getAllGames();
            for (const auto& g : allG)
            {
                if (g.gameID == gameID)
                {
                    awayTeam = g.awayTeam;
                    homeTeam = g.homeTeam;
                    break;
                }
            }
            if (!awayTeam || !homeTeam)
            {
                std::cerr << "[Season] recordGameResult: Could not find teams for gameID=" << gameID << "\n";
                return;
            }

            if (awayScore > homeScore)
            {
                standings->recordGameResult(awayTeam, homeTeam);  // away wins, home loses
            }
            else
            {
                standings->recordGameResult(homeTeam, awayTeam);  // home wins, away loses
            }

            // optionally standings->updateStandings();
            // we might do it once a day, not after every single game.
            standings->updateStandings();
        }
    }

    bool Season::isSeasonOver() const
    {
        return seasonComplete;
    }

    void Season::finalizeSeason()
    {
        if (!seasonComplete)
        {
            // We can do final updates: e.g. finalize standings or declare champion if you want
            if (standings)
            {
                // Possibly one last update
                standings->updateStandings();
            }
            seasonComplete = true;
        }
    }

    Schedule* Season::getSchedule() const
    {
        return schedule;
    }

    Standings* Season::getStandings() const
    {
        return standings;
    }

    std::vector<Team*> Season::getTeams() const
    {
        return teams;
    }

    bool Season::allGamesCompleted() const
    {
        if (!schedule) return true;
        auto allG = schedule->getAllGames();
        for (const auto& g : allG)
        {
            if (!g.completed) return false;
        }
        return true;
    }
}
