#include "luaeditor.hh"
#include "ui_luaeditor.h"

#include <spdlog/spdlog.h>
#include <QFileDialog>
#include <QLuaCompleter>
#include <QLuaHighlighter>
#include <QSyntaxStyle>
#include <utility>

namespace StillSane::Ui {

LuaEditor::LuaEditor(QWidget* parent) : QWidget(parent), ui(new ::Ui::LuaEditor) {
  ui->setupUi(this);

  mCompleter   = new QLuaCompleter(this);
  mHighlighter = new QLuaHighlighter;

  ui->eEditor->setCompleter(mCompleter);
  ui->eEditor->setHighlighter(mHighlighter);
  ui->eEditor->setTabReplaceSize(2);
}

LuaEditor::~LuaEditor() {
  delete mHighlighter;

  for (auto& style : mStyles) {
    delete style.second;
  }

  delete ui;
}

void LuaEditor::clear() {
  ui->eEditor->clear();
}

auto LuaEditor::addSyntaxStyle(QString contents) -> QString {
  auto style = new QSyntaxStyle(this);
  if (!style->load(std::move(contents))) {
    spdlog::error("Failed to load style {}", style->name().toStdString());
    delete style;
    return {};
  }

  mStyles.append({style->name(), style});
  return style->name();
}

auto LuaEditor::setSyntaxStyle(QString name) -> bool {
  auto it = std::find_if(mStyles.begin(), mStyles.end(),
                         [&name](const auto& p) { return p.first == name; });

  if (it != mStyles.end()) {
    ui->eEditor->setSyntaxStyle(it->second);
    mSyntaxStyle = name;
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

void LuaEditor::setName(const QString& name) {
  mName = name;
}

auto LuaEditor::text() -> QString {
  return ui->eEditor->toPlainText();
}

auto LuaEditor::name() -> QString {
  return mName;
}

auto LuaEditor::syntaxStyle() -> QString {
  return mSyntaxStyle;
}

void LuaEditor::on_bLoad_clicked() {
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open lua code"), mDefaultPath,
                                                  tr("Lua files (*.lua)"));

  if (fileName.isEmpty()) {
    return;
}

  QFile lua(fileName);
  if (lua.open(QIODevice::ReadOnly)) {
    ui->eEditor->setPlainText(lua.readAll());
  } else {
    emit error("Could not open file: " + fileName);
  }
  }

void LuaEditor::on_bSave_clicked() {
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save lua code"), mDefaultPath,
                                                  tr("Lua files (*.lua)"));

  if (fileName.isEmpty()) {
    return;
}

  QFile lua(fileName);
  if (lua.open(QIODevice::WriteOnly)) {
    lua.write(ui->eEditor->toPlainText().toUtf8());
  } else {
    error("Could not write to file: " + fileName);
  }
  }

void LuaEditor::paintEvent(QPaintEvent* event) {
  ui->eEditor->updateStyle();
  QWidget::paintEvent(event);
}

}  // namespace StillSane::Ui
