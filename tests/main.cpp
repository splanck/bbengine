#include <iostream>
#include <iomanip>  // for std::setw or formatting
#include "BoxScore.h"
#include "Player.h" // for Player creation
#include "PlayerAttributes.h"
#include "PlayerStats.h"
#include "StartingRotation.h"
#include "Team.h"
#include "Simulator.h"
#include "GameManager.h"
#include "Schedule.h"
#include "Standings.h"
#include "Season.h"

using namespace BBEngine;

void testGameManager()
{
    using namespace BBEngine;

    std::cout << "---- Testing GameManager ----\n";

    // 1. Setup teams
    Team home("HomeTown Heroes", "MLB");
    Team away("AwayTown Visitors", "MLB");

    // 2. Setup lineups (we only do 3 players for brevity)
    // In real usage, you'd do 9 or more
    PlayerAttributes* attr1 = new PlayerAttributes();
    PlayerStats* stats1 = new PlayerStats();
    Player p1("HomeBatter1", 25, Handedness::Right, attr1, stats1);

    PlayerAttributes* attr2 = new PlayerAttributes();
    PlayerStats* stats2 = new PlayerStats();
    Player p2("HomeBatter2", 29, Handedness::Left, attr2, stats2);

    PlayerAttributes* attr3 = new PlayerAttributes();
    PlayerStats* stats3 = new PlayerStats();
    Player p3("AwayBatter1", 26, Handedness::Right, attr3, stats3);

    home.addPlayer(&p1);
    home.addPlayer(&p2);

    away.addPlayer(&p3);

    // Set lineups
    std::vector<Player*> homeLineup{ &p1, &p2 };
    std::vector<Player*> awayLineup{ &p3 };
    home.setLineupVsRHP(homeLineup);
    away.setLineupVsRHP(awayLineup);

    // 3. Setup starting pitchers
    // We'll just say each team has 1 "starter"
    PlayerAttributes* pattrH = new PlayerAttributes();
    PlayerStats* pstatsH = new PlayerStats();
    Player homePitcher("HomePitcher", 30, Handedness::Right, pattrH, pstatsH);

    PlayerAttributes* pattrA = new PlayerAttributes();
    PlayerStats* pstatsA = new PlayerStats();
    Player awayPitcher("AwayPitcher", 31, Handedness::Left, pattrA, pstatsA);

    StartingRotation homeRotation({ &homePitcher });
    StartingRotation awayRotation({ &awayPitcher });
    home.setRotation(&homeRotation);
    away.setRotation(&awayRotation);

    // 4. Create a BoxScore & Simulator
    BoxScore box("HomeTown Heroes", "AwayTown Visitors");
    Simulator sim;

    // 5. Create GameManager
    GameManager gm(&home, &away, &box, &sim);

    // 6. Run the game
    gm.runGame();

    // Print final scores
    std::cout << "Game finished. Final Score:\n";
    std::cout << "  Home: " << box.getHomeTeamRuns() << "\n";
    std::cout << "  Away: " << box.getAwayTeamRuns() << "\n";
    std::cout << "---- End of GameManager test ----\n\n";

    // Cleanup your new allocations
    delete attr1; delete stats1;
    delete attr2; delete stats2;
    delete attr3; delete stats3;
    delete pattrH; delete pstatsH;
    delete pattrA; delete pstatsA;
    // The players themselves might be on the stack in this example, 
    // or you store them in the Team for the entire simulation. 
    // In a bigger system, you'd manage memory carefully.
}

void testTeam()
{
    std::cout << "-- Testing Team --\n";

    // Create a team named "Boston Red Sox" at the "MLB" level
    Team team("Boston Red Sox", "MLB");
    assert(team.getName() == "Boston Red Sox");
    assert(team.getLevel() == "MLB");
    assert(team.getRoster().empty());

    // 1. Create a few players
    PlayerAttributes* attr1 = new PlayerAttributes();
    PlayerStats* stats1 = new PlayerStats();
    Player p1("PlayerOne", 28, Handedness::Left, attr1, stats1);

    PlayerAttributes* attr2 = new PlayerAttributes();
    PlayerStats* stats2 = new PlayerStats();
    Player p2("PlayerTwo", 30, Handedness::Right, attr2, stats2);

    // 2. Add them to the roster
    team.addPlayer(&p1);
    team.addPlayer(&p2);
    assert(team.getRoster().size() == 2);

    // 3. Setup lineups
    std::vector<Player*> vsRHP{ &p1 };
    std::vector<Player*> vsLHP{ &p2 };
    team.setLineupVsRHP(vsRHP);
    team.setLineupVsLHP(vsLHP);

    // Confirm lineups
    assert(team.getLineupVsRHP().size() == 1);
    assert(team.getLineupVsRHP()[0] == &p1);
    assert(team.getLineupVsLHP().size() == 1);
    assert(team.getLineupVsLHP()[0] == &p2);

    // 4. Setup a rotation
    std::vector<Player*> pitchers;
    pitchers.push_back(&p1); // pretend he's also a pitcher, just for demo
    pitchers.push_back(&p2);

    StartingRotation* rotation = new StartingRotation(pitchers);
    team.setRotation(rotation);
    assert(team.getRotation() != nullptr);
    assert(team.getRotation()->getPitchers().size() == 2);
    // Next starter => p1
    assert(team.getRotation()->getNextStarter() == &p1);

    // 5. Remove a player
    team.removePlayer(&p1);
    // Now p1 is removed from the roster, lineups, and rotation if found
    assert(team.getRoster().size() == 1);       // only p2 remains
    assert(team.getLineupVsRHP().empty());      // p1 was removed from RHP lineup
    // Check rotation
    assert(team.getRotation()->getPitchers().size() == 1);
    assert(team.getRotation()->getPitchers()[0] == &p2);

    // Cleanup dynamic memory
    delete rotation;  // if you want to avoid memory leaks
    delete attr1;
    delete stats1;
    delete attr2;
    delete stats2;

    std::cout << "Team tests passed.\n";
}

