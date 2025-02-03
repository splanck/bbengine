#pragma once

#include <optional>
#include <random> // For demonstration of random events
#include "Player.h"

namespace BBEngine
{
    // --------------------------------------------------
    // 1) At-Bat Simulation
    // --------------------------------------------------

    // Inputs needed to simulate a single at-bat.
    struct AtBatInputs
    {
        // Required
        const Player* batter = nullptr;
        const Player* pitcher = nullptr;

        // Optional context for more realism
        // e.g., current inning, count of balls/strikes, weather, ballpark factors, etc.
        int currentInning = 1;
        int balls = 0;   // For partial at-bat states
        int strikes = 0; // etc.

        // If you have a dedicated random engine, you could also store it here
        // or pass it in as a separate parameter to the simulation function.
    };

    // Common outcomes from an at-bat
    enum class AtBatOutcome
    {
        Unknown,
        Strikeout,
        Walk,
        Single,
        Double,
        Triple,
        HomeRun,
        FlyOut,
        GroundOut,
        // Add more if needed (e.g., HBP, interference, etc.)
    };

    // Results of the at-bat simulation.
    struct AtBatResults
    {
        AtBatOutcome outcome = AtBatOutcome::Unknown;
        int basesGained = 0;   // 0 for an out, 1 single, 2 double, etc.
        bool endedAtBat = true; // If the at-bat concluded or not (some advanced systems track partial at-bats)
        bool wasOut = false;    // True if the batter was out
        int runsScored = 0;     // How many runs scored because of this play (batter + any base runners)
    };

    // --------------------------------------------------
    // 2) Baserunning Simulation
    // --------------------------------------------------

    // Inputs for a baserunning scenario, e.g., single with a runner on first,
    // or attempting to stretch a single into a double.
    struct BaseRunningInputs
    {
        const Player* runner = nullptr;
        const Player* fielder = nullptr; // The primary fielder trying to make the play
        int basesToAdvance = 1;         // How many bases the runner attempts to advance
        bool isHitToOutfield = true;    // Simplistic factor (maybe more if you handle direction, distance, etc.)

        // Additional context like runner speed, fielder arm strength, etc. could come from the Player objects,
        // or you can store them here if you want snapshot values.
    };

    // Results from the baserunning simulation.
    struct BaseRunningResults
    {
        bool runnerSafe = false;  // Did the runner safely advance?
        int basesAdvanced = 0;    // How many bases the runner actually advanced
        bool runnerOut = false;   // True if the runner was thrown out
        bool runScored = false;   // If crossing home, did a run score?
    };

    // --------------------------------------------------
    // 3) Fielding Simulation
    // --------------------------------------------------

    // Inputs to simulate a fielding play. This might overlap with baserunning or at-bat logic,
    // but it can also stand alone for something like a fly ball or grounder.
    struct FieldingInputs
    {
        const Player* fielder = nullptr;
        bool isFlyBall = false;
        bool isHardHit = false;
        // Additional context: distance, weather, spin, etc.
    };

    // Results of a fielding simulation.
    struct FieldingResults
    {
        bool caught = false;  // e.g., for a fly ball
        bool error = false;   // Did the fielder commit an error?
        bool successfulThrow = false; // If needed for advanced logic
        // Add more details as needed (like if the throw beats the runner, etc.)
    };

    // --------------------------------------------------
    // Simulator Class (Static Methods)
    // --------------------------------------------------

    class Simulator
    {
    public:
        // 1) Simulate a single at-bat matchup
        static AtBatResults simulateAtBat(const AtBatInputs& inputs);

        // 2) Simulate a baserunning event (e.g., runner on first trying to score on a double)
        static BaseRunningResults simulateBaseRunning(const BaseRunningInputs& inputs);

        // 3) Simulate a fielding play (e.g., fielder attempting to catch a fly ball)
        static FieldingResults simulateFielding(const FieldingInputs& inputs);

    private:
        // Helper: get a random floating value in [0,1)
        // If your project uses a dedicated RNG or seed, adapt this approach.
        static double randomDouble();

        // Example logic might reference players' attributes to determine probabilities:
        static double getBatterPowerFactor(const Player* batter);
        static double getBatterContactFactor(const Player* batter);
        static double getPitcherEffectiveness(const Player* pitcher);
        static double getRunnerSpeed(const Player* runner);
        static double getFielderSkill(const Player* fielder);
    };

} // namespace BBEngine
