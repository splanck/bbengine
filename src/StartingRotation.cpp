#include "StartingRotation.h"
#include <iostream>  // if you want debug prints, else omit

namespace BBEngine
{
    // ----------------------------------------------------
    // Constructors
    // ----------------------------------------------------
    StartingRotation::StartingRotation()
        : nextStarterIndex(0)
    {
        // empty rotation, index=0
    }

    StartingRotation::StartingRotation(const std::vector<Player*>& starterList)
        : pitchers(starterList),
        nextStarterIndex(0)
    {
    }

    // ----------------------------------------------------
    // Accessing Next Starter
    // ----------------------------------------------------
    Player* StartingRotation::getNextStarter() const
    {
        if (pitchers.empty())
        {
            // Could throw or log an error
            // throw std::runtime_error("Rotation is empty, no next starter.");
            return nullptr;
        }
        return pitchers[nextStarterIndex];
    }

    void StartingRotation::advanceRotation()
    {
        if (!pitchers.empty())
        {
            nextStarterIndex++;
            wrapIndex();
        }
        // if empty, do nothing or throw
    }

    // ----------------------------------------------------
    // Modifying the Rotation
    // ----------------------------------------------------
    void StartingRotation::addPitcher(Player* newPitcher)
    {
        if (!newPitcher)
        {
            // Possibly throw or just ignore
            // throw std::invalid_argument("newPitcher is null");
            return;
        }
        pitchers.push_back(newPitcher);
        // no changes to nextStarterIndex needed
    }

    void StartingRotation::removePitcher(int index)
    {
        if (index < 0 || index >= static_cast<int>(pitchers.size()))
        {
            throw std::out_of_range("Invalid pitcher index for removePitcher");
        }

        // If index is before nextStarterIndex, decrement nextStarterIndex
        // so that the rotation order remains consistent after removal
        if (index < nextStarterIndex)
        {
            nextStarterIndex--;
        }

        pitchers.erase(pitchers.begin() + index);
        // Then clamp nextStarterIndex if needed
        wrapIndex();
    }

    void StartingRotation::setPitcherAtIndex(int index, Player* newPitcher)
    {
        if (index < 0 || index >= static_cast<int>(pitchers.size()))
        {
            throw std::out_of_range("Invalid pitcher index for setPitcherAtIndex");
        }
        if (!newPitcher)
        {
            // Possibly throw or ignore
            // throw std::invalid_argument("newPitcher is null");
            return;
        }
        pitchers[index] = newPitcher;
    }

    // ----------------------------------------------------
    // Additional Methods
    // ----------------------------------------------------
    void StartingRotation::skipNextStarter()
    {
        if (!pitchers.empty())
        {
            nextStarterIndex++;
            wrapIndex();
        }
    }

    int StartingRotation::getNextStarterIndex() const
    {
        return nextStarterIndex;
    }

    void StartingRotation::setNextStarterIndex(int newIndex)
    {
        if (newIndex < 0 || newIndex >= static_cast<int>(pitchers.size()))
        {
            // Could clamp or throw
            // throw std::out_of_range("Invalid newIndex");
            if (!pitchers.empty())
                nextStarterIndex = newIndex % pitchers.size();
            else
                nextStarterIndex = 0;
        }
        else
        {
            nextStarterIndex = newIndex;
        }
    }

    const std::vector<Player*>& StartingRotation::getPitchers() const
    {
        return pitchers;
    }

    // ----------------------------------------------------
    // Private Helper
    // ----------------------------------------------------
    void StartingRotation::wrapIndex()
    {
        if (!pitchers.empty())
        {
            nextStarterIndex %= pitchers.size();
        }
        else
        {
            nextStarterIndex = 0;
        }
    }

} // namespace BBEngine