/**
 * A quick test function to populate a BoxScore with data
 * and print the results. Integrate this into your main.cpp
 * alongside other test functions.
 */
void testBoxScore()
{
    std::cout << "---- Testing BoxScore ----\n";

    // 1. Create a BoxScore for Home and Away teams
    BoxScore box("HomeTown Heroes", "AwayTown Visitors");

    // 2. Create some mock players for demonstration
    // (In a real engine, you might have a full Player with attributes & stats)
    PlayerAttributes* attrA = new PlayerAttributes();
    PlayerStats* statsA = new PlayerStats();
    Player            homeBatterA("Bill Batter", 28, Handedness::Right, attrA, statsA);

    PlayerAttributes* attrB = new PlayerAttributes();
    PlayerStats* statsB = new PlayerStats();
    Player            homePitcher("Hank Hurls", 30, Handedness::Left, attrB, statsB);

    PlayerAttributes* attrC = new PlayerAttributes();
    PlayerStats* statsC = new PlayerStats();
    Player            awayBatterA("Sally Slugger", 24, Handedness::Right, attrC, statsC);

    PlayerAttributes* attrD = new PlayerAttributes();
    PlayerStats* statsD = new PlayerStats();
    Player            awayPitcher("Amy Arms", 27, Handedness::Left, attrD, statsD);

    // 3. Add them to lineups / pitching lines
    box.addBatterToLineup(true, &homeBatterA);
    box.addBatterToLineup(false, &awayBatterA);
    box.addPitcher(true, &homePitcher);
    box.addPitcher(false, &awayPitcher);

    // 4. Record some batting events
    // Home Batter: 1 single, 1 double, 1 strikeout
    box.recordHit(true, &homeBatterA, /*double=*/false, /*triple=*/false, /*homerun=*/false);
    // That increments atBats=1, hits=1
    box.recordHit(true, &homeBatterA, /*double=*/true, /*triple=*/false, /*homerun=*/false);
    // Another at-bat, hits=2, doubles=1
    box.recordStrikeout(true, &homeBatterA);
    // Another at-bat, strikeout

    // Suppose he also scored 1 run in that double:
    box.recordAtBat(true, &homeBatterA, 0, 0, 0, 0, 0, 0, 0, 0, 1);

    // Away Batter: 1 walk, 1 triple
    box.recordWalk(false, &awayBatterA);  // no at-bat
    box.recordHit(false, &awayBatterA, /*double=*/false, /*triple=*/true, /*homerun=*/false);
    // atBats=1, hits=1, triple=1
    // Suppose that triple scored 1 run:
    box.recordAtBat(false, &awayBatterA, 0, 0, 0, 0, 0, 0, 0, 0, 1);

    // 5. Record some pitching events
    // Home Pitcher (Hank Hurls) pitched 5.2 innings, allowed 4 hits, 2 runs, 2 earned runs, 1 walk, 4 K
    box.recordPitching(true, &homePitcher, 5.2, 4, 2, 2, 1, 4);

    // Away Pitcher (Amy Arms) pitched 6.0 innings, allowed 6 hits, 3 runs, 3 earned runs, 2 walks, 6 K
    box.recordPitching(false, &awayPitcher, 6.0, 6, 3, 3, 2, 6);

    // 6. Summaries: Let's just print final runs for each side
    int homeRuns = box.getHomeTeamRuns();
    int awayRuns = box.getAwayTeamRuns();

    std::cout << "Final Tally (Simple): \n"
        << "Home runs: " << homeRuns << "\n"
        << "Away runs: " << awayRuns << "\n\n";

    // 7. Print batting lines for both teams
    std::cout << "Home Batting Lines:\n";
    for (const auto& line : box.getHomeBattingLines())
    {
        if (!line.player) continue;
        std::cout << "  Batter: " << line.player->getName()
            << ", AB=" << line.atBats
            << ", H=" << line.hits
            << ", 2B=" << line.doubles_
            << ", 3B=" << line.triples
            << ", HR=" << line.homeRuns
            << ", BB=" << line.walks
            << ", K=" << line.strikeouts
            << ", R=" << line.runsScored
            << "\n";
    }

    std::cout << "\nAway Batting Lines:\n";
    for (const auto& line : box.getAwayBattingLines())
    {
        if (!line.player) continue;
        std::cout << "  Batter: " << line.player->getName()
            << ", AB=" << line.atBats
            << ", H=" << line.hits
            << ", 2B=" << line.doubles_
            << ", 3B=" << line.triples
            << ", HR=" << line.homeRuns
            << ", BB=" << line.walks
            << ", K=" << line.strikeouts
            << ", R=" << line.runsScored
            << "\n";
    }

    // 8. Print pitching lines
    std::cout << "\nHome Pitching Lines:\n";
    for (const auto& pline : box.getHomePitchingLines())
    {
        if (!pline.pitcher) continue;
        std::cout << "  Pitcher: " << pline.pitcher->getName()
            << ", IP=" << pline.inningsPitched
            << ", H=" << pline.hitsAllowed
            << ", R=" << pline.runsAllowed
            << ", ER=" << pline.earnedRuns
            << ", BB=" << pline.walksAllowed
            << ", K=" << pline.strikeouts
            << (pline.decisionWin ? ", W" : "")
            << (pline.decisionLoss ? ", L" : "")
            << (pline.decisionSave ? ", S" : "")
            << "\n";
    }

    std::cout << "\nAway Pitching Lines:\n";
    for (const auto& pline : box.getAwayPitchingLines())
    {
        if (!pline.pitcher) continue;
        std::cout << "  Pitcher: " << pline.pitcher->getName()
            << ", IP=" << pline.inningsPitched
            << ", H=" << pline.hitsAllowed
            << ", R=" << pline.runsAllowed
            << ", ER=" << pline.earnedRuns
            << ", BB=" << pline.walksAllowed
            << ", K=" << pline.strikeouts
            << (pline.decisionWin ? ", W" : "")
            << (pline.decisionLoss ? ", L" : "")
            << (pline.decisionSave ? ", S" : "")
            << "\n";
    }

    // 9. Optionally, you can assign a Win/Loss/Save after deciding the final
    //    scoreboard (home 4, away 3, etc.). For example, if we decided
    //    Hank Hurls got the Win, Amy Arms got the Loss:
    // box.assignPitchingDecision(true, &homePitcher, true, false, false);
    // box.assignPitchingDecision(false, &awayPitcher, false, true, false);

    std::cout << "\n---- BoxScore test completed ----\n";
}

