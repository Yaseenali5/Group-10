#include "ComplianceWindow.hpp"

//constructor
ComplianceWindow::ComplianceWindow(QWidget *parent) : QWidget(parent) {
    setWindowTitle("Compliance Dashboard");
    mainlayout = new QVBoxLayout(this);
    createToolbar();
    createMainWidget();
}

// toolbar for the filters 
void ComplianceWindow::createToolbar() {
    QToolBar *toolbar = new QToolBar();

    pollutantFilterBox = new QComboBox(this);
    complianceFilterBox = new QComboBox(this);

    complianceFilterBox->addItems({"All", "Compliant", "Non-Compliant"});

    connect(pollutantFilterBox, &QComboBox::currentTextChanged, this, &ComplianceWindow::filterPollutant);
    connect(complianceFilterBox, &QComboBox::currentTextChanged, this, &ComplianceWindow::filterCompliance);

    toolbar->addWidget(new QLabel(tr("Pollutant: ")));
    toolbar->addWidget(pollutantFilterBox);
    toolbar->addWidget(new QLabel(tr("Compliance: ")));
    toolbar->addWidget(complianceFilterBox);

    mainlayout->addWidget(toolbar);

}

// main widget for the summary cards, -------------------------------------------------------------------------------
void ComplianceWindow::createMainWidget() {
    //mainWidget = new QWidget(this);
    //mainlayout = new QVBoxLayout(mainWidget);

    // qscrollarea allows user to scroll through the cards as the data inputed is large
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);

    summaryWidget = new QWidget(this);
    summaryWidget->setLayout(new QVBoxLayout());
    scrollArea->setWidget(summaryWidget);

    mainlayout->addWidget(scrollArea);

    //mainWidget->setLayout(layout);
}

// Original data = proxy?
// creating the summary cards, that display the pollutant and compliance and non compliance counts
void ComplianceWindow::createSummaryCards(const QString &selectedPollutant, const QString &selectedCompliance) {
    // clears previous sumary cards to update filters
    if (summaryCardsLayout) {
        QLayoutItem *child;
        while ((child = summaryCardsLayout->takeAt(0)) != nullptr) {
            if (child->widget()) {
                child->widget()->deleteLater();
            }
            delete child;
        }
        delete summaryCardsLayout;
        summaryCardsLayout = nullptr;
    }

    summaryCardsLayout = new QGridLayout();

    if (!proxy) {
        QMessageBox::warning(this, tr("No Data"), tr("Please load data before generating summary cards."));
        return;
    }

    QMap<QString, QMap<QString, int>> pollutantSummary;

    for (int row = 0; row < proxy->rowCount(); ++row) {
        QString pollutant = proxy->data(proxy->index(row, 5), Qt::DisplayRole).toString();
        QString compliance = proxy->data(proxy->index(row, 13), Qt::DisplayRole).toString().toLower();

        if (pollutant.isEmpty() || compliance.isEmpty()) {
            continue;
        }

        // implementing the filters for the summary cards
        if (!selectedPollutant.isEmpty() && selectedPollutant != "All" && pollutant.compare(selectedPollutant, Qt::CaseInsensitive) != 0)
            continue;

        if (selectedCompliance == "Compliant" && compliance != "true")
            continue;

        if (selectedCompliance == "Non-Compliant" && compliance != "false")
            continue;

        if (compliance == "true") {
            pollutantSummary[pollutant]["Compliant"]++;
        } else if (compliance == "false") {
            pollutantSummary[pollutant]["Non-Compliant"]++;
        }
    }

    // this populates the summary cards with the data from the csv file
    int row = 0;
    for (auto it = pollutantSummary.begin(); it != pollutantSummary.end(); ++it) {
        QString pollutant = it.key();
        int compliantCount = it.value().value("Compliant", 0);
        int nonCompliantCount = it.value().value("Non-Compliant", 0);

        QLabel *pollutantLabel = new QLabel(QString("<b>%1</b>").arg(pollutant));
        QLabel *compliantLabel = new QLabel(QString("✅ Compliant: %1").arg(compliantCount));
        QLabel *nonCompliantLabel = new QLabel(QString("❌ Non-Compliant: %1").arg(nonCompliantCount));

        pollutantLabel->setStyleSheet("color: black");

        //this highlights and filters the compliance section to either non compliant or compliant
        if (selectedCompliance == "Compliant") {
            compliantLabel->setStyleSheet("color: green; font-weight: bold;");
            nonCompliantLabel->setStyleSheet("color: gray;");
        } else if (selectedCompliance == "Non-Compliant") {
            nonCompliantLabel->setStyleSheet("color: red; font-weight: bold;");
            compliantLabel->setStyleSheet("color: gray;");
        } else {
            compliantLabel->setStyleSheet("color: green;");
            nonCompliantLabel->setStyleSheet("color: red;");
        }

        summaryCardsLayout->addWidget(pollutantLabel, row, 0);
        summaryCardsLayout->addWidget(compliantLabel, row, 1);
        summaryCardsLayout->addWidget(nonCompliantLabel, row, 2);

        row++;
    }

    
    delete summaryWidget->layout();
    summaryWidget->setLayout(summaryCardsLayout);

    summaryWidget->update();
}

void ComplianceWindow::populateFilters() {
    pollutantFilterBox->clear();

    QSet<QString> pollutants;

    // Use the proxy instead of the table model
    if (!proxy) {
        qWarning() << "Proxy model not set!";
        return;
    }

    int totalRows = proxy->rowCount(); // Get row count from the proxy

    for (int row = 0; row < totalRows; ++row) {
        // Access data through the proxy
        QModelIndex index = proxy->index(row, 5); // Assuming column 5 contains pollutant data
        QString pollutant = proxy->data(index, Qt::DisplayRole).toString().trimmed();

        if (!pollutant.isEmpty()) pollutants.insert(pollutant);
    }

    pollutantFilterBox->addItem("All");
    pollutantFilterBox->addItems(pollutants.values());
}


// filter functions for compliance, location and pollutant
void ComplianceWindow::filterCompliance(const QString &complianceStatus) {
    QString pollutant = pollutantFilterBox->currentText();
    createSummaryCards( pollutant, complianceStatus);
}

void ComplianceWindow::filterPollutant(const QString &pollutant) {
    QString compliance = complianceFilterBox->currentText();
    createSummaryCards( pollutant, compliance);
}

void ComplianceWindow::setProxy(filterProxy *proxyModel) {
    proxy = proxyModel;
    //populating filters after loading the file
    populateFilters();

    // generating summary cards after loading in the file
    createSummaryCards("All", "All");
}

void ComplianceWindow::updateWindow() {
    QString pollutant = pollutantFilterBox->currentText();
    QString compliance = complianceFilterBox->currentText();
    populateFilters();
    createSummaryCards( pollutant, compliance);
}

