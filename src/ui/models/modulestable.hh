#pragma once

#include <QAbstractTableModel>

namespace StillSane {

class MainWindow;

namespace Ui::Models {

class ModulesTable : public QAbstractTableModel {
  Q_OBJECT

 public:
  explicit ModulesTable(MainWindow* mainWindow, QObject* parent = nullptr);

  // Header:
  QVariant headerData(int             section,
                      Qt::Orientation orientation,
                      int             role = Qt::DisplayRole) const override;

  // Basic functionality:
  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

 private:
  MainWindow* mMainWindow;
};

}  // namespace Ui::Models

}  // namespace StillSane
