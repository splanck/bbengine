
#pragma once

#include <algorithm> // for std::clamp
#include <vector>

namespace BBEngine
{
    /**
     * The PlayerAttributes class stores all skill ratings (1–99)
     * for batting, pitching, fielding, and baserunning.
     */
    class PlayerAttributes
    {
    public:
        // ------------------------------
        // Constructor & Initialization
        // ------------------------------
        /**
         * Default constructor sets all ratings to a mid-level (50) or your preferred default.
         */
        PlayerAttributes();

        // ----------------------------------------------------------------
        // Getters & Setters - Hitting Attributes
        // ----------------------------------------------------------------
        int getContact() const;
        void setContact(int value);

        int getPower() const;
        void setPower(int value);

        /**
         * Optional: Plate Discipline
         * If your sim doesn't need it, you can remove or comment out.
         */
        int getPlateDiscipline() const;
        void setPlateDiscipline(int value);

        // ----------------------------------------------------------------
        // Getters & Setters - Pitching Attributes
        // ----------------------------------------------------------------
        int getPitchVelocity() const;
        void setPitchVelocity(int value);

        int getPitchControl() const;
        void setPitchControl(int value);

        int getPitchMovement() const;
        void setPitchMovement(int value);

        /**
         * Optional: Stamina for pitchers (how long they can pitch effectively).
         */
        int getStamina() const;
        void setStamina(int value);

        // ----------------------------------------------------------------
        // Getters & Setters - Fielding Attributes
        // ----------------------------------------------------------------
        int getReactionTime() const;
        void setReactionTime(int value);

        /**
         * ArmAccuracy could also be armStrength if that suits your sim.
         */
        int getArmAccuracy() const;
        void setArmAccuracy(int value);

        int getFieldingRange() const;
        void setFieldingRange(int value);

        // ----------------------------------------------------------------
        // Getters & Setters - Speed / Baserunning
        // ----------------------------------------------------------------
        int getSpeed() const;
        void setSpeed(int value);

        // ----------------------------------------------------------------
        // Optional Additional Ratings
        // ----------------------------------------------------------------
        int getDurability() const;
        void setDurability(int value);

        int getClutch() const;
        void setClutch(int value);

        // ----------------------------------------------------------------
        // Utility / Combined Methods
        // ----------------------------------------------------------------
        /**
         * Ensures all rating fields are clamped to [1..99].
         * Call after a bulk load or any major changes if desired.
         */
        void clampAllAttributes();

        /**
         * A quick average of relevant hitting ratings (just an example).
         */
        double averageHittingRating() const;

        /**
         * A quick average of relevant pitching ratings (just an example).
         */
        double averagePitchingRating() const;

        /**
         * Example method if you want to age the player or develop them,
         * possibly lowering velocity or increasing control, etc.
         */
        void ageOneYear();

    private:
        // Helper to clamp a given value into [1..99].
        int clampValue(int raw);

        // ------------------------------
        // Hitting Attributes
        // ------------------------------
        int contact;         ///< Probability of making solid contact (1–99)
        int power;           ///< Likelihood of hitting extra-base hits or HR
        int plateDiscipline; ///< Affects walk rate / chase tendencies

        // ------------------------------
        // Pitching Attributes
        // ------------------------------
        int pitchVelocity;   ///< Speed of pitches
        int pitchControl;    ///< Accuracy for hitting spots
        int pitchMovement;   ///< Movement/break on pitches
        int stamina;         ///< Optional: how long they can pitch effectively

        // ------------------------------
        // Fielding Attributes
        // ------------------------------
        int reactionTime;    ///< Quickness to respond to batted balls
        int armAccuracy;     ///< Or 'armStrength'
        int fieldingRange;   ///< How much ground the fielder can cover

        // ------------------------------
        // Speed / Baserunning
        // ------------------------------
        int speed;           ///< Overall speed, can apply to baserunning & fielding

        // ------------------------------
        // Optional Additional Ratings
        // ------------------------------
        int durability;      ///< E.g., likelihood to remain healthy
        int clutch;          ///< Bonus in key situations if your sim uses it
    };

} // namespace BBEngine


