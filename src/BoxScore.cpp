#include "BoxScore.h"
#include <algorithm>
#include <stdexcept>

namespace BBEngine
{
    // -------------------------------------------------
    // Constructor
    // -------------------------------------------------
    BoxScore::BoxScore(const std::string& homeTeamName,
        const std::string& awayTeamName)
        : homeTeam(homeTeamName),
        awayTeam(awayTeamName)
    {
    }

    const std::string& BoxScore::getHomeTeamName() const { return homeTeam; }
    const std::string& BoxScore::getAwayTeamName() const { return awayTeam; }

    void BoxScore::setHomeTeamName(const std::string& name) { homeTeam = name; }
    void BoxScore::setAwayTeamName(const std::string& name) { awayTeam = name; }

    // -------------------------------------------------
    // Manage Batting Lines
    // -------------------------------------------------
    void BoxScore::addBatterToLineup(bool isHome, Player* player)
    {
        if (!player)
            throw std::invalid_argument("Player pointer is null.");

        // We'll store in the appropriate vector:
        if (isHome)
        {
            // Check if there's an existing line for this player
            auto it = std::find_if(homeBattingLines.begin(), homeBattingLines.end(),
                [player](const BoxScoreBattingLine& line) {
                    return line.player == player;
                });

            if (it != homeBattingLines.end())
            {
                // Already in the lineup, do nothing or throw
                return;
            }

            BoxScoreBattingLine newLine;
            newLine.player = player;
            homeBattingLines.push_back(newLine);
        }
        else
        {
            auto it = std::find_if(awayBattingLines.begin(), awayBattingLines.end(),
                [player](const BoxScoreBattingLine& line) {
                    return line.player == player;
                });
            if (it != awayBattingLines.end())
            {
                return;
            }
            BoxScoreBattingLine newLine;
            newLine.player = player;
            awayBattingLines.push_back(newLine);
        }
    }

    void BoxScore::recordAtBat(bool isHome,
        Player* batter,
        int atBats,
        int hits,
        int doubles_,
        int triples,
        int homeRuns,
        int runsBattedIn,
        int walks,
        int strikeouts,
        int runsScored)
    {
        if (!batter)
            throw std::invalid_argument("Batter pointer is null.");

        // get or create the correct BoxScoreBattingLine
        BoxScoreBattingLine& line = findBattingLine(isHome, batter);

        line.atBats += atBats;
        line.hits += hits;
        line.doubles_ += doubles_;
        line.triples += triples;
        line.homeRuns += homeRuns;
        line.runsBattedIn += runsBattedIn;
        line.walks += walks;
        line.strikeouts += strikeouts;
        line.runsScored += runsScored;
    }

    void BoxScore::recordWalk(bool isHome, Player* batter)
    {
        recordAtBat(isHome, batter,
            0,  // no increment to atBats
            0, 0, 0, 0, // no hits
            0, // no rbis
            1, // 1 walk
            0, // no strikeout
            0  // no run scored directly
        );
    }

    void BoxScore::recordStrikeout(bool isHome, Player* batter)
    {
        // Typically counts as 1 at-bat, 0 hits, 1 strikeout
        recordAtBat(isHome, batter,
            1, // atBats
            0, // hits
            0, 0, 0, // doubles, triples, HR
            0, // rbis
            0, // walks
            1, // strikeout
            0  // run scored
        );
    }

    void BoxScore::recordHit(bool isHome, Player* batter,
        bool isDouble,
        bool isTriple,
        bool isHomerun)
    {
        int doubles_ = (isDouble ? 1 : 0);
        int triples = (isTriple ? 1 : 0);
        int hr = (isHomerun ? 1 : 0);
        int hits = 1; // A successful hit

        // Typically a hit is also an official at-bat
        recordAtBat(isHome, batter,
            1, // atBats
            hits,
            doubles_,
            triples,
            hr,
            0,   // no immediate rbis set here, you might do that logic externally if runs scored
            0,   // walks
            0,   // strikeout
            0    // run scored
        );
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
        int total = 0;
        for (const auto& line : homeBattingLines)
        {
            total += line.runsScored;
        }
        return total;
    }

