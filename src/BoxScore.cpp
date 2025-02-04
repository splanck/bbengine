#include "BoxScore.h"
#include <algorithm>
#include <stdexcept>

namespace BBEngine
{
    // -------------------------------------------------
    // Constructor & Basic Game Info
    // -------------------------------------------------
    BoxScore::BoxScore(const std::string& homeTeamName,
                       const std::string& awayTeamName,
                       const std::string& ballparkName,
                       int attendanceCount)
        : homeTeam(homeTeamName),
          awayTeam(awayTeamName),
          ballpark(ballparkName),
          attendance(attendanceCount)
    {
    }

    const std::string& BoxScore::getHomeTeamName() const { return homeTeam; }
    const std::string& BoxScore::getAwayTeamName() const { return awayTeam; }
    const std::string& BoxScore::getBallpark()    const { return ballpark; }
    int BoxScore::getAttendance() const { return attendance; }

    void BoxScore::setHomeTeamName(const std::string& name) { homeTeam = name; }
    void BoxScore::setAwayTeamName(const std::string& name) { awayTeam = name; }
    void BoxScore::setBallpark(const std::string& name)     { ballpark = name; }
    void BoxScore::setAttendance(int count)                 { attendance = count; }

    // -------------------------------------------------
    // Manage Batting Lines
    // -------------------------------------------------
    void BoxScore::addBatterToLineup(TeamSide side, Player* player, int battingOrder)
    {
        if (!player)
            throw std::invalid_argument("Player pointer is null.");

        auto& lineup = getBattingLines(side);

        // Check if there's an existing *active* line for this batting order
        auto it = std::find_if(lineup.begin(), lineup.end(),
            [battingOrder](const BoxScoreBattingLine& line) {
                return line.battingOrder == battingOrder && line.active;
            });

        if (it != lineup.end())
        {
            throw std::runtime_error("A batter is already active at this batting order.");
        }

        // Create a new line and add it
        BoxScoreBattingLine newLine;
        newLine.player = player;
        newLine.battingOrder = battingOrder;
        newLine.active = true;
        lineup.push_back(newLine);

        // Keep the batting lines sorted by battingOrder for convenience
        std::sort(lineup.begin(), lineup.end(), 
                  [](const BoxScoreBattingLine& a, const BoxScoreBattingLine& b){
                      return a.battingOrder < b.battingOrder;
                  });
    }

    void BoxScore::replaceBatter(TeamSide side, Player* newPlayer, int battingOrder)
    {
        if (!newPlayer)
            throw std::invalid_argument("New player pointer is null.");

        BoxScoreBattingLine& oldLine = findBattingLine(side, battingOrder);
        // Mark the old line inactive
        oldLine.active = false;

        // Add the new line at the same batting order
        addBatterToLineup(side, newPlayer, battingOrder);
    }

    Player* BoxScore::getNextBatter(TeamSide side)
    {
        auto& lineup = getBattingLines(side);
        if (lineup.empty())
        {
            throw std::runtime_error("No batters in the lineup.");
        }

        // Decide which index to advance
        int& nextIndex = (side == TeamSide::Home) ? homeNextBatterIndex : awayNextBatterIndex;

        // Make sure nextIndex isn't negative; if it is, start from 0
        if (nextIndex < 0)
            nextIndex = 0;

        // We'll attempt to find the next active batter within one full loop of the lineup size
        const int lineupSize = static_cast<int>(lineup.size());
        for (int attempt = 0; attempt < lineupSize; ++attempt)
        {
            // Wrap around if needed
            if (nextIndex >= lineupSize)
            {
                nextIndex = 0;
            }

            // Check if this batting slot is active
            auto& bLine = lineup[nextIndex];
            if (bLine.active)
            {
                // Found the next active batter
                Player* batter = bLine.player;
                // Move index forward for next call
                nextIndex = (nextIndex + 1) % lineupSize;
                return batter;
            }
            else
            {
                // Skip inactive slot and keep searching
                nextIndex = (nextIndex + 1) % lineupSize;
            }
        }

        // If we exit the loop, it means no active batters were found
        throw std::runtime_error("No active batters remain in the lineup!");
    }

    void BoxScore::recordAtBat(TeamSide side,
                               int battingOrder,
                               int atBats,
                               int hits,
                               int doubles,
                               int triples,
                               int homeRuns,
                               int runsBattedIn,
                               int walks,
                               int strikeouts,
                               int runsScored)
    {
        BoxScoreBattingLine& line = findBattingLine(side, battingOrder);

        line.atBats += atBats;
        line.hits += hits;
        line.doubles += doubles;
        line.triples += triples;
        line.homeRuns += homeRuns;
        line.runsBattedIn += runsBattedIn;
        line.walks += walks;
        line.strikeouts += strikeouts;
        line.runs += runsScored;
    }