/**
 * Test function for PlayerAttributes class.
 */
void testPlayerAttributes()
{
    std::cout << "---- Testing PlayerAttributes ----\n";

    // 1. Default constructor check
    PlayerAttributes attr;
    // Typically we set each rating to 50, so let's check a couple
    assert(attr.getContact() == 50);
    assert(attr.getPower() == 50);

    // 2. Out-of-range set & clamp
    attr.setContact(120);  // Above 99
    assert(attr.getContact() == 99 && "Contact should clamp to 99 if set above range");
    attr.setContact(0);    // Below 1
    assert(attr.getContact() == 1 && "Contact should clamp to 1 if set below range");

    // 3. Another example
    attr.setPower(75);
    assert(attr.getPower() == 75);

    // 4. Optional: Checking averageHittingRating (just a demonstration)
    attr.setPlateDiscipline(60);
    double avgHit = attr.averageHittingRating();
    // This is (contact + power + plateDiscipline)/3 => (1 + 75 + 60)/3 => ~ 45.333
    // But remember contact was just set to 1 by our clamp test. 
    // Let's quickly confirm logic is consistent:
    std::cout << "Average Hitting Rating = " << avgHit << " (Should be around 45.333)\n";

    // 5. Demonstrate clampAllAttributes
    attr.setPitchVelocity(150); // out of range
    attr.setPitchControl(-10);  // out of range
    attr.clampAllAttributes();
    assert(attr.getPitchVelocity() == 99);
    assert(attr.getPitchControl() == 1);

    std::cout << "PlayerAttributes tests passed.\n\n";
}

/**
 * Test function for PlayerStats class.
 */
void testPlayerStats()
{
    std::cout << "---- Testing PlayerStats ----\n";

    PlayerStats stats;  // all counters start at 0

    // 1. Basic increments
    stats.addAtBats(4);
    stats.addHits(2);
    // Batting average: 2/4 = .500
    assert(stats.getBattingAverage() == 0.5);

    stats.addWalks(1); // OBP = (hits + walks) / (atBats + walks) => (2+1)/(4+1)=3/5=0.6
    double obp = stats.getOnBasePercentage();
    assert(obp == 0.6);

    // 2. Testing pitching increments
    stats.addInningsPitched(2.1); // 2 and 1/3
    stats.addEarnedRuns(1);       // ERA => (1 * 9) / 2.1 ~ 4.2857
    double era = stats.getERA();
    std::cout << "ERA so far: " << era << " (Expect ~4.2857)\n";
    // No direct assert here for floating precision, but let's do a rough check:
    assert(era > 4.2 && era < 4.3);

    // 3. Check zero checks
    PlayerStats empty;
    // getBattingAverage => 0 if atBats=0, getERA => 0 if inningsPitched=0
    assert(empty.getBattingAverage() == 0.0);
    assert(empty.getERA() == 0.0);

    // 4. Reset stats
    stats.resetStats();
    assert(stats.getAtBats() == 0);
    assert(stats.getInningsPitched() == 0.0);

    std::cout << "PlayerStats tests passed.\n\n";
}

