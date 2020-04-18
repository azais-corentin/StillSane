#ifndef AUTOTRADE_UI_LUAHIGHLIGHTER_HH
#define AUTOTRADE_UI_LUAHIGHLIGHTER_HH

#include <QRegularExpression>
#include <QSyntaxHighlighter>

namespace AutoTrade::Ui {

class LuaHighlighter : public QSyntaxHighlighter {
  Q_OBJECT
 public:
  LuaHighlighter(QTextDocument* parent = nullptr);

 protected:
  void highlightBlock(const QString& text) override;

 private:
  struct HighlightingRule {
    QRegularExpression pattern;
    QTextCharFormat    format;
  };
  QVector<HighlightingRule> mHighlightingRules;

  QTextCharFormat mKeywordFormat;
  QTextCharFormat mOperatorFormat;
  QTextCharFormat mQuotationFormat;
  QTextCharFormat mFunctionFormat;
};

}  // namespace AutoTrade::Ui

#endif  // AUTOTRADE_UI_LUAHIGHLIGHTER_HH
