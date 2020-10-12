#ifndef Letters_ColorPattern_H
#define Letters_ColorPattern_H
#include <QSyntaxHighlighter>
#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

//! [0]
class Letters_ColorPattern : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Letters_ColorPattern(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text);

private:
    struct ColorRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<ColorRule> colorrules;



    QTextCharFormat Letter_A_Format;
    QTextCharFormat Letter_C_Format;
    QTextCharFormat Letter_G_Format;
    QTextCharFormat Letter_T_Format;

};



#endif // Letters_ColorPattern_H
