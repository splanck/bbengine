#pragma once

#include <string>
#include <vector>

namespace BBEngine
{

    /// Represents the statistics for a single season.
    struct SeasonStats
    {
        // Basic identifiers
        int year;
        std::string team;  // Which team the stats were earned with

        // Hitting stats
        int atBats;
        int hits;
        int doubles;
        int triples;
        int homeRuns;
        int walks;
        int strikeouts;   // Batter strikeouts
        int runs;
        int RBIs;

        // Pitching stats
        double inningsPitched; // Allows for fractional innings (e.g., 6.1)
        int earnedRuns;
        int wins;
        int losses;
        int saves;
        int pitcherStrikeouts;
        int pitcherWalks;

        // Fielding stats
        int putouts;
        int assists;
        int errors;

        // Default constructor initializes all raw stats to zero.
        SeasonStats();

        // --- Calculated Hitting Statistics ---
        // Returns batting average (hits divided by at-bats) or 0 if no at-bats.
        double battingAverage() const;

        // --- Calculated Pitching Statistics ---
        // Returns ERA = (earnedRuns * 9) / inningsPitched, or 0 if no innings pitched.
        double ERA() const;

        // --- Calculated Fielding Statistics ---
        // Returns fielding percentage = (putouts + assists) / (putouts + assists + errors).
        double fieldingPercentage() const;
    };

    /// A class to aggregate multiple seasons of stats for a player.
    class PlayerStats
    {
    public:
        /// Add the stats for a new season.
        void addSeason(const SeasonStats & season);

        /// Get a const reference to all seasons.
        const std::vector<SeasonStats> & getSeasons() const;

        // --- Calculated Career Statistics ---
        // Overall career batting average.
        double careerBattingAverage() const;

        // Overall career ERA.
        double careerERA() const;

        // Overall career fielding percentage.
        double careerFieldingPercentage() const;

    private:
        std::vector<SeasonStats> seasons;
    };

} // namespace BBEngine
