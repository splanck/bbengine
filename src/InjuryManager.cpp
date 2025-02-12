#include "InjuryManager.h"
#include <iostream> // for debug prints
#include <stdexcept> // for exceptions

namespace BBEngine
{
    InjuryManager::InjuryManager(std::vector<Team*>& allTeams)
        : teams(allTeams)
    {
    }

    void InjuryManager::injurePlayer(Player* player,
        const std::string& injuryType,
        int daysOut,
        int severityLevel)
    {
        if (!player)
        {
            std::cerr << "[InjuryManager] injurePlayer called with nullptr player.\n";
            return;
        }
        if (daysOut < 1) daysOut = 1; // minimal 1 day out, or your own logic

        // If already injured, decide if we override or just skip
        auto it = injuries.find(player);
        if (it != injuries.end())
        {
            std::cerr << "[InjuryManager] Player " << player->getName()
                << " is already injured. Updating info.\n";
            it->second.injuryType = injuryType;
            it->second.daysRemaining = daysOut;
            it->second.severityLevel = severityLevel;
            return;
        }

        // Otherwise, create a new record
        InjuryInfo info;
        info.injuryType = injuryType;
        info.daysRemaining = daysOut;
        info.severityLevel = severityLevel;
        injuries[player] = info;

        // Also remove from active roster so the team can't use them
        removePlayerFromRoster(player);

        std::cout << "[InjuryManager] Player " << player->getName()
            << " is now injured ("
            << injuryType << "), out for " << daysOut << " days.\n";
    }

    void InjuryManager::activatePlayer(Player* player)
    {
        auto it = injuries.find(player);
        if (it == injuries.end())
        {
            std::cerr << "[InjuryManager] activatePlayer: " << player->getName()
                << " is not in the injured list.\n";
            return; // or ignore or throw
        }
        // remove from map
        injuries.erase(it);
        std::cout << "[InjuryManager] Player " << player->getName()
            << " is recovered and can be re-activated.\n";
    }

    void InjuryManager::decrementInjuryTimers(int daysPassed)
    {
        if (daysPassed < 1) daysPassed = 1; // or allow any?

        // We have to iterate over injuries and reduce each daysRemaining
        // But we might also remove from the map if daysRemaining <= 0
        // so it's safer to gather the ones that are done first
        std::vector<Player*> recoveredList;

        for (auto& kv : injuries)
        {
            auto& info = kv.second;
            info.daysRemaining -= daysPassed;
            if (info.daysRemaining <= 0)
            {
                recoveredList.push_back(kv.first); // they are ready to re-activate
            }
        }

        // Now remove them from injuries
        for (auto* p : recoveredList)
        {
            injuries.erase(p);
            std::cout << "[InjuryManager] Player " << p->getName()
                << " has fully recovered.\n";
            // The team or manager can reinsert them in lineups if needed.
        }
    }

    bool InjuryManager::isPlayerInjured(Player* player) const
    {
        return (injuries.find(player) != injuries.end());
    }

    InjuryManager::InjuryInfo InjuryManager::getInjuryInfo(Player* player) const
    {
        auto it = injuries.find(player);
        if (it == injuries.end())
        {
            throw std::runtime_error("[InjuryManager] getInjuryInfo: player not found in injuries!");
        }
        return it->second;
    }

    // -------------------------------------------------------
    // Private helpers
    // -------------------------------------------------------
    Team* InjuryManager::findPlayerTeam(Player* p)
    {
        if (!p) return nullptr;

        // loop over teams
        for (auto* t : teams)
        {
            if (t && t->hasPlayer(p))
            {
                return t;
            }
        }
        return nullptr;
    }

    void InjuryManager::removePlayerFromRoster(Player* p)
    {
        Team* theTeam = findPlayerTeam(p);
        if (theTeam)
        {
            // remove from the roster so they're not selectable
            theTeam->removePlayer(p);
            std::cout << "[InjuryManager] Removed injured player "
                << p->getName() << " from team " << theTeam->getName()
                << "'s roster.\n";
        }
    }
}