/**
 * Test function for Player class.
 */
void testPlayer()
{
    std::cout << "---- Testing Player ----\n";

    // We'll create some PlayerAttributes and PlayerStats for demonstration
    PlayerAttributes* attr = new PlayerAttributes(); // starts at ~ 50 each
    PlayerStats* stats = new PlayerStats();

    // 1. Construct a Player with non-null pointers
    Player p("John Doe", 25, Handedness::Right, attr, stats);
    assert(p.getName() == "John Doe");
    assert(p.getAge() == 25);
    assert(p.getHandedness() == Handedness::Right);
    assert(p.getAttributes() == attr);
    assert(p.getStats() == stats);

    // 2. Change some personal info
    p.setName("Johnny Baseball");
    p.setAge(26);
    assert(p.getName() == "Johnny Baseball");
    assert(p.getAge() == 26);

    // 3. Check if the attributes pointer remains the same
    PlayerAttributes* oldAttr = p.getAttributes();
    PlayerAttributes* newAttr = new PlayerAttributes();
    newAttr->setContact(80);
    p.setAttributes(newAttr);
    assert(p.getAttributes() != oldAttr);
    assert(p.getAttributes()->getContact() == 80);

    // 4. Use the incremental age method
    p.incrementAge();
    // Should become 27 if the setAge logic is simply age++
    assert(p.getAge() == 27);

    // 5. Optionally test position / isActive
    p.setPosition("Catcher");
    assert(p.getPosition() == "Catcher");

    p.setActive(false);
    assert(!p.isActive());

    // Clean up dynamic allocations
    delete attr;    // but note we used attr for old pointer in step 3
    delete newAttr; // used in step 3
    delete stats;

    std::cout << "Player tests passed.\n\n";
}

void testStartingRotation()
{
    using namespace BBEngine;

    std::cout << "-- Testing StartingRotation --\n";

    // Create some mock players (pitchers)
    PlayerAttributes* attr1 = new PlayerAttributes();
    PlayerStats* stats1 = new PlayerStats();
    Player p1("PitcherA", 30, Handedness::Right, attr1, stats1);

    PlayerAttributes* attr2 = new PlayerAttributes();
    PlayerStats* stats2 = new PlayerStats();
    Player p2("PitcherB", 28, Handedness::Left, attr2, stats2);

    PlayerAttributes* attr3 = new PlayerAttributes();
    PlayerStats* stats3 = new PlayerStats();
    Player p3("PitcherC", 27, Handedness::Right, attr3, stats3);

    std::vector<Player*> initial{ &p1, &p2, &p3 };
    StartingRotation rotation(initial);

    // Confirm index=0
    assert(rotation.getNextStarterIndex() == 0);
    // getNextStarter => p1
    assert(rotation.getNextStarter() == &p1);

    // advanceRotation => index=1 => p2
    rotation.advanceRotation();
    assert(rotation.getNextStarterIndex() == 1);
    assert(rotation.getNextStarter() == &p2);

    // add a new pitcher
    PlayerAttributes* attr4 = new PlayerAttributes();
    PlayerStats* stats4 = new PlayerStats();
    Player p4("PitcherD", 25, Handedness::Left, attr4, stats4);

    rotation.addPitcher(&p4); // now we have 4 in rotation
    assert(rotation.getPitchers().size() == 4);

    // skip next starter => index=2 => p3, skip => index=3 => p4
    rotation.skipNextStarter();
    assert(rotation.getNextStarterIndex() == 2);
    assert(rotation.getNextStarter() == &p3);

    // remove pitcher at index=1 => that was p2
    rotation.removePitcher(1);
    // now p2 is removed, so the rotation is [p1, p3, p4]
    // index=3 => out of range, so wrap => index=3 % 3=0 => now nextStarterIndex=0 => p1
    // remove pitcher at index=1 => nextStarterIndex was 3 => becomes 2 => p4
    assert(rotation.getPitchers().size() == 3);
    
    //std::cout << rotation.getNextStarterIndex() << std::endl;
    //assert(rotation.getNextStarterIndex() == 1);
    //assert(rotation.getNextStarter() == &p4);


    // Cleanup any dynamic new
    // (In a real engine, these might persist or be managed by the Team.)
    delete attr1; delete stats1;
    delete attr2; delete stats2;
    delete attr3; delete stats3;
    delete attr4; delete stats4;

    std::cout << "StartingRotation tests passed.\n";
}

