#include "PlayerAttributes.h"

namespace BBEngine
{
    // -------------------------------------------------------
    // Constructor
    // -------------------------------------------------------
    PlayerAttributes::PlayerAttributes()
    {
        // Initialize all attributes to a default, e.g., 50
        contact = 50;
        power = 50;
        plateDiscipline = 50;

        pitchVelocity = 50;
        pitchControl = 50;
        pitchMovement = 50;
        stamina = 50;

        reactionTime = 50;
        armAccuracy = 50;
        fieldingRange = 50;

        speed = 50;

        durability = 50;
        clutch = 50;
    }

    // -------------------------------------------------------
    // Private Helper: clampValue
    // -------------------------------------------------------
    int PlayerAttributes::clampValue(int raw)
    {
        // Force into the [1..99] range:
        if (raw < 1)  return 1;
        if (raw > 99) return 99;
        return raw;
    }

    // -------------------------------------------------------
    // Hitting Attributes
    // -------------------------------------------------------
    int PlayerAttributes::getContact() const
    {
        return contact;
    }
    void PlayerAttributes::setContact(int value)
    {
        contact = clampValue(value);
    }

    int PlayerAttributes::getPower() const
    {
        return power;
    }
    void PlayerAttributes::setPower(int value)
    {
        power = clampValue(value);
    }

    int PlayerAttributes::getPlateDiscipline() const
    {
        return plateDiscipline;
    }
    void PlayerAttributes::setPlateDiscipline(int value)
    {
        plateDiscipline = clampValue(value);
    }

    // -------------------------------------------------------
    // Pitching Attributes
    // -------------------------------------------------------
    int PlayerAttributes::getPitchVelocity() const
    {
        return pitchVelocity;
    }
    void PlayerAttributes::setPitchVelocity(int value)
    {
        pitchVelocity = clampValue(value);
    }

    int PlayerAttributes::getPitchControl() const
    {
        return pitchControl;
    }
    void PlayerAttributes::setPitchControl(int value)
    {
        pitchControl = clampValue(value);
    }

    int PlayerAttributes::getPitchMovement() const
    {
        return pitchMovement;
    }
    void PlayerAttributes::setPitchMovement(int value)
    {
        pitchMovement = clampValue(value);
    }

    int PlayerAttributes::getStamina() const
    {
        return stamina;
    }
    void PlayerAttributes::setStamina(int value)
    {
        stamina = clampValue(value);
    }

    // -------------------------------------------------------
    // Fielding Attributes
    // -------------------------------------------------------
    int PlayerAttributes::getReactionTime() const
    {
        return reactionTime;
    }
    void PlayerAttributes::setReactionTime(int value)
    {
        reactionTime = clampValue(value);
    }

    int PlayerAttributes::getArmAccuracy() const
    {
        return armAccuracy;
    }
    void PlayerAttributes::setArmAccuracy(int value)
    {
        armAccuracy = clampValue(value);
    }

    int PlayerAttributes::getFieldingRange() const
    {
        return fieldingRange;
    }
    void PlayerAttributes::setFieldingRange(int value)
    {
        fieldingRange = clampValue(value);
    }

    // -------------------------------------------------------
    // Speed / Baserunning
    // -------------------------------------------------------
    int PlayerAttributes::getSpeed() const
    {
        return speed;
    }
    void PlayerAttributes::setSpeed(int value)
    {
        speed = clampValue(value);
    }

    // -------------------------------------------------------
    // Optional Additional Ratings
    // -------------------------------------------------------
    int PlayerAttributes::getDurability() const
    {
        return durability;
    }
    void PlayerAttributes::setDurability(int value)
    {
        durability = clampValue(value);
    }

    int PlayerAttributes::getClutch() const
    {
        return clutch;
    }
    void PlayerAttributes::setClutch(int value)
    {
        clutch = clampValue(value);
    }

    // -------------------------------------------------------
    // Utility Methods
    // -------------------------------------------------------
    void PlayerAttributes::clampAllAttributes()
    {
        contact = clampValue(contact);
        power = clampValue(power);
        plateDiscipline = clampValue(plateDiscipline);

        pitchVelocity = clampValue(pitchVelocity);
        pitchControl = clampValue(pitchControl);
        pitchMovement = clampValue(pitchMovement);
        stamina = clampValue(stamina);

        reactionTime = clampValue(reactionTime);
        armAccuracy = clampValue(armAccuracy);
        fieldingRange = clampValue(fieldingRange);

        speed = clampValue(speed);

        durability = clampValue(durability);
        clutch = clampValue(clutch);
    }

    double PlayerAttributes::averageHittingRating() const
    {
        // Example: combine contact, power, plateDiscipline
        // If plateDiscipline is optional, you can exclude or handle differently.
        // We assume all 3 are equally weighted.
        double sum = static_cast<double>(contact + power + plateDiscipline);
        return sum / 3.0;
    }

    double PlayerAttributes::averagePitchingRating() const
    {
        // Example: combine pitchVelocity, pitchControl, pitchMovement
        double sum = static_cast<double>(pitchVelocity + pitchControl + pitchMovement);
        return sum / 3.0;
    }

    void PlayerAttributes::ageOneYear()
    {
        // Example aging logic: reduce pitchVelocity slightly, maybe increase control, etc.
        // You can adapt this as needed or handle externally.
        pitchVelocity = clampValue(pitchVelocity - 1);
        pitchControl = clampValue(pitchControl + 1);

        // You might also reduce durability, etc.
        durability = clampValue(durability - 1);
    }

} // namespace BBEngine