    void BoxScore::recordWalk(TeamSide side, int battingOrder)
    {
        BoxScoreBattingLine& line = findBattingLine(side, battingOrder);
        line.walks += 1;
        // Typically no increment to atBats on a walk
    }

    void BoxScore::recordStrikeout(TeamSide side, int battingOrder)
    {
        BoxScoreBattingLine& line = findBattingLine(side, battingOrder);
        line.strikeouts += 1;
        line.atBats += 1; // Usually a K counts as an at-bat
    }

    void BoxScore::recordHit(TeamSide side, int battingOrder,
                             bool isDouble, bool isTriple, bool isHomerun)
    {
        BoxScoreBattingLine& line = findBattingLine(side, battingOrder);
        line.atBats += 1;
        line.hits += 1;

        if (isDouble)
            line.doubles += 1;
        else if (isTriple)
            line.triples += 1;
        else if (isHomerun)
            line.homeRuns += 1;
    }

    const std::vector<BoxScoreBattingLine>& BoxScore::getHomeBattingLines() const
    {
        return homeBattingLines;
    }

    const std::vector<BoxScoreBattingLine>& BoxScore::getAwayBattingLines() const
    {
        return awayBattingLines;
    }

    int BoxScore::getHomeTeamRuns() const
    {
        int totalRuns = 0;
        for (const auto& line : homeBattingLines)
        {
            totalRuns += line.runs;
        }
        return totalRuns;
    }

    int BoxScore::getAwayTeamRuns() const
    {
        int totalRuns = 0;
        for (const auto& line : awayBattingLines)
        {
            totalRuns += line.runs;
        }
        return totalRuns;
    }

    // -----------------------------------------
    // Internal Helpers for Batting Lines
    // -----------------------------------------
    BoxScoreBattingLine& BoxScore::findBattingLine(TeamSide side, int battingOrder)
    {
        auto& lineup = getBattingLines(side);

        auto it = std::find_if(lineup.begin(), lineup.end(),
            [battingOrder](BoxScoreBattingLine& line) {
                return line.battingOrder == battingOrder && line.active;
            });

        if (it == lineup.end())
        {
            throw std::runtime_error("No active batter found with that batting order.");
        }

        return *it;
    }

    std::vector<BoxScoreBattingLine>& BoxScore::getBattingLines(TeamSide side)
    {
        return (side == TeamSide::Home) ? homeBattingLines : awayBattingLines;
    }

    // -------------------------------------------------
    // Manage Pitching Lines
    // -------------------------------------------------
    void BoxScore::addPitcher(TeamSide side, Player* pitcher)
    {
        if (!pitcher)
            throw std::invalid_argument("Pitcher pointer is null.");

        if (side == TeamSide::Home)
        {
            BoxScorePitchingLine line;
            line.pitcher = pitcher;
            line.appearanceOrder = nextHomePitcherOrder++;
            homePitchingLines.push_back(line);
        }
        else
        {
            BoxScorePitchingLine line;
            line.pitcher = pitcher;
            line.appearanceOrder = nextAwayPitcherOrder++;
            awayPitchingLines.push_back(line);
        }
    }

    void BoxScore::recordPitching(TeamSide side,
                                  int appearanceOrder,
                                  double inningsPitchedDelta,
                                  int hitsAllowed,
                                  int runsAllowed,
                                  int earnedRuns,
                                  int walksAllowed,
                                  int strikeouts)
    {
        BoxScorePitchingLine& line = findPitchingLine(side, appearanceOrder);

        line.inningsPitched += inningsPitchedDelta;
        line.hitsAllowed += hitsAllowed;
        line.runsAllowed += runsAllowed;
        line.earnedRuns += earnedRuns;
        line.walksAllowed += walksAllowed;
        line.strikeouts += strikeouts;
    }

    void BoxScore::assignPitchingDecision(TeamSide side,
                                          int appearanceOrder,
                                          bool win,
                                          bool loss,
                                          bool save)
    {
        BoxScorePitchingLine& line = findPitchingLine(side, appearanceOrder);
        line.decisionWin = win;
        line.decisionLoss = loss;
        line.decisionSave = save;
    }

    const std::vector<BoxScorePitchingLine>& BoxScore::getHomePitchingLines() const
    {
        return homePitchingLines;
    }

    const std::vector<BoxScorePitchingLine>& BoxScore::getAwayPitchingLines() const
    {
        return awayPitchingLines;
    }

    BoxScorePitchingLine& BoxScore::findPitchingLine(TeamSide side, int appearanceOrder)
    {
        std::vector<BoxScorePitchingLine>* lines =
            (side == TeamSide::Home) ? &homePitchingLines : &awayPitchingLines;

        auto it = std::find_if(lines->begin(), lines->end(),
            [appearanceOrder](BoxScorePitchingLine& line) {
                return line.appearanceOrder == appearanceOrder;
            });

        if (it == lines->end())
        {
            throw std::runtime_error("No pitcher found with that appearance order.");
        }
        return *it;
    }

} // namespace BBEngine
