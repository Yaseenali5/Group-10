#include "WQSample.hpp"

// Constructor Implementation
WQSample::WQSample(const std::string& id, const std::string& samplingPoint, const std::string& notation, const std::string& label,
                   const std::string& sampleDateTime, const std::string& determinandLabel, const std::string& determinandDefinition,
                   const std::string& determinandNotation, const std::string& resultQualifier, double result, const std::string& interpretation,
                   const std::string& unitLabel, const std::string& materialType, bool isCompliance, const std::string& purpose,
                   double easting, double northing)
    : id(id), samplingPoint(samplingPoint), notation(notation), label(label), sampleDateTime(sampleDateTime),
      determinandLabel(determinandLabel), determinandDefinition(determinandDefinition), determinandNotation(determinandNotation),
      resultQualifier(resultQualifier), result(result), interpretation(interpretation), unitLabel(unitLabel),
      materialType(materialType), isCompliance(isCompliance), purpose(purpose), easting(easting), northing(northing)
{
    // Basic Validation
    if (result < 0.0) {
        throw std::invalid_argument("Result cannot be negative.");
    }
    if (easting < -180 || easting > 180 || northing < -90 || northing > 90) {
        throw std::out_of_range("Invalid coordinates.");
    }
}

// Overloaded << Operator Implementation
std::ostream& operator<<(std::ostream& os, const WQSample& sample) {
    os << "ID: " << sample.id << "\n"
       << "Sampling Point: " << sample.samplingPoint << "\n"
       << "Notation: " << sample.notation << "\n"
       << "Label: " << sample.label << "\n"
       << "Sample Date/Time: " << sample.sampleDateTime << "\n"
       << "Determinand Label: " << sample.determinandLabel << "\n"
       << "Determinand Definition: " << sample.determinandDefinition << "\n"
       << "Result: " << sample.result << " " << sample.unitLabel << "\n"
       << "Interpretation: " << sample.interpretation << "\n"
       << "Material Type: " << sample.materialType << "\n"
       << "Compliance Sample: " << (sample.isCompliance ? "Yes" : "No") << "\n"
       << "Purpose: " << sample.purpose << "\n"
       << "Coordinates: (" << sample.easting << ", " << sample.northing << ")";
    return os;
}