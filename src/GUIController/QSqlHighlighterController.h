/*
 * QSqlHighlighterController.h
 *
 *  Created on: 1 juil. 2016
 *      Author: echopin
 */

#ifndef SRC_GUICONTROLLER_QSQLHIGHLIGHTERCONTROLLER_H_
#define SRC_GUICONTROLLER_QSQLHIGHLIGHTERCONTROLLER_H_

#include <QObject>
#include <QSyntaxHighlighter>
#include <QRegExp>
#include <QTextCharFormat>
#include <QString>
#include <QTextDocument>

/* Class created as per http://doc.qt.io/qt-5/qtwidgets-richtext-syntaxhighlighter-example.html
 */

class QSqlHighlighterController : public QSyntaxHighlighter
{
	Q_OBJECT
public:
	QSqlHighlighterController(QTextDocument *parent = 0);
	virtual ~QSqlHighlighterController();

protected:
    void highlightBlock(const QString &szText);

private:
    QStringList makeKeywordList();

private:
    struct HighlightingRule
    {
        QRegExp m_pattern;
        QTextCharFormat m_format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp m_commentStartExpression;
    QRegExp m_commentEndExpression;

    QTextCharFormat m_keywordFormat;
    QTextCharFormat m_singleLineCommentFormat;
    QTextCharFormat m_multiLineCommentFormat;
    QTextCharFormat m_quotationFormat;
};

#endif /* SRC_GUICONTROLLER_QSQLHIGHLIGHTERCONTROLLER_H_ */
