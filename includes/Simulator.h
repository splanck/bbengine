#pragma once

#include <random>
#include "PlayerAttributes.h" // So we can read pitcher & batter ratings

namespace BBEngine
{
    // An enum describing possible pitch outcomes
    enum class PitchOutcome
    {
        BALL,
        STRIKE_SWINGING,
        STRIKE_LOOKING,
        FOUL,
        BATTED_BALL_IN_PLAY
    };

    // If the ball is put in play, we can have a BattedBallOutcome
    // describing what happened. We'll keep it simple with single/double/triple/hr or out
    enum class BattedBallOutcome
    {
        SINGLE,
        DOUBLE_,
        TRIPLE,
        HOMERUN,
        OUT
    };

    // (Optional) You might define a structure for stadium context
    struct StadiumContext
    {
        double fenceDistanceLeft = 330.0; // example distances
        double fenceDistanceCenter = 400.0;
        double fenceDistanceRight = 330.0;
        // altitude, wind, etc. if you want
    };

    // Basic struct that might hold "count, pitchedBalls, manager decisions, etc."
    struct PitchContext
    {
        int balls = 0;  // 0..3
        int strikes = 0;  // 0..2
        bool intentionalWalk = false;
        bool pitchOut = false;
        // etc. if you want
    };

    /**
     * The Simulator class handles pitch-by-pitch logic. It's mostly stateless:
     * you pass in the relevant data each time you call simulatePitch(...).
     */
    class Simulator
    {
    public:
        Simulator();

        // If you want the simulator to hold its own RNG engine
        void setRandomEngine(std::mt19937 newEngine);

        /**
         * The main method that decides whether the pitch is a ball, strike, foul, or in-play
         * based on the pitcher/batter attributes, pitch context, etc.
         *
         * If the outcome is BATTED_BALL_IN_PLAY, you'll usually call computeBattedBallOutcome() next
         * to see if it's a hit or out. You can combine them if you prefer one method returning the entire result.
         */
        PitchOutcome simulatePitch(const PlayerAttributes& pitcherAttr,
            const PlayerAttributes& batterAttr,
            const StadiumContext& stadium,
            const PitchContext& pitchCtx);

        /**
         * If we have an in-play ball, let's see if it's a single/double/triple/homerun or out,
         * using random logic + partial references to stadium distances.
         *
         * This is optional if your sim does it differently or calls BoxScore to finalize.
         */
        BattedBallOutcome computeBattedBallOutcome(const PlayerAttributes& pitcherAttr,
            const PlayerAttributes& batterAttr,
            const StadiumContext& stadium);

    private:
        /**
         * e.g., returns 0..1 probability. We'll do a simplistic approach combining pitcher & batter ratings.
         */
        double getStrikeProbability(const PlayerAttributes& pitcherAttr,
            const PlayerAttributes& batterAttr,
            const PitchContext& pitchCtx);

        double getContactProbability(const PlayerAttributes& pitcherAttr,
            const PlayerAttributes& batterAttr);

        // If we do in-play distance logic, we can do a simple function
        double estimateHitDistance(const PlayerAttributes& pitcherAttr,
            const PlayerAttributes& batterAttr);

        // Our random engine
        std::mt19937 rng;
    };

} // namespace BBEngine
