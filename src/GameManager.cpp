#include "GameManager.h"
#include <iostream>
#include <cassert>

namespace BBEngine
{
    // ----------------------------------------------------
    // Constructor
    // ----------------------------------------------------
    GameManager::GameManager(Team* home, Team* away, BoxScore* box, Simulator* sim)
        : homeTeam(home),
        awayTeam(away),
        boxScore(box),
        simulator(sim),
        inning(1),
        topOfInning(true),
        outs(0),
        gameOver(false),
        runnerOnFirst(false),
        runnerOnSecond(false),
        runnerOnThird(false),
        homeBatterIndex(0),
        awayBatterIndex(0)
    {
        // We assume teams, boxscore, simulator are not null in this example
        // or we'd add checks/throws.
    }

    bool GameManager::isGameOver() const
    {
        return gameOver;
    }

    int GameManager::getCurrentInning() const
    {
        return inning;
    }

    bool GameManager::isTopOfInning() const
    {
        return topOfInning;
    }

    // ----------------------------------------------------
    // runGame - main loop
    // ----------------------------------------------------
    void GameManager::runGame()
    {
        // We'll run until gameOver is true or we decide we've done enough innings.
        while (!gameOver)
        {
            runHalfInning();
            checkGameOver();
            if (gameOver) break;

            endHalfInning();
            if (!gameOver) // if still not over, run the other half
            {
                runHalfInning();
                checkGameOver();
                endHalfInning();
            }
        }

        // By now, gameOver should be true. The boxScore likely has final results.
        // You might do a boxScore->assignPitchingDecision(...) if you track wins/losses here.
    }

    // ----------------------------------------------------
    // runHalfInning
    // ----------------------------------------------------
    void GameManager::runHalfInning()
    {
        // Reset outs & base runners if new half
        startHalfInning();

        while (outs < 3 && !gameOver)
        {
            proceedPitch();
        }
    }

    // ----------------------------------------------------
    // proceedPitch: do a single pitch
    // ----------------------------------------------------
    void GameManager::proceedPitch()
    {
        // 1. Identify the next batter, pitcher, etc.
        bool isHomeBatting = !topOfInning; // if topOfInning=true => away is batting
        Team* battingTeam = (isHomeBatting ? homeTeam : awayTeam);
        Team* fieldingTeam = (isHomeBatting ? awayTeam : homeTeam);

        // We'll get the next batter from the correct lineup. We'll do a naive approach: always vsRHP
        // In reality, you'd check the actual pitcher handedness, but let's skip that for brevity.
        int& batterIndex = (isHomeBatting ? homeBatterIndex : awayBatterIndex);
        const auto& lineup = battingTeam->getLineupVsRHP();
        if (lineup.empty())
        {
            // can't proceed if no lineup
            // we could skip or throw
            std::cerr << "No lineup set for " << (isHomeBatting ? "Home" : "Away") << " team.\n";
            gameOver = true;
            return;
        }
        Player* batter = lineup[batterIndex];

        // Next, figure out the current pitcher. If we do a simple approach, the fieldingTeam's rotation->getNextStarter()
        // But let's say we do that once per game or per half. We'll do a naive approach:
        StartingRotation* rotation = fieldingTeam->getRotation();
        if (!rotation || rotation->getPitchers().empty())
        {
            std::cerr << "No pitcher for fielding team.\n";
            gameOver = true;
            return;
        }
        Player* pitcher = rotation->getNextStarter();

        // 2. Gather attributes for simulator
        PlayerAttributes* pitcherAttr = pitcher->getAttributes();
        PlayerAttributes* batterAttr = batter->getAttributes();
        if (!pitcherAttr || !batterAttr)
        {
            std::cerr << "Missing attributes for pitcher or batter.\n";
            gameOver = true;
            return;
        }

        // We'll do a simplistic pitch context (no ball/strike count here for brevity).
        PitchContext pitchCtx;
        // pitchCtx.balls = ... 
        // pitchCtx.strikes = ...
        // We'll skip ball/strike count logic for brevity in this example, 
        // and treat each pitch as if it decides the at-bat immediately.

        StadiumContext stadium; // if you want to pass real data, do so

        // 3. Call simulator
        PitchOutcome outcome = simulator->simulatePitch(*pitcherAttr, *batterAttr, stadium, pitchCtx);

        // 4. If we get an in-play outcome, compute BattedBallOutcome
        if (outcome == PitchOutcome::BATTED_BALL_IN_PLAY)
        {
            BattedBallOutcome hitResult = simulator->computeBattedBallOutcome(*pitcherAttr, *batterAttr, stadium);

            // We'll handle base runners and record in BoxScore
            handleBattedBall(hitResult, isHomeBatting, batter, pitcher);

            // Once a ball is put in play, that at-bat ends. 
            // So increment the batter index
            batterIndex = (batterIndex + 1) % lineup.size();
        }
        else if (outcome == PitchOutcome::BALL)
        {
            // We'll treat it as a walk => put batter on 1st, at-bat ends
            // If runnerOnFirst, move them up, etc. Let's do minimal logic.
            if (!runnerOnFirst) runnerOnFirst = true;
            else if (!runnerOnSecond) runnerOnSecond = true;
            else if (!runnerOnThird) runnerOnThird = true; // naive cascade

            // record a walk in boxscore
            boxScore->recordWalk(isHomeBatting, batter);

            // at-bat ends, next batter
            batterIndex = (batterIndex + 1) % lineup.size();
        }
        else if (outcome == PitchOutcome::FOUL)
        {
            // Typically, we'd increment strike if <2, else it's a normal foul. 
            // For simplicity, let's call it a foul "strike" but not an out unless it's the third strike. 
            // We'll do no change to outs, no change to base runners, continue the same batter if we do multiple pitches.
            // but we said we'd skip real ball/strike counting, so let's do an immediate next pitch => no at-bat end
            // We'll do nothing but a small message
            std::cout << "Foul ball (no at-bat end in this example). continuing pitch...\n";
        }
        else if (outcome == PitchOutcome::STRIKE_SWINGING
            || outcome == PitchOutcome::STRIKE_LOOKING)
        {
            // We'll call that a strikeout => out is incremented, at-bat ends
            outs++;
            boxScore->recordStrikeout(isHomeBatting, batter);

            // next batter
            batterIndex = (batterIndex + 1) % lineup.size();
        }
    }

