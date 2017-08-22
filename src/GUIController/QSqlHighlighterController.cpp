/*
 * QSqlHighlighterController.cpp
 *
 *  Created on: 1 juil. 2016
 *      Author: echopin
 */

#include "GUIController/QSqlHighlighterController.h"

QSqlHighlighterController::QSqlHighlighterController(QTextDocument *parent)
: QSyntaxHighlighter(parent)
{
	HighlightingRule rule;

	//keyword format
	m_keywordFormat.setForeground(Qt::darkRed);
	m_keywordFormat.setFontWeight(QFont::Bold);
	m_keywordFormat.setFontCapitalization(QFont::AllUppercase);//not working
	QStringList szKeywordPatterns;

	szKeywordPatterns = makeKeywordList();
	foreach(const QString &szPattern, szKeywordPatterns)
	{
		rule.m_pattern = QRegExp(szPattern);
		rule.m_format = m_keywordFormat;
		highlightingRules.append(rule);
	}

	//single line comment format rule
	m_singleLineCommentFormat.setForeground(Qt::blue);
	rule.m_pattern = QRegExp("--[^\n]*");
	rule.m_format = m_singleLineCommentFormat;
	highlightingRules.append(rule);

	//multiline comment format rule used in mySQL
	m_multiLineCommentFormat.setForeground(Qt::blue);
	m_commentStartExpression = QRegExp("/\\*");
	m_commentEndExpression = QRegExp("\\*/");

	//quotation format rule
	//TODO multiline quotation format, could not get it to work alongside multiline comment format
	m_quotationFormat.setForeground(Qt::magenta);
	rule.m_pattern = QRegExp("'.*'");
	rule.m_format = m_quotationFormat;
	highlightingRules.append(rule);
}

QSqlHighlighterController::~QSqlHighlighterController()
{
}

void QSqlHighlighterController::highlightBlock(const QString &szText)
{
	foreach (const HighlightingRule &rule, highlightingRules)
	{
		QRegExp expression(rule.m_pattern);
		int index = expression.indexIn(szText);
		while (index >= 0)
		{
			int length = expression.matchedLength();
			setFormat(index, length, rule.m_format);
			index = expression.indexIn(szText, index + length);
	    }
	}

	setCurrentBlockState(0);

	int startIndex = 0;
	if (previousBlockState() != 1)
		startIndex = m_commentStartExpression.indexIn(szText);

	while (startIndex >= 0) {
		int endIndex = m_commentEndExpression.indexIn(szText, startIndex);
		int commentLength;
		if (endIndex == -1) {
			setCurrentBlockState(1);
			commentLength = szText.length() - startIndex;
		}
		else
		{
			commentLength = endIndex - startIndex + m_commentEndExpression.matchedLength();
		}
		setFormat(startIndex, commentLength, m_multiLineCommentFormat);
		startIndex = m_commentStartExpression.indexIn(szText, startIndex + commentLength);
	}
}

QStringList QSqlHighlighterController::makeKeywordList()
{
	//Defining the list of keywords here, could maybe be defined in a text file
	QStringList szKeywordList;
	//Words are sorted alphabetically
	szKeywordList 	<< "\\balter\\b" << "\\bcreate\\b" << "\\bfrom\\b" //TODO add more keywords to the list
					<< "\\bdrop\\b" << "\\bselect\\b" << "\\btable\\b" << "\\bwhere\\b";
	return szKeywordList;
}