void testSimulator()
{
    std::cout << "---- Testing Simulator ----\n";

    // 1. Create a Simulator instance.
    Simulator simulator;

    // 2. Create pitcher and batter attributes with arbitrary values
    PlayerAttributes pitcherAttr;
    pitcherAttr.setPitchVelocity(80);   // moderate velocity
    pitcherAttr.setPitchControl(70);    // decent control
    pitcherAttr.setPitchMovement(60);   // optional
    // ignoring stamina, fielding, etc. for now

    PlayerAttributes batterAttr;
    batterAttr.setContact(65);         // moderate contact
    batterAttr.setPower(70);           // moderate power
    batterAttr.setPlateDiscipline(50); // average discipline

    // 3. Create a default stadium context
    StadiumContext stadium;
    stadium.fenceDistanceLeft = 330.0;
    stadium.fenceDistanceCenter = 400.0;
    stadium.fenceDistanceRight = 330.0;
    // ignoring wind/altitude for now

    // 4. We'll define a pitch context. 
    //    We'll keep it simple: no intentional walk, no pitchOut.
    PitchContext pitchCtx;
    pitchCtx.balls = 0;
    pitchCtx.strikes = 0;

    // 5. We'll run multiple pitches to see outcomes.
    // We'll store counts in a small map or separate variables.
    int totalPitches = 100;
    int countBall = 0;
    int countStrikeSwing = 0;
    int countStrikeLooking = 0;
    int countFoul = 0;
    int countBattedInPlay = 0;

    // For batted-ball outcomes
    int countSingle = 0;
    int countDouble = 0;
    int countTriple = 0;
    int countHR = 0;
    int countOut = 0;

    for (int i = 0; i < totalPitches; ++i)
    {
        // Each pitch might have a new or updated pitchCtx in a real game
        // but we keep it simple. If you do full at-bat logic, you'd track balls/strikes, etc.

        PitchOutcome outcome = simulator.simulatePitch(pitcherAttr, batterAttr, stadium, pitchCtx);
        switch (outcome)
        {
        case PitchOutcome::BALL:
            countBall++;
            // If you do full at-bat logic, you'd increment pitchCtx.balls
            // if pitchCtx.balls == 4 => walk, at-bat ends, etc.
            break;
        case PitchOutcome::STRIKE_SWINGING:
            countStrikeSwing++;
            // pitchCtx.strikes++ if you manage a real at-bat flow
            break;
        case PitchOutcome::STRIKE_LOOKING:
            countStrikeLooking++;
            break;
        case PitchOutcome::FOUL:
            countFoul++;
            // if pitchCtx.strikes < 2 => pitchCtx.strikes++
            break;
        case PitchOutcome::BATTED_BALL_IN_PLAY:
        {
            countBattedInPlay++;
            BattedBallOutcome hitOutcome =
                simulator.computeBattedBallOutcome(pitcherAttr, batterAttr, stadium);

            switch (hitOutcome)
            {
            case BattedBallOutcome::SINGLE:
                countSingle++;
                break;
            case BattedBallOutcome::DOUBLE_:
                countDouble++;
                break;
            case BattedBallOutcome::TRIPLE:
                countTriple++;
                break;
            case BattedBallOutcome::HOMERUN:
                countHR++;
                break;
            case BattedBallOutcome::OUT:
                countOut++;
                break;
            }
            break;
        }
        }
    }

    // 6. Print results
    std::cout << "Total Pitches: " << totalPitches << "\n";
    std::cout << "Balls: " << countBall << "\n";
    std::cout << "Strike Swinging: " << countStrikeSwing << "\n";
    std::cout << "Strike Looking: " << countStrikeLooking << "\n";
    std::cout << "Foul: " << countFoul << "\n";
    std::cout << "Batted In Play: " << countBattedInPlay << "\n";

    std::cout << "\nBatted-Ball Breakdown (In-Play Only):\n";
    std::cout << "  Single:  " << countSingle << "\n";
    std::cout << "  Double:  " << countDouble << "\n";
    std::cout << "  Triple:  " << countTriple << "\n";
    std::cout << "  HomeRun: " << countHR << "\n";
    std::cout << "  Out(?):  " << countOut << "\n";

    // Because it's random, there's no strict pass/fail. But we can do a sanity check
    // e.g., we might expect at least 1 ball or 1 strike out of 100. If we get 0, something is off.
    assert(countBall + countStrikeSwing + countStrikeLooking + countFoul + countBattedInPlay == totalPitches);

    // We can do a minimal check that we didn't get some insane distribution (like 100% home runs).
    // This is optional. We'll just do a naive check.
    std::cout << "---- Simulator test completed ----\n\n";
}

/**
 * A function that demonstrates a full 9-inning (or more) game
 * using random outcomes from your real Simulator.
 */
