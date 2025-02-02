#pragma once

#include <iostream>

namespace BBEngine
{
    class PlayerAttributes 
    {
    public:
        // Default constructor: sets all attributes to a default value of 50.
        PlayerAttributes();

        // --- Getters for Pitching Attributes ---
        int getPitchVelocity() const;
        int getPitchControl() const;
        int getPitchMovement() const;
        int getPitchStamina() const;
        int getPitchDeception() const;
        int getPitchMechanics() const;
        int getPitchConsistency() const;
        int getPitchRepertoire() const;
        int getPitchArmStrength() const;
        int getPitchGameManagement() const;

        // --- Getters for Hitting Attributes ---
        int getHitContact() const;
        int getHitPower() const;
        int getBatSpeed() const;
        int getPlateDiscipline() const;
        int getVision() const;
        int getSwingMechanics() const;
        int getSwingTiming() const;
        int getSituationalAwareness() const;
        int getAdaptability() const;
        int getOnBaseSkill() const;

        // --- Getters for Fielding Attributes ---
        int getFieldingRange() const;
        int getReactionTime() const;
        int getArmAccuracy() const;
        int getFieldingAwareness() const;
        int getSpeed() const; // Running speed for both baserunning and defense

        // --- Setters for Pitching Attributes ---
        void setPitchVelocity(int value);
        void setPitchControl(int value);
        void setPitchMovement(int value);
        void setPitchStamina(int value);
        void setPitchDeception(int value);
        void setPitchMechanics(int value);
        void setPitchConsistency(int value);
        void setPitchRepertoire(int value);
        void setPitchArmStrength(int value);
        void setPitchGameManagement(int value);

        // --- Setters for Hitting Attributes ---
        void setHitContact(int value);
        void setHitPower(int value);
        void setBatSpeed(int value);
        void setPlateDiscipline(int value);
        void setVision(int value);
        void setSwingMechanics(int value);
        void setSwingTiming(int value);
        void setSituationalAwareness(int value);
        void setAdaptability(int value);
        void setOnBaseSkill(int value);

        // --- Setters for Fielding Attributes ---
        void setFieldingRange(int value);
        void setReactionTime(int value);
        void setArmAccuracy(int value);
        void setFieldingAwareness(int value);
        void setSpeed(int value);

        // --- Utility Functions ---
        double calculatePitchingRating() const;   // Average of pitching attributes
        double calculateHittingRating() const;      // Average of hitting attributes
        double calculateFieldingRating() const;     // Average of fielding attributes

        // Print all attributes for debugging.
        void printAttributes() const;

    private:
        // --- Pitching Attributes ---
        int pitchVelocity;
        int pitchControl;
        int pitchMovement;
        int pitchStamina;
        int pitchDeception;
        int pitchMechanics;
        int pitchConsistency;
        int pitchRepertoire;
        int pitchArmStrength;
        int pitchGameManagement;

        // --- Hitting Attributes ---
        int hitContact;
        int hitPower;
        int batSpeed;
        int plateDiscipline;
        int vision;
        int swingMechanics;
        int swingTiming;
        int situationalAwareness;
        int adaptability;
        int onBaseSkill;

        // --- Fielding Attributes ---
        int fieldingRange;
        int reactionTime;
        int armAccuracy;
        int fieldingAwareness;
        int speed;  // Represents running speed for both baserunning and defense

        // Helper function to clamp a given value to the range [1, 99].
        int clampAttribute(int value) const;
    };

}
