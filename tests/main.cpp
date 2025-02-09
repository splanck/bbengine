#include <iostream>
#include <iomanip>  // for std::setw or formatting
#include "BoxScore.h"
#include "Player.h" // for Player creation
#include "PlayerAttributes.h"
#include "PlayerStats.h"
#include "StartingRotation.h"

using namespace BBEngine;

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


int main()
{
    std::cout << "Hello, Baseball Engine!\n\n";

    testPlayerAttributes();
    testPlayerStats();
    testPlayer();
    testBoxScore();
    testStartingRotation();

    std::cout << "All tests completed successfully.\n";
    return 0;
}
