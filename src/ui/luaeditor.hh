#ifndef LUAEDITOR_HH
#define LUAEDITOR_HH

#include <QWidget>

namespace Ui {
class LuaEditor;
}

class QLuaCompleter;
class QLuaHighlighter;
class QSyntaxStyle;

namespace AutoTrade::Ui {

class LuaEditor : public QWidget {
  Q_OBJECT

 public:
  explicit LuaEditor(QWidget* parent = nullptr);
  ~LuaEditor();

  void clear();

  QString addSyntaxStyle(QString contents);
  bool    setSyntaxStyle(QString name);
  void    setFont(const QFont& font);
  void    setText(const QString& text);
  void    setDefaultPath(const QString& defaultPath);
  void    setName(const QString& name);

  QString text();
  QString name();
  QString syntaxStyle();

 signals:
  void error(QString msg);

 private slots:
  void on_bLoad_clicked();
  void on_bSave_clicked();

  void paintEvent(QPaintEvent* event) override;

 private:
  ::Ui::LuaEditor* ui;

  QLuaCompleter*                         mCompleter;
  QLuaHighlighter*                       mHighlighter;
  QVector<QPair<QString, QSyntaxStyle*>> mStyles;
  QString                                mDefaultPath;

  QString mName, mSyntaxStyle;
};

}  // namespace AutoTrade::Ui

#endif  // LUAEDITOR_HH