    // ----------------------------------------------------
    // checkGameOver
    // ----------------------------------------------------
    void GameManager::checkGameOver()
    {
        // If we've completed 9 innings, or a walk-off scenario
        // For now let's do a naive approach: if inning >= 9, see if awayRuns != homeRuns
        // Actually let's do if we completed bottom of an inning and have a lead:
        if (inning >= 9)
        {
            int homeRuns = boxScore->getHomeTeamRuns();
            int awayRuns = boxScore->getAwayTeamRuns();

            // If the game is not tied => gameOver
            if (homeRuns != awayRuns && (!topOfInning)) // we've just finished bottom
            {
                gameOver = true;
            }
            else if (homeRuns != awayRuns && (inning > 9))
            {
                // in extras, if we just finished topOfInning==false => means we finished bottom half
                // or if the home team leads after top half => if away is behind => game over
                gameOver = true;
            }
        }
    }

    // ----------------------------------------------------
    // startHalfInning
    // ----------------------------------------------------
    void GameManager::startHalfInning()
    {
        outs = 0;
        // we keep base runners from previous half if we want a "clean" approach we do:
        runnerOnFirst = false;
        runnerOnSecond = false;
        runnerOnThird = false;
    }

    // ----------------------------------------------------
    // endHalfInning
    // ----------------------------------------------------
    void GameManager::endHalfInning()
    {
        // Flip topOfInning if outs=3
        topOfInning = !topOfInning;

        if (topOfInning) // we just finished the bottom
        {
            // increment the inning
            inning++;
        }
    }

    void GameManager::handleBattedBall(BattedBallOutcome outcome, bool isHome, Player* batter, Player* pitcher)
    {
        int bases = 0; // number of bases the batter earns
        int runsScored = 0;

        switch (outcome)
        {
        case BattedBallOutcome::SINGLE:
            bases = 1;
            break;
        case BattedBallOutcome::DOUBLE_:
            bases = 2;
            break;
        case BattedBallOutcome::TRIPLE:
            bases = 3;
            break;
        case BattedBallOutcome::HOMERUN:
            bases = 4;
            break;
        case BattedBallOutcome::OUT:
            outs++;
            // record out in boxScore
            // We can do: boxScore->recordAtBat(isHome, batter, 1,0,0,0,0,0,0,0,0); // an out is typically 1 AB
            // but let's skip details
            return;
        }

        // If bases > 0 => it's a hit
        // recordAtBat: e.g. 1 AB, 1 hit if bases>0
        int hits = 1;
        int isDouble = (bases == 2) ? 1 : 0;
        int isTriple = (bases == 3) ? 1 : 0;
        int isHR = (bases == 4) ? 1 : 0;

        // We'll do a naive approach: if it's a HR, that automatically scores the batter + all runners
        // if single/double/triple, we only do a naive approach
        if (bases == 4)
        {
            // Score all on base + batter
            int runs = 1; // the batter
            if (runnerOnFirst) runs++;
            if (runnerOnSecond) runs++;
            if (runnerOnThird) runs++;

            // record runs in boxScore
            boxScore->recordAtBat(isHome, batter,
                1, // ab
                hits, isDouble, isTriple, isHR,
                runs - 1, // rbis => all except the batter's run if you do that logic
                0, // walks
                0, // K
                runs // runs scored by the batter
            );

            // Clear the bases
            runnerOnFirst = runnerOnSecond = runnerOnThird = false;
        }
        else
        {
            // For single/double/triple, let's do a minimal approach:
            // always move each runner up "bases" steps, scoring any who pass home
            // move runner on third -> home => +1 run if bases >=1
            int runCount = 0;
            if (runnerOnThird && bases >= 1) { runCount++; runnerOnThird = false; }
            // move runner on second-> third if bases>=1, ->home if bases>=2
            if (runnerOnSecond)
            {
                runnerOnSecond = false;
                if (bases >= 2) { runCount++; }
                else { runnerOnThird = true; }
            }
            // runner on first-> second if bases>=1, -> third if bases>=2, ->home if bases>=3
            if (runnerOnFirst)
            {
                runnerOnFirst = false;
                if (bases >= 3) { runCount++; }
                else if (bases == 2) { runnerOnThird = true; }
                else { runnerOnSecond = true; }
            }

            // Batter ends up on base if bases<4
            if (bases == 1) runnerOnFirst = true;
            else if (bases == 2) runnerOnSecond = true;
            else if (bases == 3) runnerOnThird = true;

            // record in boxScore
            boxScore->recordAtBat(isHome, batter,
                1, // ab
                hits, isDouble, isTriple, isHR,
                runCount, // rbis
                0, // walks
                0, // K
                runCount > 0 ? 1 : 0 // if the batter actually scored, e.g. triple with enough error
            );
        }
    }

} // namespace BBEngine
