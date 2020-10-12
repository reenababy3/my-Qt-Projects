#include "highlightletters.h"

Letters_ColorPattern::Letters_ColorPattern(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    ColorRule rule;

    //Letter_A_Format.setFontWeight(QFont::Bold);
    Letter_A_Format.setForeground(Qt::green);
    rule.pattern = QRegExp("[A]");
    rule.format = Letter_A_Format;
    colorrules.append(rule);

    Letter_C_Format.setForeground(Qt::blue);
    rule.pattern = QRegExp("[C]");
    rule.format = Letter_C_Format;
    colorrules.append(rule);

    Letter_G_Format.setForeground(Qt::black);
    rule.pattern = QRegExp("[G]");
    rule.format = Letter_G_Format;
    colorrules.append(rule);

    Letter_T_Format.setForeground(Qt::red);
    rule.pattern = QRegExp("[T]");
    rule.format = Letter_T_Format;
    colorrules.append(rule);
}

void Letters_ColorPattern::highlightBlock(const QString &text)
{
    foreach (const ColorRule &rule, colorrules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {

            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    setCurrentBlockState(0);


}

