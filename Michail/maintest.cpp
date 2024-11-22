#include <QApplication>
#include "compliancedashboard.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Instantiate the ComplianceDashboard
    ComplianceDashboard dashboard;

    // Sample data for testing
    QVector<ComplianceData> sampleData = {
        {"Nitrate", "River Thames", 12.5, "Compliant"},
        {"Chlorine", "Lake District", 1.2, "Near Threshold"},
        {"Arsenic", "River Severn", 25.8, "Non-Compliant"},
    };

    // Load the data into the dashboard
    dashboard.loadData(sampleData);

    // Show the dashboard
    dashboard.show();

    return app.exec();
}