void testGameManagerRandomComprehensive()
{
    std::cout << "\n=== Starting Comprehensive Random GameManager Test ===\n\n";

    // 1. Create two Teams (home & away) with minimal rosters
    Team awayTeam("AwayTeam", "MLB");
    Team homeTeam("HomeTeam", "MLB");

    // We'll create a few players for each side:
    // For AWAY: 4 batters + 1 pitcher
    // For HOME: 4 batters + 1 pitcher
    // (In real usage, you'd have 9+ batters, multiple pitchers.)

    // AWAY players
    PlayerAttributes* aB1_attr = new PlayerAttributes();
    PlayerStats* aB1_stats = new PlayerStats();
    Player awayBatter1("AwayBatter1", 25, Handedness::Right, aB1_attr, aB1_stats);

    PlayerAttributes* aB2_attr = new PlayerAttributes();
    PlayerStats* aB2_stats = new PlayerStats();
    Player awayBatter2("AwayBatter2", 28, Handedness::Left, aB2_attr, aB2_stats);

    PlayerAttributes* aB3_attr = new PlayerAttributes();
    PlayerStats* aB3_stats = new PlayerStats();
    Player awayBatter3("AwayBatter3", 23, Handedness::Right, aB3_attr, aB3_stats);

    PlayerAttributes* aB4_attr = new PlayerAttributes();
    PlayerStats* aB4_stats = new PlayerStats();
    Player awayBatter4("AwayBatter4", 26, Handedness::Switch, aB4_attr, aB4_stats);

    PlayerAttributes* aP_attr = new PlayerAttributes();
    PlayerStats* aP_stats = new PlayerStats();
    Player awayPitcher("AwayPitcher", 30, Handedness::Right, aP_attr, aP_stats);

    // HOME players
    PlayerAttributes* hB1_attr = new PlayerAttributes();
    PlayerStats* hB1_stats = new PlayerStats();
    Player homeBatter1("HomeBatter1", 29, Handedness::Left, hB1_attr, hB1_stats);

    PlayerAttributes* hB2_attr = new PlayerAttributes();
    PlayerStats* hB2_stats = new PlayerStats();
    Player homeBatter2("HomeBatter2", 27, Handedness::Right, hB2_attr, hB2_stats);

    PlayerAttributes* hB3_attr = new PlayerAttributes();
    PlayerStats* hB3_stats = new PlayerStats();
    Player homeBatter3("HomeBatter3", 31, Handedness::Left, hB3_attr, hB3_stats);

    PlayerAttributes* hB4_attr = new PlayerAttributes();
    PlayerStats* hB4_stats = new PlayerStats();
    Player homeBatter4("HomeBatter4", 22, Handedness::Right, hB4_attr, hB4_stats);

    PlayerAttributes* hP_attr = new PlayerAttributes();
    PlayerStats* hP_stats = new PlayerStats();
    Player homePitcher("HomePitcher", 32, Handedness::Left, hP_attr, hP_stats);

    // 2. Add them to the teams
    // AWAY
    awayTeam.addPlayer(&awayBatter1);
    awayTeam.addPlayer(&awayBatter2);
    awayTeam.addPlayer(&awayBatter3);
    awayTeam.addPlayer(&awayBatter4);
    awayTeam.addPlayer(&awayPitcher);

    // HOME
    homeTeam.addPlayer(&homeBatter1);
    homeTeam.addPlayer(&homeBatter2);
    homeTeam.addPlayer(&homeBatter3);
    homeTeam.addPlayer(&homeBatter4);
    homeTeam.addPlayer(&homePitcher);

    // 3. Set lineups vs RHP for each (4 batters in a row, for demo)
    awayTeam.setLineupVsRHP({ &awayBatter1, &awayBatter2, &awayBatter3, &awayBatter4 });
    homeTeam.setLineupVsRHP({ &homeBatter1, &homeBatter2, &homeBatter3, &homeBatter4 });

    // 4. Create a rotation for each (just 1 pitcher, so no real rotation)
    StartingRotation awayRotation({ &awayPitcher });
    awayTeam.setRotation(&awayRotation);

    StartingRotation homeRotation({ &homePitcher });
    homeTeam.setRotation(&homeRotation);

    // 5. Create the BoxScore and a real random-based Simulator
    BoxScore box("HomeTeam", "AwayTeam");
    Simulator realSimulator; // uses random logic from your existing simulator

    // 6. Create the GameManager
    GameManager gameMgr(&homeTeam, &awayTeam, &box, &realSimulator);

    // 7. Run the game. This typically goes 9+ innings or ends if your game logic sees a walk-off, etc.
    gameMgr.runGame();

    // 8. Print final results from BoxScore
    int awayRuns = box.getAwayTeamRuns();
    int homeRuns = box.getHomeTeamRuns();
    std::cout << "\n--- Final Score ---\n";
    std::cout << "  AwayTeam: " << awayRuns << "\n";
    std::cout << "  HomeTeam: " << homeRuns << "\n\n";

    std::cout << "Away Batting Lines:\n";
    for (const auto& line : box.getAwayBattingLines())
    {
        if (!line.player) continue;
        std::cout << "  " << line.player->getName()
            << ": AB=" << line.atBats
            << ", H=" << line.hits
            << ", HR=" << line.homeRuns
            << ", R=" << line.runsScored
            << ", RBI=" << line.runsBattedIn
            << ", BB=" << line.walks
            << ", K=" << line.strikeouts
            << "\n";
    }

    std::cout << "\nHome Batting Lines:\n";
    for (const auto& line : box.getHomeBattingLines())
    {
        if (!line.player) continue;
        std::cout << "  " << line.player->getName()
            << ": AB=" << line.atBats
            << ", H=" << line.hits
            << ", HR=" << line.homeRuns
            << ", R=" << line.runsScored
            << ", RBI=" << line.runsBattedIn
            << ", BB=" << line.walks
            << ", K=" << line.strikeouts
            << "\n";
    }

    // Optionally, we can also show the pitching lines if your BoxScore logs them.

    // 9. Basic validation: game should be over
    assert(gameMgr.isGameOver() && "Game should have ended by now.");

    // 10. Clean up dynamic memory if needed (if these are not managed by some bigger system).
    delete aB1_attr; delete aB1_stats;
    delete aB2_attr; delete aB2_stats;
    delete aB3_attr; delete aB3_stats;
    delete aB4_attr; delete aB4_stats;
    delete aP_attr;  delete aP_stats;

    delete hB1_attr; delete hB1_stats;
    delete hB2_attr; delete hB2_stats;
    delete hB3_attr; delete hB3_stats;
    delete hB4_attr; delete hB4_stats;
    delete hP_attr;  delete hP_stats;

    std::cout << "=== End of Comprehensive Random GameManager Test ===\n\n";
}


