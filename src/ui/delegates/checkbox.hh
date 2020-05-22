#pragma once

#include <QStyledItemDelegate>

namespace StillSane::Ui::Delegates {

class CheckBox : public QStyledItemDelegate {
  Q_OBJECT
 public:
  CheckBox(QObject* parent = nullptr);

  // QAbstractItemDelegate interface
 public:
  QWidget* createEditor(QWidget*                    parent,
                        const QStyleOptionViewItem& option,
                        const QModelIndex&          index) const override;

  void setEditorData(QWidget* editor, const QModelIndex& index) const override;
  void setModelData(QWidget*            editor,
                    QAbstractItemModel* model,
                    const QModelIndex&  index) const override;

  void updateEditorGeometry(QWidget*                    editor,
                            const QStyleOptionViewItem& option,
                            const QModelIndex&          index) const override;
};

}  // namespace StillSane::Ui::Delegates
