#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <ctime>       // for seeding RNG
#include <algorithm>   // for std::find if needed

// Include BBEngine classes
#include "Team.h"
#include "Player.h"
#include "PlayerStats.h"
#include "PlayerAttributes.h"
#include "StartingRotation.h"
#include "BoxScore.h"
#include "Simulator.h"

// If you have your own enumerations for pitch outcomes, etc.:
using namespace BBEngine;

// ----------------------------------------------------
// Utility: Create a 25-player roster for a Team
// ----------------------------------------------------
std::vector<Player*> createRoster(const std::string& teamName, int numPlayers = 25)
{
    std::vector<Player*> roster;
    roster.reserve(numPlayers);

    for (int i = 1; i <= numPlayers; i++)
    {
        std::string name = teamName + " Player" + std::to_string(i);
        Handedness handedness = (i % 2 == 0) ? Handedness::Right : Handedness::Left;

        // Create minimal attributes and stats
        PlayerAttributes* attr = new PlayerAttributes();
        PlayerStats* stat = new PlayerStats();

        // Construct player
        Player* p = new Player(name, 20 + (i % 10), handedness, attr, stat);
        roster.push_back(p);
    }
    return roster;
}

// ----------------------------------------------------
// Utility: Create a Team with a roster, rotation, lineups
// ----------------------------------------------------
Team* createTeam(const std::string& name)
{
    Team* team = new Team(name, "MLB");

    // Build a 25-player roster
    auto roster = createRoster(name, 25);

    // Add them to the Team
    for (auto* p : roster)
    {
        team->addPlayer(p);
    }

    // Rotation: pick the first 5 as "pitchers"
    for (int i = 0; i < 5; i++)
    {
        team->getRotation()->addPitcher(roster[i]);
    }

    // Batting lineup: pick 9 from the roster
    // (We assume we skip the first 5 if they are pitchers, 
    // but let's just do something naive: first 9 total or last 9. Up to you.)
    // We'll do last 9 for demonstration
    std::vector<Player*> lineup(roster.begin() + 5, roster.begin() + 14); // positions 6..14
    team->setLineupVsRHP(lineup);
    team->setLineupVsLHP(lineup);

    return team;
}

// ----------------------------------------------------
// We define a small struct to track base state for each half-inning
// (simple approach: no advanced base running).
// ----------------------------------------------------
struct BaseRunners
{
    bool onFirst = false;
    bool onSecond = false;
    bool onThird = false;

    void clear()
    {
        onFirst = onSecond = onThird = false;
    }
};

