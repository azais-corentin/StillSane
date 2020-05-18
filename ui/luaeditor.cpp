#include "luaeditor.hh"
#include "ui_luaeditor.h"

#include <spdlog/spdlog.h>
#include <QFileDialog>
#include <QLuaCompleter>
#include <QLuaHighlighter>
#include <QSyntaxStyle>

namespace AutoTrade::Ui {

LuaEditor::LuaEditor(QWidget* parent) : QWidget(parent), ui(new ::Ui::LuaEditor) {
  ui->setupUi(this);

  mCompleter   = new QLuaCompleter(this);
  mHighlighter = new QLuaHighlighter;

  ui->eEditor->setCompleter(mCompleter);
  ui->eEditor->setHighlighter(mHighlighter);
  ui->eEditor->setTabReplaceSize(2);
}

LuaEditor::~LuaEditor() {
  delete ui;

  delete mHighlighter;

  for (auto& style : mStyles) {
    delete style.second;
  }
}

void LuaEditor::clear() {
  ui->eEditor->clear();
}

QString LuaEditor::addSyntaxStyle(QString contents) {
  auto style = new QSyntaxStyle(this);
  if (!style->load(contents)) {
    spdlog::error("Failed to load style {}", style->name().toStdString());
    delete style;
    return {};
  }

  mStyles.append({style->name(), style});
  return style->name();
}

bool LuaEditor::setSyntaxStyle(QString name) {
  auto it = std::find_if(mStyles.begin(), mStyles.end(),
                         [&name](const auto& p) { return p.first == name; });

  if (it != mStyles.end()) {
    ui->eEditor->setSyntaxStyle(it->second);
    return true;
  }
  return false;
}

void LuaEditor::setFont(const QFont& font) {
  ui->eEditor->setFont(font);
}

void LuaEditor::setText(const QString& text) {
  ui->eEditor->setPlainText(text);
}

void LuaEditor::setDefaultPath(const QString& defaultPath) {
  mDefaultPath = defaultPath;
}

QString LuaEditor::text() {
  return ui->eEditor->toPlainText();
}

}  // namespace AutoTrade::Ui

void AutoTrade::Ui::LuaEditor::on_bLoad_clicked() {
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open lua code"), mDefaultPath,
                                                  tr("Lua files (*.lua)"));

  if (fileName.isEmpty())
    return;

  QFile lua(fileName);
  if (lua.open(QIODevice::ReadOnly)) {
    ui->eEditor->setPlainText(lua.readAll());
  } else {
    emit error("Could not open file: " + fileName);
  }
}

void AutoTrade::Ui::LuaEditor::on_bSave_clicked() {
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save lua code"), mDefaultPath,
                                                  tr("Lua files (*.lua)"));

  if (fileName.isEmpty())
    return;

  QFile lua(fileName);
  if (lua.open(QIODevice::WriteOnly)) {
    lua.write(ui->eEditor->toPlainText().toUtf8());
  } else {
    error("Could not write to file: " + fileName);
  }
}