    int BoxScore::getAwayTeamRuns() const
    {
        int total = 0;
        for (const auto& line : awayBattingLines)
        {
            total += line.runsScored;
        }
        return total;
    }

    // -------------------------------------------------
    // Manage Pitching Lines
    // -------------------------------------------------
    void BoxScore::addPitcher(bool isHome, Player* pitcher)
    {
        if (!pitcher)
            throw std::invalid_argument("Pitcher pointer is null.");

        if (isHome)
        {
            // Check if pitcher is already in the lines
            auto it = std::find_if(homePitchingLines.begin(), homePitchingLines.end(),
                [pitcher](const BoxScorePitchingLine& line) {
                    return line.pitcher == pitcher;
                });
            if (it != homePitchingLines.end())
            {
                return; // pitcher already present
            }

            BoxScorePitchingLine line;
            line.pitcher = pitcher;
            homePitchingLines.push_back(line);
        }
        else
        {
            auto it = std::find_if(awayPitchingLines.begin(), awayPitchingLines.end(),
                [pitcher](const BoxScorePitchingLine& line) {
                    return line.pitcher == pitcher;
                });
            if (it != awayPitchingLines.end())
            {
                return;
            }

            BoxScorePitchingLine line;
            line.pitcher = pitcher;
            awayPitchingLines.push_back(line);
        }
    }

    void BoxScore::recordPitching(bool isHome,
        Player* pitcher,
        double inningsPitchedDelta,
        int hitsAllowed,
        int runsAllowed,
        int earnedRuns,
        int walksAllowed,
        int strikeouts)
    {
        if (!pitcher)
            throw std::invalid_argument("Pitcher pointer is null.");

        BoxScorePitchingLine& line = findPitchingLine(isHome, pitcher);

        line.inningsPitched += inningsPitchedDelta;
        line.hitsAllowed += hitsAllowed;
        line.runsAllowed += runsAllowed;
        line.earnedRuns += earnedRuns;
        line.walksAllowed += walksAllowed;
        line.strikeouts += strikeouts;
    }

    void BoxScore::assignPitchingDecision(bool isHome,
        Player* pitcher,
        bool win,
        bool loss,
        bool save)
    {
        BoxScorePitchingLine& line = findPitchingLine(isHome, pitcher);
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

    // -------------------------------------------------
    // Private Helpers
    // -------------------------------------------------
    BoxScoreBattingLine& BoxScore::findBattingLine(bool isHome, Player* player)
    {
        std::vector<BoxScoreBattingLine>* lines = (isHome ? &homeBattingLines : &awayBattingLines);

        // Try to find an existing line for this player
        auto it = std::find_if(lines->begin(), lines->end(),
            [player](BoxScoreBattingLine& line) {
                return line.player == player;
            });

        if (it == lines->end())
        {
            // If not found, create a new one
            BoxScoreBattingLine newLine;
            newLine.player = player;
            lines->push_back(newLine);
            return lines->back();
        }
        else
        {
            return *it;
        }
    }

    BoxScorePitchingLine& BoxScore::findPitchingLine(bool isHome, Player* pitcher)
    {
        std::vector<BoxScorePitchingLine>* lines = (isHome ? &homePitchingLines : &awayPitchingLines);

        // find existing
        auto it = std::find_if(lines->begin(), lines->end(),
            [pitcher](BoxScorePitchingLine& line) {
                return line.pitcher == pitcher;
            });

        if (it == lines->end())
        {
            // If not found, create a new one
            BoxScorePitchingLine newLine;
            newLine.pitcher = pitcher;
            lines->push_back(newLine);
            return lines->back();
        }
        else
        {
            return *it;
        }
    }

} // namespace BBEngine
