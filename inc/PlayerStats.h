#pragma once  // Use this modern include guard for convenience

#include <string>

namespace BBEngine
{
    /**
     * The PlayerStats class stores and manages a player's statistical performance
     * for hitting, pitching, etc., typically over one season (or as a cumulative record).
     * You can expand it to track multiple seasons by storing a vector of seasonal stats.
     */
    class PlayerStats
    {
    public:
        // ------------------------------------------------------
        // Constructor
        // ------------------------------------------------------
        /**
         * Default constructor: initializes all counters to zero.
         */
        PlayerStats();

        // ------------------------------------------------------
        // Raw Increment Methods - Offensive Stats
        // ------------------------------------------------------
        void addAtBats(int count);
        void addHits(int count);
        void addDoubles(int count);
        void addTriples(int count);
        void addHomeRuns(int count);
        void addWalks(int count);
        void addStrikeouts(int count);
        void addRuns(int count);
        void addRBIs(int count);

        // ------------------------------------------------------
        // Raw Increment Methods - Pitching Stats
        // ------------------------------------------------------
        void addInningsPitched(double innings);
        void addHitsAllowed(int count);
        void addRunsAllowed(int count);
        void addEarnedRuns(int count);
        void addWalksAllowed(int count);
        void addPitcherStrikeouts(int count);
        void addWins(int count);
        void addLosses(int count);
        void addSaves(int count);

        // ------------------------------------------------------
        // Derived Statistics - Offense
        // ------------------------------------------------------
        /**
         * getBattingAverage() = hits / atBats, or 0 if atBats=0.
         */
        double getBattingAverage() const;

        /**
         * getOnBasePercentage() = (hits + walks) / (atBats + walks), or 0 if (atBats+walks)=0.
         * If your sim tracks sacFlys, incorporate them if needed.
         */
        double getOnBasePercentage() const;

        // ------------------------------------------------------
        // Derived Statistics - Pitching
        // ------------------------------------------------------
        /**
         * getERA() = (earnedRuns * 9) / inningsPitched, or 0 if inningsPitched=0.
         */
        double getERA() const;

        /**
         * getWHIP() = (walksAllowed + hitsAllowed) / inningsPitched, or 0 if inningsPitched=0.
         */
        double getWHIP() const;

        // ------------------------------------------------------
        // (Optional) Basic Accessors to Raw Counters
        // ------------------------------------------------------
        int  getAtBats()       const;
        int  getHits()         const;
        int  getDoubles()      const;
        int  getTriples()      const;
        int  getHomeRuns()     const;
        int  getWalks()        const;
        int  getStrikeouts()   const;
        int  getRuns()         const;
        int  getRBIs()         const;

        double getInningsPitched()  const;
        int    getHitsAllowed()     const;
        int    getRunsAllowed()     const;
        int    getEarnedRuns()      const;
        int    getWalksAllowed()    const;
        int    getPitcherStrikeouts() const;
        int    getWins()            const;
        int    getLosses()          const;
        int    getSaves()           const;

        // ------------------------------------------------------
        // Utility - If you want to reset for a new season
        // ------------------------------------------------------
        /**
         * Resets all counters to zero (simple approach).
         * If you prefer storing multiple seasons, you'll do a separate approach.
         */
        void resetStats();

    private:
        // Offensive raw counters
        int atBats;
        int hits;
        int doublesHit;
        int triplesHit;
        int homeRuns;
        int walks;
        int strikeouts;
        int runs;
        int rbis;

        // Pitching raw counters
        double inningsPitched;   // e.g., 6.1 for 6 and 1/3
        int hitsAllowed;
        int runsAllowed;
        int earnedRuns;
        int walksAllowed;
        int pitcherStrikeouts;
        int wins;
        int losses;
        int saves;
    };

} // namespace BBEngine
