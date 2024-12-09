#pragma once

#include <string>
#include <iostream>

using namespace std;

class WQSample
{
public:
    WQSample(const string&, const string&, const string&, const string&,
             const string&, const string&, const string&, const string&, 
             const string&, double, const string&, const string&, 
             const string&, const string&, const string&, double, double );

    string getId() const { return id; }
    string getSamplingPoint() const { return samplingPoint; }
    string getNotation() const { return notation; }
    string getLabel() const { return label; }
    string getSampleDateTime() const { return sampleDateTime; }
    string getDeterminandLabel() const { return determinandLabel; }
    string getDeterminandDefinition() const { return determinandDefinition; }
    string getDeterminandNotation() const { return determinandNotation; }
    string getResultQualifier() const { return resultQualifier; }
    double getResult() const { return result; }
    string getInterpretation() const { return interpretation; }
    string getUnitLabel() const { return unitLabel; }
    string getMaterialType() const { return materialType; }
    string getIsComplianceSample() const { return isCompliance; }
    string getPurpose() const { return purpose; }
    double getEasting() const { return easting; }
    double getNorthing() const { return northing; }

private:
    string id;
    string samplingPoint;
    string notation;
    string label;
    string sampleDateTime;
    string determinandLabel;
    string determinandDefinition;
    string determinandNotation;
    string resultQualifier;
    double result;
    string interpretation;
    string unitLabel;
    string materialType;
    string isCompliance;
    string purpose;
    double easting;
    double northing;
};

ostream& operator<<(ostream& os, const WQSample& sample);