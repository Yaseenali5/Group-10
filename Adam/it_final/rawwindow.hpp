#include <QtWidgets>
#include <QSortFilterProxyModel>
#include "WQDataset.hpp"
#include "WQTableModel.hpp"

class RawWindow : public QMainWindow
{    
    Q_OBJECT

public:
    RawWindow();

private:
    void createMainWidget();
    void createFileSelectors();
    void createButtons();
    void createToolbar();
    void createStatusBar();
    void addFileMenu();

    QAction *openAction;
    QAction *closeAction;
    QMenu *fileMenu;
    QLabel *fileInfo;
    QLabel *dataInfo;
    QString filename;
    QPushButton *loadButton;
    QTableView *table;
    QComboBox *locBox;
    QSortFilterProxyModel* proxy;
    
    WQTableModel tablemodel;  
  
  private slots:
    void setDataLoc();
    void loadCSV();
    void filterLoc(const QString& location);
};
