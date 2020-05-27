#include "modulestable.hh"

#include <spdlog/spdlog.h>

#include "../../mainwindow.hh"

namespace StillSane::Ui::Models {

/*!
 * \brief Constructs a ModulesTable model that lists the modules and their properties.
 *
 * \param mainWindow Pointer to the MainWindow.
 * \param parent Pointer to the parent object.
 */
ModulesTable::ModulesTable(MainWindow* mainWindow, QObject* parent)
    : QAbstractTableModel(parent), mMainWindow(mainWindow) {}

auto ModulesTable::headerData(int section, Qt::Orientation orientation, int role) const
    -> QVariant {
  if (role != Qt::DisplayRole)
    return {};

  if (orientation == Qt::Horizontal) {
    QStringList columnNames;
    columnNames << "Name"
                << "Load"
                << "Version"
                << "Description";
    return columnNames.at(section);
  }

  return {};
}

auto ModulesTable::rowCount(const QModelIndex& parent) const -> int {
  if (parent.isValid()) {
    return 0;
  }

  return 0;
}

auto ModulesTable::columnCount(const QModelIndex& parent) const -> int {
  if (parent.isValid()) {
    return 0;
  }
  return 4;
}

auto ModulesTable::data(const QModelIndex& index, int /*role*/) const -> QVariant {
  if (!index.isValid()) {
    return {};
  }

  return {};
}

}  // namespace StillSane::Ui::Models
