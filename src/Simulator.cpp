#include "Simulator.h"
#include <cmath>      // for distance calculations if needed
#include <algorithm>  // std::clamp if you want
#include <iostream>   // debug prints if wanted

namespace BBEngine
{
    Simulator::Simulator()
    {
        // seed with some default (non-deterministic) seed
        std::random_device rd;
        rng.seed(rd());
    }

    void Simulator::setRandomEngine(std::mt19937 newEngine)
    {
        rng = newEngine;
    }

    PitchOutcome Simulator::simulatePitch(const PlayerAttributes& pitcherAttr,
        const PlayerAttributes& batterAttr,
        const StadiumContext& /*stadium*/,
        const PitchContext& pitchCtx)
    {
        // 1. Check if it's an intentional walk scenario
        if (pitchCtx.intentionalWalk)
        {
            // Typically an IBB => outcome is 4 balls in a row
            return PitchOutcome::BALL;
        }

        // 2. Probability approach for strike vs. ball
        double strikeProb = getStrikeProbability(pitcherAttr, batterAttr, pitchCtx);

        // Generate a random number 0..1
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        double roll = dist(rng);

        if (roll > strikeProb)
        {
            // We got a ball
            return PitchOutcome::BALL;
        }
        else
        {
            // It's in the strike zone. Next let's see if the batter swings or not, or if it’s a foul or contact.
            // We'll do a simplistic approach: contactProb = getContactProbability(...).
            double contactProb = getContactProbability(pitcherAttr, batterAttr);

            double roll2 = dist(rng);
            if (roll2 > contactProb)
            {
                // Batter misses or doesn't swing => let's say half the time it's a strike looking, half it's swinging
                double roll3 = dist(rng);
                if (roll3 < 0.5)
                    return PitchOutcome::STRIKE_LOOKING;
                else
                    return PitchOutcome::STRIKE_SWINGING;
            }
            else
            {
                // Batter makes contact. Could be foul or in-play
                // Let's say 30% of contact is a foul, 70% is in-play (arbitrary)
                double roll4 = dist(rng);
                if (roll4 < 0.3)
                    return PitchOutcome::FOUL;
                else
                    return PitchOutcome::BATTED_BALL_IN_PLAY;
            }
        }
    }

    BattedBallOutcome Simulator::computeBattedBallOutcome(const PlayerAttributes& pitcherAttr,
        const PlayerAttributes& batterAttr,
        const StadiumContext& stadium)
    {
        // For demonstration, let's guess a "distance" for the hit
        double distance = estimateHitDistance(pitcherAttr, batterAttr);

        // Compare with stadium fence
        double minFence = std::min({ stadium.fenceDistanceLeft,
                                    stadium.fenceDistanceCenter,
                                    stadium.fenceDistanceRight });
        double maxFence = std::max({ stadium.fenceDistanceLeft,
                                    stadium.fenceDistanceCenter,
                                    stadium.fenceDistanceRight });

        // If distance < minFence => maybe single/double/triple
        // If distance > maxFence => home run
        // We'll do a random approach here for single/double/triple if it's short of the fence
        //  - 0..200 => single
        //  - 200..280 => double
        //  - 280..(fence) => triple
        // > fence => homer

        if (distance >= maxFence)
        {
            return BattedBallOutcome::HOMERUN;
        }
        else
        {
            // pick an outcome by segments
            if (distance < 200.0)
                return BattedBallOutcome::SINGLE;
            else if (distance < 280.0)
                return BattedBallOutcome::DOUBLE_;
            else
                return BattedBallOutcome::TRIPLE;
        }

        // Possibly consider random factor for out vs. hit if you prefer a chance the fielder catches it.
        // For brevity, we skip that here.
    }

    double Simulator::getStrikeProbability(const PlayerAttributes& pitcherAttr,
        const PlayerAttributes& batterAttr,
        const PitchContext& pitchCtx)
    {
        // A naive formula:
        // strikeProb = (pitcherControl + 1/2 pitcherVelocity) / sum_of_possible + maybe minus batterDisc
        // if pitchOut => reduce strike probability
        double ctrl = static_cast<double>(pitcherAttr.getPitchControl());
        double velo = static_cast<double>(pitcherAttr.getPitchVelocity());
        double disc = static_cast<double>(batterAttr.getPlateDiscipline());

        double base = (ctrl + 0.5 * velo) / 150.0; // 150 is an arbitrary scaling factor
        // if the batter is disciplined, reduce strike prob slightly
        base -= (disc / 300.0);

        if (pitchCtx.pitchOut)
        {
            // reduce chance of strike further
            base -= 0.2;
        }

        // clamp between 0..1
        if (base < 0.0) base = 0.0;
        if (base > 1.0) base = 1.0;

        return base;
    }

    double Simulator::getContactProbability(const PlayerAttributes& /*pitcherAttr*/,
        const PlayerAttributes& batterAttr)
    {
        // A naive formula: contact prob is based on batter's contact rating
        double c = static_cast<double>(batterAttr.getContact());
        double base = c / 100.0; // e.g., if contact=75 => 0.75 base
        if (base < 0.0) base = 0.0;
        if (base > 1.0) base = 1.0;
        return base;
    }

    double Simulator::estimateHitDistance(const PlayerAttributes& pitcherAttr,
        const PlayerAttributes& batterAttr)
    {
        // For demonstration, let's do:
        // distance = (batterPower + random(0..20)) - pitcherVelocityFactor
        // The logic is arbitrary, just for an example
        double power = static_cast<double>(batterAttr.getPower());
        double velo = static_cast<double>(pitcherAttr.getPitchVelocity());

        // random offset
        std::uniform_real_distribution<double> dist(0.0, 20.0);
        double randomBoost = dist(rng);

        // If velocity is huge, maybe it helps or hinders... let's do a mild factor
        double distance = power + randomBoost - (velo * 0.1);

        if (distance < 0.0) distance = 0.0; // clamp no negative
        return distance;
    }

} // namespace BBEngine
