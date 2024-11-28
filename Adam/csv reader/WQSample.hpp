#pragma once

#include <string>
#include <iostream>

class WQSample
{
public:
    WQSample(const std::string& id, const std::string& samplingPoint, const std::string& notation, const std::string& label,
             const std::string& sampleDateTime, const std::string& determinandLabel, const std::string& determinandDefinition,
             const std::string& determinandNotation, const std::string& resultQualifier, double result, const std::string& interpretation,
             const std::string& unitLabel, const std::string& materialType, bool isCompliance, const std::string& purpose,
             double easting, double northing);

    std::string getId() const { return id; }
    std::string getSamplingPoint() const { return samplingPoint; }
    std::string getNotation() const { return notation; }
    std::string getLabel() const { return label; }
    std::string getSampleDateTime() const { return sampleDateTime; }
    std::string getDeterminandLabel() const { return determinandLabel; }
    std::string getDeterminandDefinition() const { return determinandDefinition; }
    std::string getDeterminandNotation() const { return determinandNotation; }
    std::string getResultQualifier() const { return resultQualifier; }
    double getResult() const { return result; }
    std::string getInterpretation() const { return interpretation; }
    std::string getUnitLabel() const { return unitLabel; }
    std::string getMaterialType() const { return materialType; }
    bool getIsComplianceSample() const { return isCompliance; }
    std::string getPurpose() const { return purpose; }
    double getEasting() const { return easting; }
    double getNorthing() const { return northing; }

private:
    std::string id;
    std::string samplingPoint;
    std::string notation;
    std::string label;
    std::string sampleDateTime;
    std::string determinandLabel;
    std::string determinandDefinition;
    std::string determinandNotation;
    std::string resultQualifier;
    double result;
    std::string interpretation;
    std::string unitLabel;
    std::string materialType;
    bool isCompliance;
    std::string purpose;
    double easting;
    double northing;
};

std::ostream& operator<<(std::ostream& os, const WQSample& sample);