#include "Standings.h"
#include <algorithm>  // for std::sort
#include <iostream>   // for debug prints if needed
#include <cassert>

namespace BBEngine
{
    Standings::Standings(const std::vector<Team*>& allTeams, const std::string& defaultDivision)
    {
        for (auto* t : allTeams)
        {
            // each record starts at 0-0
            TeamRecord rec(t, defaultDivision);
            records.push_back(rec);
        }
        // optionally call updateStandings() if you want initial sorting
        updateStandings();
    }

    void Standings::recordWin(Team* team)
    {
        TeamRecord* rec = findRecord(team);
        if (!rec)
        {
            // could throw or log. We'll just ignore for now
            return;
        }
        rec->wins++;
    }

    void Standings::recordLoss(Team* team)
    {
        TeamRecord* rec = findRecord(team);
        if (!rec) return;
        rec->losses++;
    }

    void Standings::recordGameResult(Team* winner, Team* loser)
    {
        recordWin(winner);
        recordLoss(loser);
    }

    void Standings::updateStandings()
    {
        // If you want to separate by division, you could group them, sort each, then merge or store them separately.
        // We'll do a single list sort for demonstration.

        // Step 1: sort the records by best W-L
        std::sort(records.begin(), records.end(), teamRecordCompare);

        // Step 2: compute games behind the top team
        computeGamesBehind();
    }

    std::vector<TeamRecord> Standings::getOverallStandings() const
    {
        return records; // note: they're not guaranteed sorted if you haven't called updateStandings() recently
    }

    std::vector<TeamRecord> Standings::getStandingsForDivision(const std::string& division) const
    {
        // filter by division
        std::vector<TeamRecord> result;
        for (const auto& rec : records)
        {
            if (rec.division == division)
            {
                result.push_back(rec);
            }
        }
        // The caller might want to do a final sort, or we do it here. We'll do a local sort:
        std::sort(result.begin(), result.end(), teamRecordCompare);

        return result;
    }

    int Standings::getWins(Team* team) const
    {
        for (const auto& rec : records)
        {
            if (rec.team == team)
                return rec.wins;
        }
        return -1; // or throw
    }

    int Standings::getLosses(Team* team) const
    {
        for (const auto& rec : records)
        {
            if (rec.team == team)
                return rec.losses;
        }
        return -1; // or throw
    }

    TeamRecord* Standings::findRecord(Team* team)
    {
        for (auto& rec : records)
        {
            if (rec.team == team)
                return &rec;
        }
        return nullptr;
    }

    void Standings::computeGamesBehind()
    {
        if (records.empty()) return;

        // We assume records is sorted by best record first
        // The top team is records[0]. We'll compare each other team to them.
        int leaderWins = records[0].wins;
        int leaderLosses = records[0].losses;

        for (size_t i = 0; i < records.size(); ++i)
        {
            int w = records[i].wins;
            int l = records[i].losses;
            // formula = ((leaderWins - w) + (l - leaderLosses)) / 2
            double gb = static_cast<double>((leaderWins - w) + (l - leaderLosses)) / 2.0;
            if (gb < 0.0) gb = 0.0; // if they are leading or tied
            records[i].gamesBehind = gb;
        }
    }

    bool Standings::teamRecordCompare(const TeamRecord& a, const TeamRecord& b)
    {
        // We sort by best "win%" or wins-loss difference
        // e.g. if a has better W-L than b => a is earlier
        // Let's do difference: (wins - losses)
        int diffA = a.wins - a.losses;
        int diffB = b.wins - b.losses;
        if (diffA != diffB)
            return diffA > diffB; // bigger diff => better
        else
        {
            // tiebreak: if you want. We'll do lesser losses or by name
            // for simplicity
            return a.team->getName() < b.team->getName();
        }
    }

} // namespace BBEngine
