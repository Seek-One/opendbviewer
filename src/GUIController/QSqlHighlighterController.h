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
#include <QTextCharFormat>
#include <QString>
#include <QTextDocument>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QRegularExpression>
#else
#include <QRegExp>
#endif


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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
		QRegularExpression m_pattern;
#else
        QRegExp m_pattern;
#endif
        QTextCharFormat m_format;
    };
    QVector<HighlightingRule> highlightingRules;

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	QRegularExpression m_commentStartExpression;
    QRegularExpression m_commentEndExpression;
#else
    QRegExp m_commentStartExpression;
    QRegExp m_commentEndExpression;
#endif

    QTextCharFormat m_keywordFormat;
    QTextCharFormat m_singleLineCommentFormat;
    QTextCharFormat m_multiLineCommentFormat;
    QTextCharFormat m_quotationFormat;
};

#endif /* SRC_GUICONTROLLER_QSQLHIGHLIGHTERCONTROLLER_H_ */