// ----------------------------------------------------
// A simple function to interpret a pitch outcome from the Simulator
// and update outs, boxscore, baseRunners, runs, etc.
// This is extremely simplified: 
//   - BALL => we walk the batter
//   - STRIKE => out 
//   - BATTED_BALL_IN_PLAY => random single/double/triple/HR/out
//   - FOUL => treat as strike if <2 strikes? 
//   (We skip real ball/strike counting for brevity.)
// ----------------------------------------------------
void simulateAtBat(Team* battingTeam,
    Player* batter,
    Player* pitcher,
    BoxScore* box,
    bool isHome, // or TeamSide side
    int& outs,
    BaseRunners& bases,
    Simulator* sim)
{
    // We'll do a loop calling simulator->simulatePitch() until an outcome ends the at-bat
    // but to keep it short, we'll do a single call that yields a final outcome
    PitchOutcome outcome = sim->simulatePitch(*(pitcher->getAttributes()),
        *(batter->getAttributes()),
        StadiumContext(), // if you do ballpark
        PitchContext());   // ignoring ball/strike count

    // For demonstration, let's interpret the outcome in a naive way
    // If we want more detail, we can do multiple pitches, but let's do it in one shot.
    using PO = PitchOutcome;
    if (outcome == PO::BALL)
    {
        // walk => no AB, box->recordWalk
        box->recordWalk(isHome, batter);
        // Move base runners (we skip advanced logic: if first is occupied => force)
        if (!bases.onFirst) bases.onFirst = true;
        else if (!bases.onSecond) bases.onSecond = true;
        else if (!bases.onThird) bases.onThird = true;
        std::cout << "  " << batter->getName() << " walks.\n";
    }
    else if (outcome == PO::STRIKE_SWINGING || outcome == PO::STRIKE_LOOKING || outcome == PO::FOUL)
    {
        // We'll just call it a strikeout => 1 out
        outs++;
        box->recordStrikeout(isHome, batter);
        std::cout << "  " << batter->getName() << " strikes out. (Out #" << outs << ")\n";
    }
    else if (outcome == PO::BATTED_BALL_IN_PLAY)
    {
        // Let's do a small random for single/double/triple/HR/out
        static std::mt19937 rng(std::random_device{}());
        int r = (rng() % 100); // 0..99
        if (r < 60)
        {
            // 60% chance it's a single
            // record single => recordAtBat(...) with hits=1, doubles=0...
            box->recordHit(isHome, batter, false, false, false);
            // Move base runners (naive approach)
            if (bases.onThird) { box->recordAtBat(isHome, batter, 0, 0, 0, 0, 0, 1, 0, 0, 1); bases.onThird = false; } // 1 run
            if (bases.onSecond) { bases.onThird = true; bases.onSecond = false; }
            if (bases.onFirst) { bases.onSecond = true; bases.onFirst = false; }
            bases.onFirst = true;
            std::cout << "  " << batter->getName() << " singles.\n";
        }
        else if (r < 75)
        {
            // 15% chance double
            box->recordHit(isHome, batter, true, false, false);
            // Move base runners
            int runsScored = 0;
            if (bases.onThird) { runsScored++; bases.onThird = false; }
            if (bases.onSecond) { runsScored++; bases.onSecond = false; }
            if (bases.onFirst) { bases.onThird = true; bases.onFirst = false; }
            // record runs
            if (runsScored > 0) box->recordAtBat(isHome, batter, 0, 0, 0, 0, 0, runsScored, 0, 0, runsScored);
            bases.onSecond = true;
            std::cout << "  " << batter->getName() << " doubles.\n";
        }
        else if (r < 80)
        {
            // 5% triple
            box->recordHit(isHome, batter, false, true, false);
            int runsScored = 0;
            if (bases.onThird) { runsScored++; bases.onThird = false; }
            if (bases.onSecond) { runsScored++; bases.onSecond = false; }
            if (bases.onFirst) { runsScored++; bases.onFirst = false; }
            // record runs
            if (runsScored > 0) box->recordAtBat(isHome, batter, 0, 0, 0, 0, 0, runsScored, 0, 0, runsScored);
            bases.onThird = true;
            std::cout << "  " << batter->getName() << " triples.\n";
        }
        else if (r < 90)
        {
            // 10% HR
            box->recordHit(isHome, batter, false, false, true);
            int runsScored = 1; // batter
            if (bases.onThird) { runsScored++; bases.onThird = false; }
            if (bases.onSecond) { runsScored++; bases.onSecond = false; }
            if (bases.onFirst) { runsScored++; bases.onFirst = false; }
            // record runs
            box->recordAtBat(isHome, batter, 0, 0, 0, 0, runsScored - 1, 0, 0, 0, runsScored);
            std::cout << "  " << batter->getName() << " hits a HOME RUN! " << runsScored << " runs.\n";
        }
        else
        {
            // 10% in-play out => e.g. flyout / groundout
            outs++;
            box->recordAtBat(isHome, batter, 1, 0, 0, 0, 0, 0, 0, 0, 0); // 1 AB, 0 hits
            std::cout << "  " << batter->getName() << " is out in play. (Out #" << outs << ")\n";
        }
    }
}

// ----------------------------------------------------
// Simulate one half-inning (top or bottom).
// We'll keep going until we get 3 outs or a special condition
// ----------------------------------------------------
void simulateHalfInning(Team* battingTeam,
    Team* pitchingTeam,
    BoxScore* box,
    bool isHomeBatting,
    int inningNumber,
    Simulator* sim)
{
    std::cout << "\n--- " << (isHomeBatting ? "Bottom" : "Top")
        << " of Inning " << inningNumber << " ---\n";

    // We'll get the lineup vs RHP for demonstration
    // (You might check if the pitchingTeam's starter is left or right)
    auto& lineup = battingTeam->getLineupVsRHP();
    static int awayBatterIndex = 0;
    static int homeBatterIndex = 0;

    int& batterIndex = (isHomeBatting ? homeBatterIndex : awayBatterIndex);

    // We'll pick the "pitcher" from the pitchingTeam's rotation for demonstration
    // Real code would do a daily starter. We'll just do the first rotation pitcher.
    auto rotation = pitchingTeam->getRotation();
    if (rotation.getPitchers().empty())
    {
        std::cerr << "[Error] " << pitchingTeam->getName() << " has no pitchers!\n";
        return;
    }
    Player* pitcher = rotation.getPitchers()[0];

    // We track outs
    int outs = 0;
    BaseRunners baseState; // no runners initially

    while (outs < 3)
    {
        // get the next batter
        Player* batter = nullptr;
        if (!lineup.empty())
        {
            batter = lineup[batterIndex % lineup.size()];
        }
        else
        {
            std::cerr << "[Error] lineup is empty for " << battingTeam->getName() << "\n";
            return;
        }
        simulateAtBat(battingTeam, batter, pitcher, box, isHomeBatting, outs, baseState, sim);

        batterIndex++;
        if (outs >= 3) break;
    }
}

