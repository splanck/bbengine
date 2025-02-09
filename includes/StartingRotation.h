#pragma once

#include <vector>
#include <stdexcept>  // if you want to throw exceptions
#include "Player.h"   // or forward-declare class Player; if you prefer

namespace BBEngine
{
    /**
     * The StartingRotation class manages a cyclical list of Player* pitchers
     * for a team’s rotation. Typically 5 pitchers, but can be 4, 6, or more.
     */
    class StartingRotation
    {
    public:
        // ----------------------------------------------------
        // Constructors
        // ----------------------------------------------------

        /**
         * Default constructor: empty rotation, nextStarterIndex=0
         */
        StartingRotation();

        /**
         * Constructor that initializes from a list of pitchers.
         * nextStarterIndex is set to 0 by default.
         */
        StartingRotation(const std::vector<Player*>& starterList);

        // ----------------------------------------------------
        // Accessing Next Starter
        // ----------------------------------------------------

        /**
         * Returns the pitcher currently scheduled to start
         * (i.e., pitchers[nextStarterIndex]) without incrementing.
         * If the rotation is empty, returns nullptr (or you could throw).
         */
        Player* getNextStarter() const;

        /**
         * After a game, call this to advance the rotation by 1 slot.
         * If nextStarterIndex equals pitchers.size() after increment,
         * wrap to 0.
         */
        void advanceRotation();

        // ----------------------------------------------------
        // Modifying the Rotation
        // ----------------------------------------------------

        /**
         * Add a new pitcher at the end of the rotation (or potentially
         * anywhere if you wanted a specific index).
         */
        void addPitcher(Player* newPitcher);

        /**
         * Remove a pitcher at a given index from the rotation.
         * If the index is before nextStarterIndex, adjust nextStarterIndex.
         */
        void removePitcher(int index);

        /**
         * Replace the pitcher at 'index' with 'newPitcher'.
         * If 'index' is out of range, throw or log an error.
         */
        void setPitcherAtIndex(int index, Player* newPitcher);

        // ----------------------------------------------------
        // (Optional) Additional Methods
        // ----------------------------------------------------

        /**
         * skipNextStarter() increments nextStarterIndex by 1 (wrap if needed)
         * without that pitcher actually starting a game. (Used if you skip #5 starter.)
         */
        void skipNextStarter();

        /**
         * If you want direct control over nextStarterIndex.
         */
        int  getNextStarterIndex() const;
        void setNextStarterIndex(int newIndex);

        /**
         * If you want to read or modify the entire vector externally,
         * e.g., for advanced manager logic.
         */
        const std::vector<Player*>& getPitchers() const;

    private:
        /**
         * Helper to ensure nextStarterIndex never goes out of bounds
         * if pitchers is not empty. This is called after increments.
         */
        void wrapIndex();

    private:
        std::vector<Player*> pitchers;  ///< The rotation list of pitchers
        int nextStarterIndex;           ///< Tracks which pitcher is next
    };

} // namespace BBEngine
