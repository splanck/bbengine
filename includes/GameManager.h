#pragma once

#include <string>
#include "Team.h"
#include "BoxScore.h"
#include "Simulator.h"

namespace BBEngine
{
    /**
     * The GameManager class orchestrates a single baseball game.
     * It handles:
     *  - The current inning, top/bottom
     *  - The number of outs
     *  - Base runners
     *  - Calls the Simulator for each pitch
     *  - Updates the BoxScore accordingly
     *  - Ends the game when conditions are met
     */
    class GameManager
    {
    public:
        /**
         * Constructor: set references to home/away teams, boxscore, simulator
         * and initialize game state.
         */
        GameManager(Team* home, Team* away, BoxScore* box, Simulator* sim);

        /**
         * Main method to run the entire game until a winner is decided (or 9+ innings).
         */
        void runGame();

        // Accessors for final results or status
        bool isGameOver() const;

        int  getCurrentInning() const;
        bool isTopOfInning() const;

    private:
        /**
         * Runs a half-inning of baseball: from 0 outs until 3 outs or other end condition.
         */
        void runHalfInning();

        /**
         * Perform a single pitch, calling the simulator.
         * If the pitch ends the at-bat (hit, walk, out), break from that at-bat loop.
         */
        void proceedPitch();

        /**
         * Check if the game should end now (walk-off, or after 9 innings).
         * If ended, set gameOver = true.
         */
        void checkGameOver();

        /**
         * Resets outs to 0, possibly resets base runners if needed (between half-innings).
         */
        void startHalfInning();

        /**
         * Called after each half-inning: flips top/bottom, increments inning if we finished the bottom.
         */
        void endHalfInning();

        // Helper for basic base running. Extremely naive: single always moves each runner up one base, etc.
        void handleBattedBall(BattedBallOutcome outcome, bool isHome, Player* batter, Player* pitcher);

        // Data members
        Team* homeTeam;
        Team* awayTeam;
        BoxScore* boxScore;
        Simulator* simulator;

        // Game state
        int  inning;        // starts at 1
        bool topOfInning;   // true => away batting, false => home batting
        int  outs;          // 0..3
        bool gameOver;

        // Simple base runner flags for first, second, third
        bool runnerOnFirst;
        bool runnerOnSecond;
        bool runnerOnThird;

        // Track the next batter index for each team
        int homeBatterIndex;
        int awayBatterIndex;
    };

} // namespace BBEngine
