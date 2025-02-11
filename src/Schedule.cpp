#include "Schedule.h"
#include <algorithm>  // for std::find_if

namespace BBEngine
{
    Schedule::Schedule()
        : nextGameID(1000)  // Start from 1000 if you like
    {
    }

    void Schedule::generateSchedule(const std::vector<Team*>& teams)
    {
        // For demonstration, let's create a tiny schedule:
        // Suppose each team plays each other team once at home on date=1, once away on date=2.
        // This is just an example. Real logic would be more elaborate.

        for (size_t i = 0; i < teams.size(); ++i)
        {
            for (size_t j = 0; j < teams.size(); ++j)
            {
                if (i == j) continue; // skip same team
                // Create a home game for i vs j on date=1
                GameEntry g;
                g.gameID = nextGameID++;
                g.date = 1;  // same date for demonstration
                g.homeTeam = teams[i];
                g.awayTeam = teams[j];
                g.stadium = ""; // or g.homeTeam->getStadium() if you have that
                g.completed = false;
                g.homeScore = 0;
                g.awayScore = 0;
                g.boxScore = nullptr;
                games.push_back(g);

                // Create an away game for i vs j => means j is home, i is away => on date=2
                GameEntry g2;
                g2.gameID = nextGameID++;
                g2.date = 2;
                g2.homeTeam = teams[j];
                g2.awayTeam = teams[i];
                g2.stadium = "";
                g2.completed = false;
                g2.homeScore = 0;
                g2.awayScore = 0;
                g2.boxScore = nullptr;
                games.push_back(g2);
            }
        }
    }

    const std::vector<GameEntry>& Schedule::getAllGames() const
    {
        return games;
    }

    std::vector<GameEntry> Schedule::getGamesOn(int date) const
    {
        std::vector<GameEntry> result;
        for (const auto& gm : games)
        {
            if (gm.date == date)
            {
                result.push_back(gm);
            }
        }
        return result;
    }

    void Schedule::recordGameResult(int gameID, int awayScore, int homeScore)
    {
        int idx = findGameIndexByID(gameID);
        if (idx < 0)
        {
            // Could throw or log
            return;
        }
        games[idx].awayScore = awayScore;
        games[idx].homeScore = homeScore;
        games[idx].completed = true;

        // In a real sim, you'd call something like: updateStandings(awayTeam, homeTeam, awayScore, homeScore);
        // or notify the Season that the game is done.
    }

    void Schedule::setGameCompleted(int gameID, BoxScore* box)
    {
        int idx = findGameIndexByID(gameID);
        if (idx < 0) return;
        games[idx].completed = true;
        games[idx].boxScore = box;
        if (box)
        {
            // we might also read final score from box
            int awayRuns = box->getAwayTeamRuns();
            int homeRuns = box->getHomeTeamRuns();
            games[idx].awayScore = awayRuns;
            games[idx].homeScore = homeRuns;
        }
    }

    void Schedule::postponeGame(int gameID, int newDate)
    {
        int idx = findGameIndexByID(gameID);
        if (idx < 0) return;
        // Mark old date as changed
        games[idx].date = newDate;
        games[idx].completed = false; // if it was completed, maybe it's weird, but let's do it
    }

    int Schedule::findGameIndexByID(int gameID) const
    {
        for (size_t i = 0; i < games.size(); ++i)
        {
            if (games[i].gameID == gameID)
                return static_cast<int>(i);
        }
        return -1;
    }

} // namespace BBEngine
