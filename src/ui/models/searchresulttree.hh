#pragma once

#include <QAbstractItemModel>

namespace StillSane::Ui::Models {

/*!
 * \brief Tree model for trade search results
 */
class SearchResultTree : public QAbstractItemModel {
  Q_OBJECT

 public:
  explicit SearchResultTree(QObject* parent = nullptr);

  // Header:
  QVariant headerData(int             section,
                      Qt::Orientation orientation,
                      int             role = Qt::DisplayRole) const override;

  // Basic functionality:
  QModelIndex index(int                row,
                    int                column,
                    const QModelIndex& parent = QModelIndex()) const override;
  QModelIndex parent(const QModelIndex& index) const override;

  int rowCount(const QModelIndex& parent = QModelIndex()) const override;
  int columnCount(const QModelIndex& parent = QModelIndex()) const override;

  QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

 private:
};

}  // namespace StillSane::Ui::Models