// ----------------------------------------------------
// Full 9-inning (or more) simulation
// If tied after 9, do extras until a winner emerges
// ----------------------------------------------------
void simulateFullGame()
{
    // 1) Create the two teams
    Team* awayTeam = createTeam("RedSox");
    Team* homeTeam = createTeam("Yankees");

    // 2) Create BoxScore
    BoxScore box(homeTeam->getName(), awayTeam->getName(), "Yankee Stadium", 35000);

    // 3) Create a random-based Simulator
    Simulator sim;

    // Print starting pitchers
    auto awayRotation = awayTeam->getRotation();
    auto homeRotation = homeTeam->getRotation();
    if (!awayRotation.getPitchers().empty() && !homeRotation.getPitchers().empty())
    {
        std::cout << "Starting Pitchers:\n";
        std::cout << " Away: " << awayRotation.getPitchers()[0]->getName() << "\n";
        std::cout << " Home: " << homeRotation.getPitchers()[0]->getName() << "\n\n";
    }

    // 4) Loop innings
    int inning = 1;
    bool gameOver = false;
    while (!gameOver)
    {
        // Top
        simulateHalfInning(awayTeam, homeTeam, &box, false, inning, &sim);
        int awayRuns = box.getAwayTeamRuns();
        int homeRuns = box.getHomeTeamRuns();
        // check if home is behind after top of 9+ => if innings>=9 and away>home => game ends? (not quite correct for "walk-off")
        if (inning >= 9 && awayRuns > homeRuns)
        {
            std::cout << "\n[Game End] Away leads after top " << inning << ".\n";
            gameOver = true;
            break;
        }

        // Bottom
        simulateHalfInning(homeTeam, awayTeam, &box, true, inning, &sim);
        awayRuns = box.getAwayTeamRuns();
        homeRuns = box.getHomeTeamRuns();
        if (inning >= 9 && homeRuns > awayRuns)
        {
            std::cout << "\n[Game End] Home wins in walk-off.\n";
            gameOver = true;
            break;
        }

        // If 9 innings done, check if tie
        if (inning >= 9 && (awayRuns != homeRuns))
        {
            std::cout << "\n[Game End] 9 innings complete, no tie.\n";
            gameOver = true;
            break;
        }
        // else go to next inning
        inning++;
        if (inning > 20) // safety break
        {
            std::cout << "[Error] Exceeding 20 innings, let's stop.\n";
            gameOver = true;
        }
    }

    // 5) Print final Box Score
    std::cout << "\n===== Final Box Score =====\n";
    std::cout << "  " << awayTeam->getName() << ": " << box.getAwayTeamRuns() << "\n";
    std::cout << "  " << homeTeam->getName() << ": " << box.getHomeTeamRuns() << "\n";

    // If you want, print each player's line:
    std::cout << "\nAway Batting Lines:\n";
    for (const auto& line : box.getAwayBattingLines())
    {
        if (!line.player) continue;
        std::cout << " " << line.player->getName()
            << ": AB=" << line.atBats
            << ", H=" << line.hits
            << ", HR=" << line.homeRuns
            << ", R=" << line.runs
            << ", RBI=" << line.runsBattedIn
            << ", BB=" << line.walks
            << ", K=" << line.strikeouts
            << "\n";
    }
    std::cout << "\nHome Batting Lines:\n";
    for (const auto& line : box.getHomeBattingLines())
    {
        if (!line.player) continue;
        std::cout << " " << line.player->getName()
            << ": AB=" << line.atBats
            << ", H=" << line.hits
            << ", HR=" << line.homeRuns
            << ", R=" << line.runs
            << ", RBI=" << line.runsBattedIn
            << ", BB=" << line.walks
            << ", K=" << line.strikeouts
            << "\n";
    }

    std::cout << "\n===== Game Simulation Complete =====\n";
}

// ----------------------------------------------------
// The actual test function to call from main() or any test harness
// ----------------------------------------------------
int main()
{
    // seed random
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    simulateFullGame();
    return 0;
}
