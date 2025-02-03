#include "Simulator.h"
#include "PlayerAttributes.h" // So we can pull from Player attributes
#include <random>
#include <stdexcept>
#include <iostream> // For debugging/logging if needed

namespace BBEngine
{
    // --------------------------------------
    // Random utility
    // --------------------------------------
    double Simulator::randomDouble()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<> dist(0.0, 1.0);
        return dist(gen);
    }

    // --------------------------------------
    // Helper attribute-based factors
    // --------------------------------------

    double Simulator::getBatterPowerFactor(const Player* batter)
    {
        if (!batter) return 0.5; // default fallback

        // Example: use the player's hitting power / 99.0
        // or combine contact + power into a single factor
        const auto& attrs = batter->getAttributes();
        double power   = attrs.getHitPower();  // 1..99
        double contact = attrs.getHitContact();
        return (power + contact / 2.0) / 99.0; // just an arbitrary formula
    }

    double Simulator::getBatterContactFactor(const Player* batter)
    {
        if (!batter) return 0.5;
        double contact = batter->getAttributes().getHitContact();
        return contact / 99.0;
    }

    double Simulator::getPitcherEffectiveness(const Player* pitcher)
    {
        if (!pitcher) return 0.5;
        double velocity = pitcher->getAttributes().getPitchVelocity();
        double control  = pitcher->getAttributes().getPitchControl();
        // Arbitrary formula: average them
        return (velocity + control) / 198.0; // max combined = 198
    }

    double Simulator::getRunnerSpeed(const Player* runner)
    {
        if (!runner) return 0.5;
        double speed = runner->getAttributes().getSpeed();
        return speed / 99.0;
    }

    double Simulator::getFielderSkill(const Player* fielder)
    {
        if (!fielder) return 0.5;
        double range   = fielder->getAttributes().getFieldingRange();
        double react   = fielder->getAttributes().getReactionTime();
        double armAcc  = fielder->getAttributes().getArmAccuracy();
        // Example: average them
        return (range + react + armAcc) / (3.0 * 99.0);
    }

    // --------------------------------------
    // 1) At-Bat Simulation
    // --------------------------------------
    AtBatResults Simulator::simulateAtBat(const AtBatInputs& inputs)
    {
        AtBatResults results;
        if (!inputs.batter || !inputs.pitcher)
        {
            // Could throw or handle error
            std::cerr << "[simulateAtBat] Invalid batter or pitcher.\n";
            return results;
        }

        // Simple approach: combine pitcher effectiveness and batter power/contact
        double pitcherFactor = getPitcherEffectiveness(inputs.pitcher);
        double batterPower   = getBatterPowerFactor(inputs.batter);
        double batterContact = getBatterContactFactor(inputs.batter);
        double randomVal     = randomDouble();

        // Example logic:
        //  - A rough weighted approach:
        //    1) Strikeout chance if pitcher's factor is significantly higher than contact
        //    2) HR chance if batterPower + random is high
        //    3) Single/Double/Triple from a range
        //    4) Possibly a walk chance based on some arbitrary value, etc.

        // Let's define some arbitrary "probabilities" 
        // (In reality, you'd want more nuanced calculations.)
        double strikeoutThreshold = 0.20 + (pitcherFactor * 0.10) - (batterContact * 0.05);
        double walkThreshold      = strikeoutThreshold + 0.08; 
        double singleThreshold    = walkThreshold + 0.20;
        double doubleThreshold    = singleThreshold + 0.08;
        double tripleThreshold    = doubleThreshold + 0.03;
        double homeRunThreshold   = tripleThreshold + (batterPower * 0.10); // up to 0.1

        if (randomVal < strikeoutThreshold)
        {
            results.outcome    = AtBatOutcome::Strikeout;
            results.basesGained = 0;
            results.wasOut      = true;
        }
        else if (randomVal < walkThreshold)
        {
            results.outcome    = AtBatOutcome::Walk;
            results.basesGained = 0; // typically no bases for the batter unless forced
        }
        else if (randomVal < singleThreshold)
        {
            results.outcome    = AtBatOutcome::Single;
            results.basesGained = 1;
        }
        else if (randomVal < doubleThreshold)
        {
            results.outcome    = AtBatOutcome::Double;
            results.basesGained = 2;
        }
        else if (randomVal < tripleThreshold)
        {
            results.outcome    = AtBatOutcome::Triple;
            results.basesGained = 3;
        }
        else if (randomVal < homeRunThreshold)
        {
            results.outcome    = AtBatOutcome::HomeRun;
            results.basesGained = 4;
            results.runsScored = 1; // batter at least
        }
        else
        {
            // Some kind of out on ball in play (flyout, groundout, etc.)
            results.outcome    = AtBatOutcome::FlyOut; 
            results.basesGained = 0;
            results.wasOut      = true;
        }

        return results;
    }

    // --------------------------------------
    // 2) BaseRunning Simulation
    // --------------------------------------
    BaseRunningResults Simulator::simulateBaseRunning(const BaseRunningInputs& inputs)
    {
        BaseRunningResults results;
        if (!inputs.runner || !inputs.fielder)
        {
            std::cerr << "[simulateBaseRunning] Invalid runner or fielder.\n";
            return results;
        }

        double runnerFactor = getRunnerSpeed(inputs.runner);
        double fielderFactor = getFielderSkill(inputs.fielder);

        // We do a random check to see if the runner is safe or out:
        // The difference between runnerFactor and fielderFactor can weigh the outcome.
        double randomVal = randomDouble();
        double successProbability = runnerFactor - (fielderFactor * 0.5);
        // Just a simplistic formula
        // If successProbability is negative, the fielder is favored.
        // e.g., runnerFactor=0.7, fielderFactor=0.5 => successProb=0.7-0.25=0.45

        // Adjust for the distance: more bases = more time for the fielder to respond
        double distancePenalty = 0.05 * (inputs.basesToAdvance - 1);
        successProbability -= distancePenalty;

        // Clamp it to [0, 1]
        if (successProbability < 0.0) successProbability = 0.0;
        if (successProbability > 1.0) successProbability = 1.0;

        if (randomVal < successProbability)
        {
            // Runner is safe
            results.runnerSafe = true;
            results.basesAdvanced = inputs.basesToAdvance;
            results.runnerOut = false;

            // If the runner advanced to home, that's a run
            if (results.basesAdvanced >= 4)
            {
                results.runScored = true;
            }
        }
        else
        {
            // Runner is out
            results.runnerSafe = false;
            results.runnerOut = true;
            results.basesAdvanced = 0;
        }

        return results;
    }

    // --------------------------------------
    // 3) Fielding Simulation
    // --------------------------------------
    FieldingResults Simulator::simulateFielding(const FieldingInputs& inputs)
    {
        FieldingResults results;
        if (!inputs.fielder)
        {
            std::cerr << "[simulateFielding] Invalid fielder.\n";
            return results;
        }

        double skillFactor = getFielderSkill(inputs.fielder);
        double randomVal = randomDouble();

        // Simple logic: if skillFactor + randomVal is high enough => catch
        // Harder hits or fly balls might reduce or change the threshold
        double difficultyMultiplier = 1.0;
        if (inputs.isFlyBall) difficultyMultiplier += 0.1; 
        if (inputs.isHardHit) difficultyMultiplier += 0.2;

        double threshold = 0.5 * difficultyMultiplier; 
        // e.g. normal grounder => threshold=0.5
        // hard fly => threshold=0.5*(1.3)=0.65

        double chanceToCatch = skillFactor; // 0..1
        // If chanceToCatch < threshold => might be an error or missed catch

        if (chanceToCatch + randomVal > threshold)
        {
            // Fielder makes the play
            results.caught = inputs.isFlyBall; 
            results.successfulThrow = true; // say they fielded the grounder or caught the fly
        }
        else
        {
            // Some chance of error
            double errorChance = 0.05 * (1.0 - skillFactor); 
            if (randomDouble() < errorChance)
            {
                results.error = true;
            }
        }

        return results;
    }

} // namespace BBEngine
