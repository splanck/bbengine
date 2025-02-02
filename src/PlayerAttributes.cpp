#include "PlayerAttributes.h"

namespace BBEngine
{
    // Helper function: clamps a given value to the range 1 to 99.
    int PlayerAttributes::clampAttribute(int value) const 
    {
        if (value < 1) return 1;
        if (value > 99) return 99;
        
        return value;
    }

    // Default constructor: initializes all attributes to 50.
    PlayerAttributes::PlayerAttributes() 
    {
        // Initialize Pitching Attributes
        pitchVelocity = 50;
        pitchControl = 50;
        pitchMovement = 50;
        pitchStamina = 50;
        pitchDeception = 50;
        pitchMechanics = 50;
        pitchConsistency = 50;
        pitchRepertoire = 50;
        pitchArmStrength = 50;
        pitchGameManagement = 50;

        // Initialize Hitting Attributes
        hitContact = 50;
        hitPower = 50;
        batSpeed = 50;
        plateDiscipline = 50;
        vision = 50;
        swingMechanics = 50;
        swingTiming = 50;
        situationalAwareness = 50;
        adaptability = 50;
        onBaseSkill = 50;

        // Initialize Fielding Attributes
        fieldingRange = 50;
        reactionTime = 50;
        armAccuracy = 50;
        fieldingAwareness = 50;
        speed = 50;  // Running speed for both baserunning and defense
    }

    // --- Getters for Pitching Attributes ---
    int PlayerAttributes::getPitchVelocity() const { return pitchVelocity; }
    int PlayerAttributes::getPitchControl() const { return pitchControl; }
    int PlayerAttributes::getPitchMovement() const { return pitchMovement; }
    int PlayerAttributes::getPitchStamina() const { return pitchStamina; }
    int PlayerAttributes::getPitchDeception() const { return pitchDeception; }
    int PlayerAttributes::getPitchMechanics() const { return pitchMechanics; }
    int PlayerAttributes::getPitchConsistency() const { return pitchConsistency; }
    int PlayerAttributes::getPitchRepertoire() const { return pitchRepertoire; }
    int PlayerAttributes::getPitchArmStrength() const { return pitchArmStrength; }
    int PlayerAttributes::getPitchGameManagement() const { return pitchGameManagement; }

    // --- Getters for Hitting Attributes ---
    int PlayerAttributes::getHitContact() const { return hitContact; }
    int PlayerAttributes::getHitPower() const { return hitPower; }
    int PlayerAttributes::getBatSpeed() const { return batSpeed; }
    int PlayerAttributes::getPlateDiscipline() const { return plateDiscipline; }
    int PlayerAttributes::getVision() const { return vision; }
    int PlayerAttributes::getSwingMechanics() const { return swingMechanics; }
    int PlayerAttributes::getSwingTiming() const { return swingTiming; }
    int PlayerAttributes::getSituationalAwareness() const { return situationalAwareness; }
    int PlayerAttributes::getAdaptability() const { return adaptability; }
    int PlayerAttributes::getOnBaseSkill() const { return onBaseSkill; }

    // --- Getters for Fielding Attributes ---
    int PlayerAttributes::getFieldingRange() const { return fieldingRange; }
    int PlayerAttributes::getReactionTime() const { return reactionTime; }
    int PlayerAttributes::getArmAccuracy() const { return armAccuracy; }
    int PlayerAttributes::getFieldingAwareness() const { return fieldingAwareness; }
    int PlayerAttributes::getSpeed() const { return speed; }

    // --- Setters for Pitching Attributes ---
    void PlayerAttributes::setPitchVelocity(int value) { pitchVelocity = clampAttribute(value); }
    void PlayerAttributes::setPitchControl(int value) { pitchControl = clampAttribute(value); }
    void PlayerAttributes::setPitchMovement(int value) { pitchMovement = clampAttribute(value); }
    void PlayerAttributes::setPitchStamina(int value) { pitchStamina = clampAttribute(value); }
    void PlayerAttributes::setPitchDeception(int value) { pitchDeception = clampAttribute(value); }
    void PlayerAttributes::setPitchMechanics(int value) { pitchMechanics = clampAttribute(value); }
    void PlayerAttributes::setPitchConsistency(int value) { pitchConsistency = clampAttribute(value); }
    void PlayerAttributes::setPitchRepertoire(int value) { pitchRepertoire = clampAttribute(value); }
    void PlayerAttributes::setPitchArmStrength(int value) { pitchArmStrength = clampAttribute(value); }
    void PlayerAttributes::setPitchGameManagement(int value) { pitchGameManagement = clampAttribute(value); }

