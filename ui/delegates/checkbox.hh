#ifndef AUTOTRADE_UI_DELEGATES_CHECKBOX_HH
#define AUTOTRADE_UI_DELEGATES_CHECKBOX_HH

#include <QStyledItemDelegate>

namespace AutoTrade::Ui::Delegates {

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

}  // namespace AutoTrade::Ui::Delegates

#endif  // AUTOTRADE_UI_DELEGATES_CHECKBOX_HH
