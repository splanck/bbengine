#pragma once

#include <string>
#include <vector>
#include <cassert> // or <stdexcept>

namespace BBEngine
{
    /**
     * A single-game batting line for a player:
     *  - Player pointer (if you store actual references)
     *  - Stats for that game (AB, hits, doubles, HR, walks, strikeouts, runs, etc.)
     */
    class Player;

    struct BoxScoreBattingLine
    {
        Player* player = nullptr;
        int atBats = 0;
        int hits = 0;
        int doubles_ = 0; // underscore suffix to avoid naming conflict
        int triples = 0;
        int homeRuns = 0;
        int runsBattedIn = 0;
        int walks = 0;
        int strikeouts = 0;
        int runsScored = 0;
        // Add more single-game stats if needed (SB, HBP, SF, etc.)
    };

    /**
     * A single-game pitching line for a pitcher:
     *  - Player pointer (the actual pitcher)
     *  - Stats for that appearance (innings pitched, hitsAllowed, runsAllowed, etc.)
     */
    struct BoxScorePitchingLine
    {
        Player* pitcher = nullptr;
        double inningsPitched = 0.0;
        int hitsAllowed = 0;
        int runsAllowed = 0;
        int earnedRuns = 0;
        int walksAllowed = 0;
        int strikeouts = 0;

        bool decisionWin = false;
        bool decisionLoss = false;
        bool decisionSave = false;
        // Add more stats if needed (pitch count, etc.)
    };

    /**
     * BoxScore class for a single game, allowing incremental updates
     * as the game progresses. By the end, it can provide a final summary.
     */
    class BoxScore
    {
    public:
        /**
         * Constructor with the names of home and away teams or references to actual Team objects.
         * For simplicity, we'll just store strings here.
         */
        BoxScore(const std::string& homeTeamName,
            const std::string& awayTeamName);

        // ----------------------
        // High-level Game Info
        // ----------------------
        const std::string& getHomeTeamName() const;
        const std::string& getAwayTeamName() const;

        void setHomeTeamName(const std::string& name);
        void setAwayTeamName(const std::string& name);

        // -----------------------------------------------------
        // Methods to Manage Batting Lines (Lineup & Statistics)
        // -----------------------------------------------------
        /**
         * Add a batter to either the home or away lineup.
         * This will create a new BoxScoreBattingLine entry for that player.
         */
        void addBatterToLineup(bool isHome, Player* player);

        /**
         * Update hitting stats for a single at-bat or plate appearance.
         */
        void recordAtBat(bool isHome,
            Player* player,
            int atBats = 1,
            int hits = 0,
            int doubles_ = 0,
            int triples = 0,
            int homeRuns = 0,
            int runsBattedIn = 0,
            int walks = 0,
            int strikeouts = 0,
            int runsScored = 0);

        // Convenience methods:
        void recordWalk(bool isHome, Player* batter);
        void recordStrikeout(bool isHome, Player* batter);
        void recordHit(bool isHome, Player* batter,
            bool isDouble = false,
            bool isTriple = false,
            bool isHomerun = false);

        /**
         * Access to the lineups
         */
        const std::vector<BoxScoreBattingLine>& getHomeBattingLines() const;
        const std::vector<BoxScoreBattingLine>& getAwayBattingLines() const;

        /**
         * Team run totals (just sums of runsScored if you prefer).
         */
        int getHomeTeamRuns() const;
        int getAwayTeamRuns() const;

        // -------------------------------------------------------
        // Methods to Manage Pitching Lines (Appearance & Updates)
        // -------------------------------------------------------
        /**
         * Add a pitcher for home or away side.
         * The next appearance is appended to the vector.
         */
        void addPitcher(bool isHome, Player* pitcher);

        /**
         * Incrementally update a pitcher's stats, e.g., after an inning or partial inning.
         */
        void recordPitching(bool isHome,
            Player* pitcher,
            double inningsPitchedDelta,
            int hitsAllowed,
            int runsAllowed,
            int earnedRuns,
            int walksAllowed,
            int strikeouts);

        // Mark a pitcher with a decision if applicable (win, loss, save).
        void assignPitchingDecision(bool isHome,
            Player* pitcher,
            bool win,
            bool loss,
            bool save);

        // Access to pitching lines
        const std::vector<BoxScorePitchingLine>& getHomePitchingLines() const;
        const std::vector<BoxScorePitchingLine>& getAwayPitchingLines() const;

    private:
        // Helper to find (or create) a batting line for a given player (home or away).
        BoxScoreBattingLine& findBattingLine(bool isHome, Player* player);

        // Helper to find (or create) a pitching line for a given pitcher (home or away).
        BoxScorePitchingLine& findPitchingLine(bool isHome, Player* pitcher);

    private:
        // Basic game info
        std::string homeTeam;
        std::string awayTeam;

        // Offense
        std::vector<BoxScoreBattingLine> homeBattingLines;
        std::vector<BoxScoreBattingLine> awayBattingLines;

        // Pitching
        std::vector<BoxScorePitchingLine> homePitchingLines;
        std::vector<BoxScorePitchingLine> awayPitchingLines;
    };

} // namespace BBEngine
