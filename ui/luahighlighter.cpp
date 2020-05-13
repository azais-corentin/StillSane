#include "luahighlighter.hh"

namespace AutoTrade::Ui {

LuaHighlighter::LuaHighlighter(QTextDocument* parent) : QSyntaxHighlighter(parent) {
  HighlightingRule rule;

  mKeywordFormat.setForeground(Qt::darkBlue);
  mKeywordFormat.setFontWeight(QFont::Bold);
  const QString keywordPatterns[] = {
      QStringLiteral("\\bbreak\\b"),     QStringLiteral("\\bdo\\b"),
      QStringLiteral("\\belse\\b"),      QStringLiteral("\\belseif\\b"),
      QStringLiteral("\\bend\\b"),       QStringLiteral("\\bfor\\b"),
      QStringLiteral("\\bfunction\\b"),  QStringLiteral("\\bif\\b"),
      QStringLiteral("\\bin\\b"),        QStringLiteral("\\blocal\\b"),
      QStringLiteral("\\brepeat\\b"),    QStringLiteral("\\breturn\\b"),
      QStringLiteral("\\bthen\\b"),      QStringLiteral("\\buntil\\b"),
      QStringLiteral("\\bwhile\\b"),     QStringLiteral("\\bor\\b"),
      QStringLiteral("\\band\\b"),       QStringLiteral("\\bnot\\b"),
      QStringLiteral("\\btrue\\b"),      QStringLiteral("\\bfalse\\b"),
      QStringLiteral("\\bnil\\b"),       QStringLiteral("\\bstring\\b"),
      QStringLiteral("\\bpackage\\b"),   QStringLiteral("\\bos\\b"),
      QStringLiteral("\\bio\\b"),        QStringLiteral("\\bmath\\b"),
      QStringLiteral("\\bdebug\\b"),     QStringLiteral("\\btable\\b"),
      QStringLiteral("\\bcoroutine\\b"), QStringLiteral("\\bforeach\\b")};

  for (const QString& pattern : keywordPatterns) {
    rule.pattern = QRegularExpression(pattern);
    rule.format  = mKeywordFormat;
    mHighlightingRules.append(rule);
  }

  mOperatorFormat.setFontWeight(QFont::Bold);
  mOperatorFormat.setForeground(Qt::darkMagenta);
  rule.pattern = QRegularExpression(QStringLiteral(
      "\\b\\+|\\-|\\*|\\/|%|\\#|\\^|~|<|>|<=|=>|==|~=|=|\\:|\\.\\.\\.|\\.\\.\\b"));
  rule.format  = mOperatorFormat;
  mHighlightingRules.append(rule);

  mCommentFormat.setForeground(Qt::gray);
  rule.pattern = QRegularExpression(QStringLiteral(R"(--(?>\[{2}\X*?\]{2}|.*))"));
  rule.format  = mCommentFormat;
  mHighlightingRules.append(rule);

  mQuotationFormat.setForeground(Qt::darkGreen);
  rule.pattern =
      QRegularExpression(QStringLiteral(R"(((?<![\\])['"])((?:.(?!(?<![\\])\1))*.?)\1)"));
  rule.format = mQuotationFormat;
  mHighlightingRules.append(rule);

  mFunctionFormat.setFontItalic(true);
  mFunctionFormat.setForeground(Qt::blue);
  rule.pattern = QRegularExpression(QStringLiteral("\\b[A-Za-z0-9_]+(?=\\()"));
  rule.format  = mFunctionFormat;
  mHighlightingRules.append(rule);
}

void LuaHighlighter::highlightBlock(const QString& text) {
  for (const HighlightingRule& rule : qAsConst(mHighlightingRules)) {
    QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
    while (matchIterator.hasNext()) {
      QRegularExpressionMatch match = matchIterator.next();
      setFormat(match.capturedStart(), match.capturedLength(), rule.format);
    }
  }

  setCurrentBlockState(0);
}

}  // namespace AutoTrade::Ui
