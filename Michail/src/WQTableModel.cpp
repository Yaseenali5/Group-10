#include "WQTableModel.hpp"

void WQTableModel::updateFromFile(const QString& filename)
{
  beginResetModel();
  dataset.loadData(filename.toStdString());
  endResetModel();
}

QVariant WQTableModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid()) {
    return QVariant();
  }

  if (role == Qt::TextAlignmentRole) {
    return int(Qt::AlignRight | Qt::AlignVCenter);
  }
  else if (role == Qt::DisplayRole) {
    WQSample sample = dataset[index.row()];
    switch (index.column()) {
      case 0: return QVariant(QString::fromStdString(sample.getId()));
      case 1: return QVariant(QString::fromStdString(sample.getSamplingPoint()));
      case 2: return QVariant(QString::fromStdString(sample.getNotation()));
      case 3: return QVariant(QString::fromStdString(sample.getLabel()));
      case 4: return QVariant(QString::fromStdString(sample.getSampleDateTime()));
      case 5: return QVariant(QString::fromStdString(sample.getDeterminandLabel()));
      case 6: return QVariant(QString::fromStdString(sample.getDeterminandDefinition()));
      case 7: return QVariant(QString::fromStdString(sample.getDeterminandNotation()));
      case 8: return QVariant(QString::fromStdString(sample.getResultQualifier()));
      case 9: return QVariant(sample.getResult());
      case 10: return QVariant(QString::fromStdString(sample.getInterpretation()));
      case 11: return QVariant(QString::fromStdString(sample.getUnitLabel()));
      case 12: return QVariant(QString::fromStdString(sample.getMaterialType()));
      case 13: return QVariant(QString::fromStdString(sample.getIsComplianceSample()));
      case 14: return QVariant(QString::fromStdString(sample.getPurpose()));
      case 15: return QVariant(sample.getEasting());
      case 16: return QVariant(sample.getNorthing());
      default: return QVariant();
    }
  }

  return QVariant();
}

QVariant WQTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (role != Qt::DisplayRole) {
    return QVariant();
  }

  if (orientation == Qt::Vertical) {
    return QVariant(section + 1);
  }

  switch (section) {
    case 0: return QString("@id");
    case 1: return QString("Sampling Point");
    case 2: return QString("Notation");
    case 3: return QString("Label");
    case 4: return QString("DateTime");
    case 5: return QString("Determinand Label");
    case 6: return QString("Determinand Definitiom");
    case 7: return QString("Determinand Notation");
    case 8: return QString("Result Qualifier");
    case 9: return QString("Result");
    case 10: return QString("Interpretation");
    case 11: return QString("Unit Label");
    case 12: return QString("Material Type");
    case 13: return QString("isComplianceSample");
    case 14: return QString("Purpose");
    case 15: return QString("Easting");
    case 16: return QString("Northing");
    default: return QVariant();
  }
}