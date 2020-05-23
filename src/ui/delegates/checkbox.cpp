#include "checkbox.hh"

#include <QCheckBox>

#include <QDebug>

namespace StillSane::Ui::Delegates {

CheckBox::CheckBox(QObject* parent) : QStyledItemDelegate(parent) {}

QWidget* CheckBox::createEditor(QWidget* parent,
                                const QStyleOptionViewItem& /*option*/,
                                const QModelIndex& /*index*/) const {
  qDebug() << "Create editor";
  QCheckBox* editor = new QCheckBox(parent);
  editor->setStyleSheet("margin-left:50%; margin-right:50%;");

  return editor;
}

void CheckBox::setEditorData(QWidget* editor, const QModelIndex& index) const {
  static_cast<QCheckBox*>(editor)->setChecked(
      index.model()->data(index, Qt::EditRole).toBool());
}

void CheckBox::setModelData(QWidget*            editor,
                            QAbstractItemModel* model,
                            const QModelIndex&  index) const {
  model->setData(index, static_cast<QCheckBox*>(editor)->isChecked(), Qt::EditRole);
}

void CheckBox::updateEditorGeometry(QWidget*                    editor,
                                    const QStyleOptionViewItem& option,
                                    const QModelIndex& /*index*/) const {
  editor->setGeometry(option.rect);
}

}  // namespace StillSane::Ui::Delegates