    // --- Setters for Hitting Attributes ---
    void PlayerAttributes::setHitContact(int value) { hitContact = clampAttribute(value); }
    void PlayerAttributes::setHitPower(int value) { hitPower = clampAttribute(value); }
    void PlayerAttributes::setBatSpeed(int value) { batSpeed = clampAttribute(value); }
    void PlayerAttributes::setPlateDiscipline(int value) { plateDiscipline = clampAttribute(value); }
    void PlayerAttributes::setVision(int value) { vision = clampAttribute(value); }
    void PlayerAttributes::setSwingMechanics(int value) { swingMechanics = clampAttribute(value); }
    void PlayerAttributes::setSwingTiming(int value) { swingTiming = clampAttribute(value); }
    void PlayerAttributes::setSituationalAwareness(int value) { situationalAwareness = clampAttribute(value); }
    void PlayerAttributes::setAdaptability(int value) { adaptability = clampAttribute(value); }
    void PlayerAttributes::setOnBaseSkill(int value) { onBaseSkill = clampAttribute(value); }

    // --- Setters for Fielding Attributes ---
    void PlayerAttributes::setFieldingRange(int value) { fieldingRange = clampAttribute(value); }
    void PlayerAttributes::setReactionTime(int value) { reactionTime = clampAttribute(value); }
    void PlayerAttributes::setArmAccuracy(int value) { armAccuracy = clampAttribute(value); }
    void PlayerAttributes::setFieldingAwareness(int value) { fieldingAwareness = clampAttribute(value); }
    void PlayerAttributes::setSpeed(int value) { speed = clampAttribute(value); }

    // --- Utility Functions ---

    double PlayerAttributes::calculatePitchingRating() const 
    {
        int total = pitchVelocity + pitchControl + pitchMovement + pitchStamina + pitchDeception + 
            pitchMechanics + pitchConsistency + pitchRepertoire + pitchArmStrength + pitchGameManagement;

        return total / 10.0;
    }

    double PlayerAttributes::calculateHittingRating() const 
    {
        int total = hitContact + hitPower + batSpeed + plateDiscipline + vision + swingMechanics + 
            swingTiming + situationalAwareness + adaptability + onBaseSkill;
        
        return total / 10.0;
    }

    double PlayerAttributes::calculateFieldingRating() const 
    {
        int total = fieldingRange + reactionTime + armAccuracy + fieldingAwareness + speed;

        return total / 5.0;
    }

    void PlayerAttributes::printAttributes() const {
        std::cout << "Pitching Attributes:\n";
        std::cout << "  Velocity: " << pitchVelocity << "\n";
        std::cout << "  Control: " << pitchControl << "\n";
        std::cout << "  Movement: " << pitchMovement << "\n";
        std::cout << "  Stamina: " << pitchStamina << "\n";
        std::cout << "  Deception: " << pitchDeception << "\n";
        std::cout << "  Mechanics: " << pitchMechanics << "\n";
        std::cout << "  Consistency: " << pitchConsistency << "\n";
        std::cout << "  Repertoire: " << pitchRepertoire << "\n";
        std::cout << "  Arm Strength: " << pitchArmStrength << "\n";
        std::cout << "  Game Management: " << pitchGameManagement << "\n";
        std::cout << "  Composite Pitching Rating: " << calculatePitchingRating() << "\n\n";

        std::cout << "Hitting Attributes:\n";
        std::cout << "  Contact: " << hitContact << "\n";
        std::cout << "  Power: " << hitPower << "\n";
        std::cout << "  Bat Speed: " << batSpeed << "\n";
        std::cout << "  Plate Discipline: " << plateDiscipline << "\n";
        std::cout << "  Vision: " << vision << "\n";
        std::cout << "  Swing Mechanics: " << swingMechanics << "\n";
        std::cout << "  Swing Timing: " << swingTiming << "\n";
        std::cout << "  Situational Awareness: " << situationalAwareness << "\n";
        std::cout << "  Adaptability: " << adaptability << "\n";
        std::cout << "  On Base Skill: " << onBaseSkill << "\n";
        std::cout << "  Composite Hitting Rating: " << calculateHittingRating() << "\n\n";

        std::cout << "Fielding Attributes:\n";
        std::cout << "  Fielding Range: " << fieldingRange << "\n";
        std::cout << "  Reaction Time: " << reactionTime << "\n";
        std::cout << "  Arm Accuracy: " << armAccuracy << "\n";
        std::cout << "  Fielding Awareness: " << fieldingAwareness << "\n";
        std::cout << "  Speed: " << speed << "\n";
        std::cout << "  Composite Fielding Rating: " << calculateFieldingRating() << "\n";
    }

}

