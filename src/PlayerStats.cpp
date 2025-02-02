#include "PlayerStats.h"

namespace BBEngine
{
    // ----- SeasonStats Implementation -----

    SeasonStats::SeasonStats()
    {
        // Initialize basic identifiers
        year = 0;
        team = "";

        // Initialize Hitting stats
        atBats = 0;
        hits = 0;
        doubles = 0;
        triples = 0;
        homeRuns = 0;
        walks = 0;
        strikeouts = 0;
        runs = 0;
        RBIs = 0;

        // Initialize Pitching stats
        inningsPitched = 0.0;
        earnedRuns = 0;
        wins = 0;
        losses = 0;
        saves = 0;
        pitcherStrikeouts = 0;
        pitcherWalks = 0;

        // Initialize Fielding stats
        putouts = 0;
        assists = 0;
        errors = 0;
    }

    double SeasonStats::battingAverage() const
    {
        return (atBats > 0) ? static_cast<double>(hits) / atBats : 0.0;
    }

    double SeasonStats::ERA() const
    {
        return (inningsPitched > 0.0) ? (earnedRuns * 9.0) / inningsPitched : 0.0;
    }

    double SeasonStats::fieldingPercentage() const
    {
        int totalChances = putouts + assists + errors;
        
        return (totalChances > 0) ? static_cast<double>(putouts + assists) / totalChances : 0.0;
    }

    // ----- PlayerStats Implementation -----

    void PlayerStats::addSeason(const SeasonStats & season)
    {
        seasons.push_back(season);
    }

    const std::vector<SeasonStats> & PlayerStats::getSeasons() const
    {
        return seasons;
    }

    double PlayerStats::careerBattingAverage() const
    {
        int totalAtBats = 0;
        int totalHits = 0;

        for (const auto & season : seasons)
        {
            totalAtBats += season.atBats;
            totalHits += season.hits;
        }

        return (totalAtBats > 0) ? static_cast<double>(totalHits) / totalAtBats : 0.0;
    }

    double PlayerStats::careerERA() const
    {
        double totalInnings = 0.0;
        int totalEarnedRuns = 0;
        for (const auto & season : seasons)
        {
            totalInnings += season.inningsPitched;
            totalEarnedRuns += season.earnedRuns;
        }
        return (totalInnings > 0.0) ? (totalEarnedRuns * 9.0) / totalInnings : 0.0;
    }

    double PlayerStats::careerFieldingPercentage() const
    {
        int totalPutouts = 0;
        int totalAssists = 0;
        int totalErrors = 0;

        for (const auto & season : seasons)
        {
            totalPutouts += season.putouts;
            totalAssists += season.assists;
            totalErrors += season.errors;
        }

        int totalChances = totalPutouts + totalAssists + totalErrors;
        
        return (totalChances > 0) ? static_cast<double>(totalPutouts + totalAssists) / totalChances : 0.0;
    }

} // namespace BBEngine
