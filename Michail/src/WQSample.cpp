#include "WQSample.hpp"

using namespace std;


WQSample::WQSample(const string& id, const string& samplingPoint, const string& notation, const string& label,
                   const string& sampleDateTime, const string& determinandLabel, const string& determinandDefinition,
                   const string& determinandNotation, const string& resultQualifier, double result, const string& interpretation,
                   const string& unitLabel, const string& materialType, const string& isCompliance, const string& purpose,
                   double easting, double northing)
    : id(id), samplingPoint(samplingPoint), notation(notation), label(label), sampleDateTime(sampleDateTime),
      determinandLabel(determinandLabel), determinandDefinition(determinandDefinition), determinandNotation(determinandNotation),
      resultQualifier(resultQualifier), result(result), interpretation(interpretation), unitLabel(unitLabel),
      materialType(materialType), isCompliance(isCompliance), purpose(purpose), easting(easting), northing(northing)
{

}

ostream& operator<<(ostream& os, const WQSample& sample) {
    os << "ID: " << sample.getId() << "\n"
       << "Sampling Point: " << sample.getSamplingPoint() << "\n"
       << "Notation: " << sample.getNotation() << "\n"
       << "Label: " << sample.getLabel() << "\n"
       << "Sample Date/Time: " << sample.getSampleDateTime() << "\n"
       << "Determinand Label: " << sample.getDeterminandLabel() << "\n"
       << "Determinand Definition: " << sample.getDeterminandDefinition() << "\n"
       << "Result: " << sample.getResult() << " " << sample.getUnitLabel() << "\n"
       << "Interpretation: " << sample.getInterpretation() << "\n"
       << "Material Type: " << sample.getMaterialType() << "\n"
       << "Compliance Sample: " << (sample.getIsComplianceSample()) << "\n"
       << "Purpose: " << sample.getPurpose() << "\n"
       << "Coordinates: (" << sample.getEasting() << ", " << sample.getNorthing() << ")";
    return os;
}