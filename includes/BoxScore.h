#pragma once

#include <string>
#include <vector>
#include <optional>
#include <stdexcept>
#include "Player.h"

namespace BBEngine
{
    /// Represents a single-game offensive line for a player.
    struct BoxScoreBattingLine
    {
        Player* player = nullptr; // Pointer to the actual Player instance
        int battingOrder = -1;    // Typically 1-based in baseball
        int atBats = 0;
        int runs = 0;
        int hits = 0;
        int doubles = 0;
        int triples = 0;
        int homeRuns = 0;
        int runsBattedIn = 0;
        int walks = 0;
        int strikeouts = 0;
        bool active = true; // If set to false, it means this slot was replaced (e.g., pinch hitter)
        // ... add more single-game stats as needed (SB, HBP, SF, etc.)
    };

    /// Represents a single-game pitching line for a pitcher.
    struct BoxScorePitchingLine
    {
        Player* pitcher = nullptr;
        int appearanceOrder = 0; // 0 = starter, 1 = first reliever, etc.
        double inningsPitched = 0.0; 
        int hitsAllowed = 0;
        int runsAllowed = 0;
        int earnedRuns = 0;
        int walksAllowed = 0;
        int strikeouts = 0;
        bool decisionWin = false;
        bool decisionLoss = false;
        bool decisionSave = false;
        // ... add more single-game stats as needed (pitch count, etc.)
    };

    enum class TeamSide
    {
        Home,
        Away
    };

    /// BoxScore class for a single game, allowing incremental updates as the game progresses.
    class BoxScore
    {
    public:
        BoxScore(const std::string& homeTeamName,
                 const std::string& awayTeamName,
                 const std::string& ballparkName,
                 int attendanceCount);

        // ----------------------
        // High-level Game Info
        // ----------------------
        const std::string& getHomeTeamName() const;
        const std::string& getAwayTeamName() const;
        const std::string& getBallpark()    const;
        int getAttendance() const;

        void setHomeTeamName(const std::string& name);
        void setAwayTeamName(const std::string& name);
        void setBallpark(const std::string& name);
        void setAttendance(int count);

        // -----------------------------------------------------
        // Methods to Manage Batting Lines (Lineup & Statistics)
        // -----------------------------------------------------

        /**
         * Add a batter to either the home or away lineup.
         *  - 'battingOrder' is typically 1..9 in traditional baseball, but you can adapt as needed.
         *  - If that battingOrder is already taken by another active batter, this throws an exception.
         */
        void addBatterToLineup(TeamSide side, Player* player, int battingOrder);

        /**
         * Replace an existing batter (e.g., pinch hitter) in the same battingOrder.
         *  - The existing BoxScoreBattingLine is marked inactive (active=false).
         *  - A new BoxScoreBattingLine is inserted for the new player at the same battingOrder.
         */
        void replaceBatter(TeamSide side, Player* newPlayer, int battingOrder);

        /**
         * Retrieve the next batter in the lineup, based on an internal "next batter index" for each team.
         *  - This increments the index and wraps around after the last batter in the lineup.
         *  - Skips any inactive batting slots (e.g., someone replaced by a pinch hitter).
         */
        Player* getNextBatter(TeamSide side);

        /**
         * Record hitting stats for a single at-bat.
         */
        void recordAtBat(TeamSide side,
                         int battingOrder,
                         int atBats = 1,
                         int hits = 0,
                         int doubles = 0,
                         int triples = 0,
                         int homeRuns = 0,
                         int runsBattedIn = 0,
                         int walks = 0,
                         int strikeouts = 0,
                         int runsScored = 0);

        void recordWalk(TeamSide side, int battingOrder);
        void recordStrikeout(TeamSide side, int battingOrder);
        void recordHit(TeamSide side, int battingOrder,
                       bool isDouble = false,
                       bool isTriple = false,
                       bool isHomerun = false);

        // Access to the lineups
        const std::vector<BoxScoreBattingLine>& getHomeBattingLines() const;
        const std::vector<BoxScoreBattingLine>& getAwayBattingLines() const;

        // Compute total runs for each team (sum of 'runsScored' in batting lines).
        int getHomeTeamRuns() const;
        int getAwayTeamRuns() const;

        // -------------------------------------------------------
        // Methods to Manage Pitching Lines (Appearance & Updates)
        // -------------------------------------------------------
        void addPitcher(TeamSide side, Player* pitcher);
        void recordPitching(TeamSide side,
                            int appearanceOrder,
                            double inningsPitchedDelta,
                            int hitsAllowed,
                            int runsAllowed,
                            int earnedRuns,
                            int walksAllowed,
                            int strikeouts);
        void assignPitchingDecision(TeamSide side,
                                    int appearanceOrder,
                                    bool win,
                                    bool loss,
                                    bool save);

        const std::vector<BoxScorePitchingLine>& getHomePitchingLines() const;
        const std::vector<BoxScorePitchingLine>& getAwayPitchingLines() const;

    private:
        // Helper to find a reference to a batting line by batting order.
        // Throws an exception if not found.
        BoxScoreBattingLine& findBattingLine(TeamSide side, int battingOrder);

        // Helper to find a reference to a pitching line by appearance order.
        BoxScorePitchingLine& findPitchingLine(TeamSide side, int appearanceOrder);

        // Retrieve the vector of batting lines for the requested side (home/away).
        std::vector<BoxScoreBattingLine>& getBattingLines(TeamSide side);

    private:
        // Basic game info
        std::string homeTeam;
        std::string awayTeam;
        std::string ballpark;
        int attendance;

        // Offense
        std::vector<BoxScoreBattingLine> homeBattingLines;
        std::vector<BoxScoreBattingLine> awayBattingLines;

        // Pitching
        std::vector<BoxScorePitchingLine> homePitchingLines;
        std::vector<BoxScorePitchingLine> awayPitchingLines;

        // Keep track of the next appearance order for home and away pitchers
        int nextHomePitcherOrder = 0;
        int nextAwayPitcherOrder = 0;

        // Track the "next batter" index for each team.
        // This is an index in the sorted order of active batting lines (not necessarily the vector index).
        int homeNextBatterIndex = 0;
        int awayNextBatterIndex = 0;
    };

} // namespace BBEngine
