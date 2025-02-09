#include "PlayerStats.h"

namespace BBEngine
{
    // -------------------------------------------------------------
    // Constructor
    // -------------------------------------------------------------
    PlayerStats::PlayerStats()
        : atBats(0), hits(0), doublesHit(0), triplesHit(0), homeRuns(0),
        walks(0), strikeouts(0), runs(0), rbis(0),
        inningsPitched(0.0), hitsAllowed(0), runsAllowed(0), earnedRuns(0),
        walksAllowed(0), pitcherStrikeouts(0), wins(0), losses(0), saves(0)
    {
        // All counters are zero at initialization.
    }

    // -------------------------------------------------------------
    // Offensive Stats: Increment Methods
    // -------------------------------------------------------------
    void PlayerStats::addAtBats(int count)
    {
        if (count < 0) return; // Or throw an exception if you prefer
        atBats += count;
    }

    void PlayerStats::addHits(int count)
    {
        if (count < 0) return;
        hits += count;
    }

    void PlayerStats::addDoubles(int count)
    {
        if (count < 0) return;
        doublesHit += count;
        addHits(count); // Usually a double also counts as a hit
    }

    void PlayerStats::addTriples(int count)
    {
        if (count < 0) return;
        triplesHit += count;
        addHits(count); // A triple is also a hit
    }

    void PlayerStats::addHomeRuns(int count)
    {
        if (count < 0) return;
        homeRuns += count;
        addHits(count); // A home run is also a hit
    }

    void PlayerStats::addWalks(int count)
    {
        if (count < 0) return;
        walks += count;
    }

    void PlayerStats::addStrikeouts(int count)
    {
        if (count < 0) return;
        strikeouts += count;
    }

    void PlayerStats::addRuns(int count)
    {
        if (count < 0) return;
        runs += count;
    }

    void PlayerStats::addRBIs(int count)
    {
        if (count < 0) return;
        rbis += count;
    }

    // -------------------------------------------------------------
    // Pitching Stats: Increment Methods
    // -------------------------------------------------------------
    void PlayerStats::addInningsPitched(double innings)
    {
        if (innings < 0.0) return;
        inningsPitched += innings;
    }

    void PlayerStats::addHitsAllowed(int count)
    {
        if (count < 0) return;
        hitsAllowed += count;
    }

    void PlayerStats::addRunsAllowed(int count)
    {
        if (count < 0) return;
        runsAllowed += count;
    }

    void PlayerStats::addEarnedRuns(int count)
    {
        if (count < 0) return;
        earnedRuns += count;
    }

    void PlayerStats::addWalksAllowed(int count)
    {
        if (count < 0) return;
        walksAllowed += count;
    }

    void PlayerStats::addPitcherStrikeouts(int count)
    {
        if (count < 0) return;
        pitcherStrikeouts += count;
    }

    void PlayerStats::addWins(int count)
    {
        if (count < 0) return;
        wins += count;
    }

    void PlayerStats::addLosses(int count)
    {
        if (count < 0) return;
        losses += count;
    }

    void PlayerStats::addSaves(int count)
    {
        if (count < 0) return;
        saves += count;
    }

    // -------------------------------------------------------------
    // Derived Stats: Offense
    // -------------------------------------------------------------
    double PlayerStats::getBattingAverage() const
    {
        if (atBats == 0)
            return 0.0;
        return static_cast<double>(hits) / atBats;
    }

    double PlayerStats::getOnBasePercentage() const
    {
        int denominator = atBats + walks; // ignoring sacFlys for simplicity
        if (denominator == 0)
            return 0.0;
        return static_cast<double>(hits + walks) / denominator;
    }

    // -------------------------------------------------------------
    // Derived Stats: Pitching
    // -------------------------------------------------------------
    double PlayerStats::getERA() const
    {
        if (inningsPitched <= 0.0)
            return 0.0;
        return (earnedRuns * 9.0) / inningsPitched;
    }

    double PlayerStats::getWHIP() const
    {
        if (inningsPitched <= 0.0)
            return 0.0;
        return static_cast<double>(walksAllowed + hitsAllowed) / inningsPitched;
    }

    // -------------------------------------------------------------
    // Accessors for Raw Counters (Optional)
    // -------------------------------------------------------------
    int  PlayerStats::getAtBats()         const { return atBats; }
    int  PlayerStats::getHits()           const { return hits; }
    int  PlayerStats::getDoubles()        const { return doublesHit; }
    int  PlayerStats::getTriples()        const { return triplesHit; }
    int  PlayerStats::getHomeRuns()       const { return homeRuns; }
    int  PlayerStats::getWalks()          const { return walks; }
    int  PlayerStats::getStrikeouts()     const { return strikeouts; }
    int  PlayerStats::getRuns()           const { return runs; }
    int  PlayerStats::getRBIs()           const { return rbis; }

    double PlayerStats::getInningsPitched()      const { return inningsPitched; }
    int    PlayerStats::getHitsAllowed()         const { return hitsAllowed; }
    int    PlayerStats::getRunsAllowed()         const { return runsAllowed; }
    int    PlayerStats::getEarnedRuns()          const { return earnedRuns; }
    int    PlayerStats::getWalksAllowed()        const { return walksAllowed; }
    int    PlayerStats::getPitcherStrikeouts()   const { return pitcherStrikeouts; }
    int    PlayerStats::getWins()                const { return wins; }
    int    PlayerStats::getLosses()              const { return losses; }
    int    PlayerStats::getSaves()               const { return saves; }

    // -------------------------------------------------------------
    // Utility: Reset for new season
    // -------------------------------------------------------------
    void PlayerStats::resetStats()
    {
        // Offensive
        atBats = 0;
        hits = 0;
        doublesHit = 0;
        triplesHit = 0;
        homeRuns = 0;
        walks = 0;
        strikeouts = 0;
        runs = 0;
        rbis = 0;

        // Pitching
        inningsPitched = 0.0;
        hitsAllowed = 0;
        runsAllowed = 0;
        earnedRuns = 0;
        walksAllowed = 0;
        pitcherStrikeouts = 0;
        wins = 0;
        losses = 0;
        saves = 0;
    }

} // namespace BBEngine
