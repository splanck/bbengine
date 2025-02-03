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

        std::vector<BoxScoreBattingLine>* lineup = nullptr;
        if (side == TeamSide::Home)
            lineup = &homeBattingLines;
        else
            lineup = &awayBattingLines;

        // Check if there's an existing line for this batting order
        auto it = std::find_if(lineup->begin(), lineup->end(),
            [battingOrder](const BoxScoreBattingLine& line) {
                return line.battingOrder == battingOrder;
            });

        if (it != lineup->end())
        {
            throw std::runtime_error("A batter is already assigned to this batting order.");
        }

        // Create a new line and add it
        BoxScoreBattingLine newLine;
        newLine.player = player;
        newLine.battingOrder = battingOrder;
        lineup->push_back(newLine);
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

    BoxScoreBattingLine& BoxScore::findBattingLine(TeamSide side, int battingOrder)
    {
        std::vector<BoxScoreBattingLine>* lineup =
            (side == TeamSide::Home) ? &homeBattingLines : &awayBattingLines;

        auto it = std::find_if(lineup->begin(), lineup->end(),
            [battingOrder](BoxScoreBattingLine& line) {
                return line.battingOrder == battingOrder;
            });

        if (it == lineup->end())
        {
            throw std::runtime_error("No batter found with that batting order.");
        }

        return *it;
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
}