void testSchedule()
{
    std::cout << "\n=== Testing Schedule ===\n\n";

    // 1. Create some teams
    Team t1("Yankees", "MLB");
    Team t2("RedSox", "MLB");
    Team t3("BlueJays", "MLB");

    // If you want to create real Players, do so, but the schedule doesn't strictly need them.
    std::vector<Team*> teams = { &t1, &t2, &t3 };

    // 2. Create a Schedule
    Schedule schedule;

    // 3. Generate a minimal schedule
    schedule.generateSchedule(teams);
    
    // 4. Check the total number of games
    // with 3 teams, each pair => 2 games (home/away) => 3C2=3 pairs, each pair 2 games => 6 total games
    const auto& allGames = schedule.getAllGames();
    assert(allGames.size() == 12);
    std::cout << "Hello" << allGames.size() << std::endl;
    std::cout << "All scheduled games:\n";
    for (const auto& g : allGames)
    {
        std::cout << " GameID=" << g.gameID
            << " date=" << g.date
            << " home=" << (g.homeTeam ? g.homeTeam->getName() : "null")
            << " away=" << (g.awayTeam ? g.awayTeam->getName() : "null")
            << " completed=" << g.completed
            << " score=" << g.awayScore << "-" << g.homeScore << "\n";
    }

    // 5. Suppose we pick the first game (index=0) to record a final score
    int someID = allGames[0].gameID;
    schedule.recordGameResult(someID, /*awayScore=*/5, /*homeScore=*/3);

    // confirm updated
    auto afterGames = schedule.getAllGames();
    bool foundOne = false;
    for (const auto& g : afterGames)
    {
        if (g.gameID == someID)
        {
            assert(g.completed == true);
            assert(g.awayScore == 5);
            assert(g.homeScore == 3);
            foundOne = true;
            std::cout << "Recorded result for gameID=" << g.gameID << " => away=5, home=3\n";
        }
    }
    assert(foundOne);

    // 6. Let's see what games are on date=2
    auto day2games = schedule.getGamesOn(2);
    std::cout << "\nGames on date=2:\n";
    for (const auto& g : day2games)
    {
        std::cout << " gameID=" << g.gameID
            << " " << (g.homeTeam ? g.homeTeam->getName() : "null")
            << " vs " << (g.awayTeam ? g.awayTeam->getName() : "null")
            << "\n";
    }
    // 7. Postpone the first day2 game to day=5
    if (!day2games.empty())
    {
        int gameToPostpone = day2games[0].gameID;
        schedule.postponeGame(gameToPostpone, /*newDate=*/5);
        auto updated = schedule.getAllGames();
        // Just confirm
        for (const auto& g : updated)
        {
            if (g.gameID == gameToPostpone)
            {
                assert(g.date == 5);
                std::cout << "Postponed gameID=" << g.gameID << " to new date=5\n";
            }
        }
    }

    // 8. Print final schedule state
    std::cout << "\nFinal schedule state:\n";
    for (const auto& g : schedule.getAllGames())
    {
        std::cout << "GameID=" << g.gameID
            << " date=" << g.date
            << " home=" << (g.homeTeam ? g.homeTeam->getName() : "null")
            << " away=" << (g.awayTeam ? g.awayTeam->getName() : "null")
            << " completed=" << g.completed
            << " score=" << g.awayScore << "-" << g.homeScore
            << "\n";
    }

    std::cout << "\n=== End of Schedule Test ===\n\n";
}

