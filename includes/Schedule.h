#pragma once

#include <vector>
#include <string>
#include "Team.h"
#include "BoxScore.h" // If you want to reference a BoxScore pointer

namespace BBEngine
{
    // A simple struct representing one scheduled game
    struct GameEntry
    {
        int        gameID;       // A unique identifier for the game
        int        date;         // Could be day # in the season, or yyyymmdd
        Team* homeTeam;
        Team* awayTeam;

        // Optional metadata
        std::string stadium;     // If you want to store which stadium, or do so in Team
        bool       completed;
        int        homeScore;
        int        awayScore;

        BoxScore* boxScore;     // If you attach a box score after the game is completed

        GameEntry()
            : gameID(-1), date(0), homeTeam(nullptr), awayTeam(nullptr),
            completed(false), homeScore(0), awayScore(0),
            boxScore(nullptr)
        {
        }
    };

    class Schedule
    {
    public:
        /**
         * Default constructor: empty schedule.
         */
        Schedule();

        /**
         * Possibly a constructor that takes a set of teams or config,
         * if you want to auto-generate in the constructor.
         */
         // Schedule(const std::vector<Team*>& teams, int totalGamesPerTeam);

         /**
          * Add or generate scheduled games. If you have more advanced logic, you can
          * place it here or in generateSchedule(...).
          */
        void generateSchedule(const std::vector<Team*>& teams);

        /**
         * Return all games in this schedule, if needed.
         */
        const std::vector<GameEntry>& getAllGames() const;

        /**
         * Return all games scheduled on a given date.
         */
        std::vector<GameEntry> getGamesOn(int date) const;

        /**
         * Record final results for a game (set completed=true, store home/awayScore).
         * Potentially notifies standings or season logic (not shown here).
         */
        void recordGameResult(int gameID, int awayScore, int homeScore);

        /**
         * Mark a game as completed and attach a BoxScore if you track detailed stats.
         */
        void setGameCompleted(int gameID, BoxScore* box);

        /**
         * Mark or move a game to a new date if postponed.
         */
        void postponeGame(int gameID, int newDate);

    private:
        /**
         * Helper to find an index in 'games' by gameID.
         * Returns -1 if not found.
         */
        int findGameIndexByID(int gameID) const;

        // The container of all scheduled games for this season
        std::vector<GameEntry> games;

        // Next ID to assign to a new game
        int nextGameID;
    };

} // namespace BBEngine
