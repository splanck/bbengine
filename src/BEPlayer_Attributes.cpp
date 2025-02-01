#include "..\include\BEPlayer_Attributes.h"

// Helper function: clamps a given value to the range 1 to 99.
int BEPlayer_Attributes::clampAttribute(int value) const 
{
    if (value < 1) return 1;
    if (value > 99) return 99;
    
    return value;
}

// Default constructor: initializes all attributes to 50.
BEPlayer_Attributes::BEPlayer_Attributes() 
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
int BEPlayer_Attributes::getPitchVelocity() const { return pitchVelocity; }
int BEPlayer_Attributes::getPitchControl() const { return pitchControl; }
int BEPlayer_Attributes::getPitchMovement() const { return pitchMovement; }
int BEPlayer_Attributes::getPitchStamina() const { return pitchStamina; }
int BEPlayer_Attributes::getPitchDeception() const { return pitchDeception; }
int BEPlayer_Attributes::getPitchMechanics() const { return pitchMechanics; }
int BEPlayer_Attributes::getPitchConsistency() const { return pitchConsistency; }
int BEPlayer_Attributes::getPitchRepertoire() const { return pitchRepertoire; }
int BEPlayer_Attributes::getPitchArmStrength() const { return pitchArmStrength; }
int BEPlayer_Attributes::getPitchGameManagement() const { return pitchGameManagement; }

// --- Getters for Hitting Attributes ---
int BEPlayer_Attributes::getHitContact() const { return hitContact; }
int BEPlayer_Attributes::getHitPower() const { return hitPower; }
int BEPlayer_Attributes::getBatSpeed() const { return batSpeed; }
int BEPlayer_Attributes::getPlateDiscipline() const { return plateDiscipline; }
int BEPlayer_Attributes::getVision() const { return vision; }
int BEPlayer_Attributes::getSwingMechanics() const { return swingMechanics; }
int BEPlayer_Attributes::getSwingTiming() const { return swingTiming; }
int BEPlayer_Attributes::getSituationalAwareness() const { return situationalAwareness; }
int BEPlayer_Attributes::getAdaptability() const { return adaptability; }
int BEPlayer_Attributes::getOnBaseSkill() const { return onBaseSkill; }

// --- Getters for Fielding Attributes ---
int BEPlayer_Attributes::getFieldingRange() const { return fieldingRange; }
int BEPlayer_Attributes::getReactionTime() const { return reactionTime; }
int BEPlayer_Attributes::getArmAccuracy() const { return armAccuracy; }
int BEPlayer_Attributes::getFieldingAwareness() const { return fieldingAwareness; }
int BEPlayer_Attributes::getSpeed() const { return speed; }

// --- Setters for Pitching Attributes ---
void BEPlayer_Attributes::setPitchVelocity(int value) { pitchVelocity = clampAttribute(value); }
void BEPlayer_Attributes::setPitchControl(int value) { pitchControl = clampAttribute(value); }
void BEPlayer_Attributes::setPitchMovement(int value) { pitchMovement = clampAttribute(value); }
void BEPlayer_Attributes::setPitchStamina(int value) { pitchStamina = clampAttribute(value); }
void BEPlayer_Attributes::setPitchDeception(int value) { pitchDeception = clampAttribute(value); }
void BEPlayer_Attributes::setPitchMechanics(int value) { pitchMechanics = clampAttribute(value); }
void BEPlayer_Attributes::setPitchConsistency(int value) { pitchConsistency = clampAttribute(value); }
void BEPlayer_Attributes::setPitchRepertoire(int value) { pitchRepertoire = clampAttribute(value); }
void BEPlayer_Attributes::setPitchArmStrength(int value) { pitchArmStrength = clampAttribute(value); }
void BEPlayer_Attributes::setPitchGameManagement(int value) { pitchGameManagement = clampAttribute(value); }

// --- Setters for Hitting Attributes ---
void BEPlayer_Attributes::setHitContact(int value) { hitContact = clampAttribute(value); }
void BEPlayer_Attributes::setHitPower(int value) { hitPower = clampAttribute(value); }
void BEPlayer_Attributes::setBatSpeed(int value) { batSpeed = clampAttribute(value); }
void BEPlayer_Attributes::setPlateDiscipline(int value) { plateDiscipline = clampAttribute(value); }
void BEPlayer_Attributes::setVision(int value) { vision = clampAttribute(value); }
void BEPlayer_Attributes::setSwingMechanics(int value) { swingMechanics = clampAttribute(value); }
void BEPlayer_Attributes::setSwingTiming(int value) { swingTiming = clampAttribute(value); }
void BEPlayer_Attributes::setSituationalAwareness(int value) { situationalAwareness = clampAttribute(value); }
void BEPlayer_Attributes::setAdaptability(int value) { adaptability = clampAttribute(value); }
void BEPlayer_Attributes::setOnBaseSkill(int value) { onBaseSkill = clampAttribute(value); }

// --- Setters for Fielding Attributes ---
void BEPlayer_Attributes::setFieldingRange(int value) { fieldingRange = clampAttribute(value); }
void BEPlayer_Attributes::setReactionTime(int value) { reactionTime = clampAttribute(value); }
void BEPlayer_Attributes::setArmAccuracy(int value) { armAccuracy = clampAttribute(value); }
void BEPlayer_Attributes::setFieldingAwareness(int value) { fieldingAwareness = clampAttribute(value); }
void BEPlayer_Attributes::setSpeed(int value) { speed = clampAttribute(value); }

// --- Utility Functions ---

double BEPlayer_Attributes::calculatePitchingRating() const 
{
    int total = pitchVelocity + pitchControl + pitchMovement + pitchStamina + pitchDeception + 
        pitchMechanics + pitchConsistency + pitchRepertoire + pitchArmStrength + pitchGameManagement;

    return total / 10.0;
}

double BEPlayer_Attributes::calculateHittingRating() const 
{
    int total = hitContact + hitPower + batSpeed + plateDiscipline + vision + swingMechanics + 
        swingTiming + situationalAwareness + adaptability + onBaseSkill;
    
    return total / 10.0;
}

double BEPlayer_Attributes::calculateFieldingRating() const 
{
    int total = fieldingRange + reactionTime + armAccuracy + fieldingAwareness + speed;

    return total / 5.0;
}

void BEPlayer_Attributes::printAttributes() const {
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