void testStandings()
{
    std::cout << "\n==== Testing Standings ====\n\n";

    // 1. Make some teams
    Team yankees("Yankees", "MLB");
    Team redSox("RedSox", "MLB");
    Team blueJays("BlueJays", "MLB");
    Team rays("Rays", "MLB");

    std::vector<Team*> alEast = { &yankees, &redSox, &blueJays, &rays };

    // 2. Construct a Standings object
    Standings alEastStandings(alEast);

    // initially, all 0-0
    for (auto& rec : alEastStandings.getOverallStandings())
    {
        assert(rec.wins == 0 && rec.losses == 0);
        // gamesBehind = 0 for top, or 0 for all if tie
    }

    // 3. Record some results
    // yankees beat redSox
    alEastStandings.recordGameResult(&yankees, &redSox);
    // redSox beat rays
    alEastStandings.recordGameResult(&redSox, &rays);
    // blueJays beat yankees
    alEastStandings.recordGameResult(&blueJays, &yankees);
    // yankees beat rays
    alEastStandings.recordGameResult(&yankees, &rays);

    // Now let's call updateStandings
    alEastStandings.updateStandings();

    // 4. Print out the standings in sorted order
    std::cout << "AL East Standings:\n";
    auto sorted = alEastStandings.getOverallStandings(); // after updateStandings, they are sorted
    for (const auto& rec : sorted)
    {
        // rec.team->getName()
        // rec.wins, rec.losses, rec.gamesBehind
        std::cout << "  " << rec.team->getName()
            << "  " << rec.wins << "-" << rec.losses
            << "  GB=" << rec.gamesBehind
            << "\n";
    }

    // 5. Basic checks
    // e.g. yankees might be 2-1, redSox 1-1, blueJays 1-0, rays 0-2 => depends on the order we recorded
    // Actually let's see:
    //   yankees: W over redSox, W over rays, L from blueJays => 2-1
    //   redSox:  L from yankees, W over rays => 1-1
    //   blueJays: W over yankees => 1-0
    //   rays: L from redSox, L from yankees => 0-2
    // If sorted, top is yankees(2-1) or blueJays(1-0)? Actually, 2-1 is a .667 win%, 1-0 is 1.0, so blueJays is top. 
    // Let's just ensure no errors or negative. We'll just confirm no crashes and final looks good.

    std::cout << "==== End of Standings Test ====\n\n";
}

void testSeason()
{
    std::cout << "\n==== Testing Season ====\n\n";

    // 1. Create some Teams
    Team t1("TeamA", "MLB");
    Team t2("TeamB", "MLB");
    Team t3("TeamC", "MLB");

    // In a real system, you might add players, lineups, etc. For Season's purposes, 
    // we just need a handful of teams to pass into schedule/standings.

    std::vector<Team*> teams = { &t1, &t2, &t3 };

    // 2. Create a Schedule & generate games
    Schedule schedule;
    schedule.generateSchedule(teams);
    // Possibly the schedule has 2 or 6 games, depending on your logic. 
    // For 3 teams, we might see 6 games as each pair meets twice.

    // 3. Create a Standings for these teams
    Standings standings(teams);
    // all starts at 0–0. 
    // We can confirm:
    for (const auto& rec : standings.getOverallStandings())
    {
        assert(rec.wins == 0 && rec.losses == 0);
    }

    // 4. Construct the Season
    Season season(teams, &schedule, &standings);

    season.startSeason();  // set flags

    // We'll do a naive day-by-day approach:
    // Let's see what dates we have in schedule. Typically might be day=1 or day=2 from your generation logic.

    // We'll assume day 1.. day 5 might exist. We'll just do a loop:
    for (int day = 1; day <= 5; ++day)
    {
        if (season.isSeasonOver())
            break;  // done

        std::cout << "Simulating day=" << day << "\n";
        season.simulateDay(day);

        if (season.isSeasonOver())
        {
            std::cout << "Season ended on day=" << day << "\n";
            break;
        }
    }

    // if not ended, finalize anyway
    if (!season.isSeasonOver())
        season.finalizeSeason();

    // 5. Print final standings
    auto finalStandings = standings.getOverallStandings();
    std::cout << "\n--- Final Standings ---\n";
    for (const auto& rec : finalStandings)
    {
        std::cout << " " << rec.team->getName()
            << ": " << rec.wins << "-" << rec.losses
            << "  GB=" << rec.gamesBehind << "\n";
    }

    std::cout << "==== End of Season Test ====\n\n";
}

int main()
{
    std::cout << "Hello, Baseball Engine!\n\n";

    testPlayerAttributes();
    testPlayerStats();
    testPlayer();
    testBoxScore();
    testStartingRotation();
    testTeam();
    testSimulator();
    testGameManager();
    testGameManagerRandomComprehensive();
    testSchedule();
    testStandings();
    testSeason();

    std::cout << "All tests completed successfully.\n";
    return 0;
}
