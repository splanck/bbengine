#include "StatsManager.h"
#include "PlayerStats.h"
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <cmath> // for possible rounding

namespace BBEngine
{
    StatsManager::StatsManager()
    {
    }

    StatsManager::StatsManager(const std::vector<Player*>& initialPlayers)
        : allPlayers(initialPlayers)
    {
    }

    void StatsManager::registerPlayer(Player* player)
    {
        if (!player) return;
        // Optionally check if already in vector
        allPlayers.push_back(player);
    }

    void StatsManager::unregisterPlayer(Player* player)
    {
        auto it = std::find(allPlayers.begin(), allPlayers.end(), player);
        if (it != allPlayers.end())
        {
            allPlayers.erase(it);
        }
    }

    std::vector<std::pair<Player*, double>>
        StatsManager::getLeaders(const std::string& stat, int topN, bool ascending)
    {
        std::vector<std::pair<Player*, double>> results;
        results.reserve(allPlayers.size());

        // Fill with (player, statValue)
        for (auto* p : allPlayers)
        {
            double val = getStatValue(p, stat);
            results.push_back({ p, val });
        }

        // Sort by the second field
        // If ascending, a < b => ascending, else descending
        if (ascending)
        {
            std::sort(results.begin(), results.end(),
                [](auto& a, auto& b) { return a.second < b.second; });
        }
        else
        {
            std::sort(results.begin(), results.end(),
                [](auto& a, auto& b) { return a.second > b.second; });
        }

        // clamp topN
        if (topN > static_cast<int>(results.size()))
            topN = static_cast<int>(results.size());

        results.resize(topN);
        return results;
    }

    double StatsManager::getTeamStat(Team* team,
        const std::string& stat,
        const std::string& aggregateMethod)
    {
        if (!team) return 0.0;
        double sum = 0.0;
        int count = 0;

        // for each player in team->getRoster()
        const auto& roster = team->getRoster();
        for (auto* p : roster)
        {
            double val = getStatValue(p, stat);
            sum += val;
            count++;
        }

        if (count == 0) return 0.0;

        if (aggregateMethod == "SUM")
        {
            return sum;
        }
        else if (aggregateMethod == "AVG")
        {
            return (sum / count);
        }
        // fallback 
        return sum;
    }

    void StatsManager::checkAndUpdateAllTimeRecord(Player* player, const std::string& stat)
    {
        double currentVal = getStatValue(player, stat);

        auto it = allTimeRecords.find(stat);
        if (it == allTimeRecords.end())
        {
            // This means we have no record for this stat yet, so set it
            AllTimeRecord rec;
            rec.recordValue = currentVal;
            rec.recordHolder = player;
            allTimeRecords[stat] = rec;
            std::cout << "[StatsManager] New all-time record for stat=" << stat
                << ": " << currentVal << " by " << player->getName() << "\n";
        }
        else
        {
            // Compare with existing record
            if (currentVal > it->second.recordValue)
            {
                it->second.recordValue = currentVal;
                it->second.recordHolder = player;

                std::cout << "[StatsManager] " << player->getName()
                    << " set a new record in " << stat
                    << " with " << currentVal << "\n";
            }
        }
    }

    StatsManager::AllTimeRecord StatsManager::getAllTimeRecord(const std::string& stat) const
    {
        auto it = allTimeRecords.find(stat);
        if (it == allTimeRecords.end())
        {
            // return default or throw
            throw std::runtime_error("No all-time record found for stat = " + stat);
        }
        return it->second;
    }

    // This is the "heart" of stat retrieval. 
    // We'll expand the if-else for more stats. 
    double StatsManager::getStatValue(Player* player, const std::string& stat) const
    {
        auto* ps = player->getStats();
        if (!ps) return 0.0;

        // Example usage: we assume your PlayerStats has methods like 
        // getAtBats(), getHits(), getWalks(), getHomeRuns(), getRBI(), getERA()...
        // We'll define them in if-else:

        if (stat == "AVG")
        {
            int ab = ps->getAtBats();
            int h = ps->getHits();
            if (ab == 0) return 0.0;
            return static_cast<double>(h) / ab;
        }
        else if (stat == "OBP")
        {
            // OBP = (H + BB) / (AB + BB + SF...) we keep it simple
            int ab = ps->getAtBats();
            int h = ps->getHits();
            int bb = ps->getWalks();
            // ignoring HBP, sac flies, etc. for brevity
            int denom = ab + bb;
            if (denom == 0) return 0.0;
            return static_cast<double>(h + bb) / denom;
        }
        else if (stat == "SLG")
        {
            // Suppose we have getSingles, getDoubles, getTriples, getHomeRuns?
            // We'll do a rough approach: totalBases / AB
            int ab = ps->getAtBats();
            if (ab == 0) return 0.0;
            int singles = ps->getHits() - (ps->getDoubles() + ps->getTriples() + ps->getHomeRuns());
            int totalBases = singles + 2 * ps->getDoubles() + 3 * ps->getTriples() + 4 * ps->getHomeRuns();
            return static_cast<double>(totalBases) / ab;
        }
        else if (stat == "OPS")
        {
            // OPS = OBP + SLG
            double obp = getStatValue(player, "OBP");
            double slg = getStatValue(player, "SLG");
            return obp + slg;
        }
        else if (stat == "HR")
        {
            return static_cast<double>(ps->getHomeRuns());
        }
        else if (stat == "RBI")
        {
            return static_cast<double>(ps->getRBIs());
        }
        else if (stat == "ERA")
        {
            // (ER * 9) / IP
            double ip = ps->getInningsPitched();
            if (ip <= 0.0) return 99.99;
            int er = ps->getEarnedRuns();
            return (er * 9.0) / ip;
        }
        else if (stat == "WHIP")
        {
            // (walks + hits) / IP
            double ip = ps->getInningsPitched();
            if (ip <= 0.0) return 99.99;
            double wh = ps->getWalksAllowed() + ps->getHitsAllowed();
            return (wh / ip);
        }
        else
        {
            // not recognized
            throw std::runtime_error("[StatsManager::getStatValue] Unknown stat: " + stat);
        }
    }
}
