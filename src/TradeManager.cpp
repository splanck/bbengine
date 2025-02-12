#include "TradeManager.h"
#include <algorithm>  // for std::find
#include <iostream>   // for logs

namespace BBEngine
{
    TradeManager::TradeManager(std::vector<Team*>& leagueTeams,
        std::vector<Player*>& freeAgentsPool,
        bool deadlinePassed,
        int maxRosterSize)
        : teams(leagueTeams),
        freeAgents(freeAgentsPool),
        deadline(deadlinePassed),
        maxActiveRoster(maxRosterSize)
    {
    }

    void TradeManager::setDeadlinePassed(bool isPassed)
    {
        deadline = isPassed;
    }
    bool TradeManager::getDeadlinePassed() const
    {
        return deadline;
    }

    bool TradeManager::proposeTrade(Team* fromTeam, Team* toTeam,
        const std::vector<Player*>& playersToGive,
        const std::vector<Player*>& playersToReceive)
    {
        // 1. Check if trade deadline has passed
        if (deadline)
        {
            std::cerr << "[TradeManager] Trade deadline passed. Cannot execute.\n";
            return false;
        }
        // 2. Validate fromTeam actually has playersToGive,
        //    and toTeam has playersToReceive. Also check roster capacity after swap.

        // Check fromTeam
        for (auto* p : playersToGive)
        {
            // ensure p is in fromTeam->roster
            if (!fromTeam->hasPlayer(p))
            {
                std::cerr << "[TradeManager] " << fromTeam->getName()
                    << " does not have player " << p->getName() << "\n";
                return false;
            }
        }
        // Check toTeam
        for (auto* p : playersToReceive)
        {
            if (!toTeam->hasPlayer(p))
            {
                std::cerr << "[TradeManager] " << toTeam->getName()
                    << " does not have player " << p->getName() << "\n";
                return false;
            }
        }

        // Let's see how many new players each side would get
        // fromTeam will receive playersToReceive, toTeam receives playersToGive.
        if (!canTeamAcquirePlayers(fromTeam, playersToReceive))
        {
            std::cerr << "[TradeManager] " << fromTeam->getName()
                << " cannot acquire these players (roster size?).\n";
            return false;
        }
        if (!canTeamAcquirePlayers(toTeam, playersToGive))
        {
            std::cerr << "[TradeManager] " << toTeam->getName()
                << " cannot acquire these players (roster size?).\n";
            return false;
        }

        // If everything is valid, we do the trade
        executeTrade(fromTeam, toTeam, playersToGive, playersToReceive);
        return true;
    }

    bool TradeManager::signFreeAgent(Team* signingTeam, Player* freeAgent)
    {
        // confirm this player is in freeAgents
        auto it = std::find(freeAgents.begin(), freeAgents.end(), freeAgent);
        if (it == freeAgents.end())
        {
            std::cerr << "[TradeManager] signFreeAgent: " << freeAgent->getName()
                << " is not a free agent.\n";
            return false;
        }

        // check roster capacity
        std::vector<Player*> onePlayer{ freeAgent };
        if (!canTeamAcquirePlayers(signingTeam, onePlayer))
        {
            std::cerr << "[TradeManager] " << signingTeam->getName()
                << " cannot sign " << freeAgent->getName()
                << " (roster limit?).\n";
            return false;
        }

        // remove from free agent list
        freeAgents.erase(it);
        // add to team
        signingTeam->addPlayer(freeAgent);

        std::cout << "[TradeManager] " << signingTeam->getName()
            << " signed free agent " << freeAgent->getName() << "\n";
        return true;
    }

    bool TradeManager::releasePlayer(Team* fromTeam, Player* p)
    {
        // ensure p is in fromTeam
        if (!fromTeam->hasPlayer(p))
        {
            std::cerr << "[TradeManager] releasePlayer: "
                << fromTeam->getName() << " does not have "
                << p->getName() << "\n";
            return false;
        }
        // remove from roster
        fromTeam->removePlayer(p);
        // add to freeAgents
        freeAgents.push_back(p);
        std::cout << "[TradeManager] " << fromTeam->getName()
            << " released " << p->getName()
            << " to free agents.\n";
        return true;
    }

    // -----------------------------------------
    // Private
    // -----------------------------------------
    void TradeManager::executeTrade(Team* fromTeam, Team* toTeam,
        const std::vector<Player*>& fromTeamPlayers,
        const std::vector<Player*>& toTeamPlayers)
    {
        // 1. remove fromTeamPlayers from fromTeam
        //    add them to toTeam
        for (auto* p : fromTeamPlayers)
        {
            fromTeam->removePlayer(p);
            toTeam->addPlayer(p);
        }
        // 2. remove toTeamPlayers from toTeam
        //    add them to fromTeam
        for (auto* p : toTeamPlayers)
        {
            toTeam->removePlayer(p);
            fromTeam->addPlayer(p);
        }

        // Possibly update lineups or rotation if these players are in them
        // fromTeam->updateLineupsAfterTrade(...); etc. 
        // For demonstration, we skip that.

        std::cout << "[TradeManager] Trade executed between "
            << fromTeam->getName() << " and " << toTeam->getName() << "\n";
    }

    bool TradeManager::canTeamAcquirePlayers(Team* team, const std::vector<Player*>& newPlayers)
    {
        // check if team->roster.size() + newPlayers.size() <= maxActiveRoster
        // For demonstration, let's do that logic
        int currentSize = team->getRoster().size();
        int needed = static_cast<int>(newPlayers.size());
        if (currentSize + needed > maxActiveRoster)
        {
            return false;
        }
        // If you had finances, you'd check them here
        return true;
    }
}
