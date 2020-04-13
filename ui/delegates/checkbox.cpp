#include "checkbox.hh"

#include <QCheckBox>

#include <QDebug>

namespace AutoTrade::Ui::Delegates {

CheckBox::CheckBox(QObject* parent) : QStyledItemDelegate(parent) {}

QWidget* AutoTrade::Ui::Delegates::CheckBox::createEditor(
    QWidget*                    parent,
    const QStyleOptionViewItem& option,
    const QModelIndex&          index) const {
  qDebug() << "Create editor";
  QCheckBox* editor = new QCheckBox(parent);
  editor->setStyleSheet("margin-left:50%; margin-right:50%;");

  return editor;
}

void AutoTrade::Ui::Delegates::CheckBox::setEditorData(QWidget*           editor,
                                                       const QModelIndex& index) const {
  static_cast<QCheckBox*>(editor)->setChecked(
      index.model()->data(index, Qt::EditRole).toBool());
}

void AutoTrade::Ui::Delegates::CheckBox::setModelData(QWidget*            editor,
                                                      QAbstractItemModel* model,
                                                      const QModelIndex&  index) const {
  model->setData(index, static_cast<QCheckBox*>(editor)->isChecked(), Qt::EditRole);
}

void AutoTrade::Ui::Delegates::CheckBox::updateEditorGeometry(
    QWidget*                    editor,
    const QStyleOptionViewItem& option,
    const QModelIndex&          index) const {
  editor->setGeometry(option.rect);
}

}  // namespace AutoTrade::Ui::Delegates
