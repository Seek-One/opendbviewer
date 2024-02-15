/*
 * QSqlHighlighterController.cpp
 *
 *  Created on: 1 juil. 2016
 *      Author: echopin
 */

#include <QtGlobal>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QRegularExpression>
#else
#include <QRegExp>
#endif

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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
		rule.m_pattern = QRegularExpression(szPattern);
#else
		rule.m_pattern = QRegExp(szPattern);
#endif
		rule.m_format = m_keywordFormat;
		highlightingRules.append(rule);
	}

	//single line comment format rule
	m_singleLineCommentFormat.setForeground(Qt::blue);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	rule.m_pattern = QRegularExpression("--[^\n]*");
#else
	rule.m_pattern = QRegExp("--[^\n]*");
#endif
	rule.m_format = m_singleLineCommentFormat;
	highlightingRules.append(rule);

	//multiline comment format rule used in mySQL
	m_multiLineCommentFormat.setForeground(Qt::blue);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	m_commentStartExpression = QRegularExpression("/\\*");
	m_commentEndExpression = QRegularExpression("\\*/");
#else
	m_commentStartExpression = QRegExp("/\\*");
	m_commentEndExpression = QRegExp("\\*/");
#endif

	//quotation format rule
	//TODO multiline quotation format, could not get it to work alongside multiline comment format
	m_quotationFormat.setForeground(Qt::magenta);
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	rule.m_pattern = QRegularExpression("'.*'");
#else
	rule.m_pattern = QRegExp("'.*'");
#endif
	rule.m_format = m_quotationFormat;
	highlightingRules.append(rule);
}

QSqlHighlighterController::~QSqlHighlighterController()
{
}

void QSqlHighlighterController::highlightBlock(const QString &szText)
{
	// Highlight SQL keyword
	foreach (const HighlightingRule &rule, highlightingRules)
	{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
		QRegularExpression expression(rule.m_pattern);
		expression.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
		QRegularExpressionMatchIterator iter = expression.globalMatch(szText);
		while (iter.hasNext()) {
			QRegularExpressionMatch regExpMatch = iter.next();
			if (regExpMatch.hasMatch()) {
				setFormat((int) regExpMatch.capturedStart(), (int) regExpMatch.capturedLength(), rule.m_format);
			}
		}
#else
		QRegExp expression(rule.m_pattern);
		expression.setCaseSensitivity(Qt::CaseInsensitive);
		int index = expression.indexIn(szText);
		while (index >= 0)
		{
			int length = expression.matchedLength();
			setFormat(index, length, rule.m_format);
			index = expression.indexIn(szText, index + length);
		}
#endif
	}

	// Highlight comments
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
	qsizetype iStartIndex = 0;
	qsizetype iEndIndex = -1;

	// Look for comments start
	QRegularExpressionMatchIterator iter = m_commentStartExpression.globalMatch(szText);
	while (iter.hasNext()) {
		QRegularExpressionMatch startMatch = iter.next();
		if (startMatch.hasMatch()) {
			iStartIndex = startMatch.capturedStart();
			if (iStartIndex > iEndIndex) {
				// Look for comments end
				QRegularExpressionMatch endMatch = m_commentEndExpression.match(szText, iStartIndex);
				qsizetype iCommentLength = 0;
				if (endMatch.hasMatch()) {
					iEndIndex = endMatch.capturedEnd();
					iCommentLength = iEndIndex - iStartIndex;
				} else {
					iCommentLength = szText.length() - iStartIndex;
				}
				setFormat((int) iStartIndex, (int) iCommentLength, m_multiLineCommentFormat);
			} else {
				// We find a start comment within a comment
			}
		}
	}
#else
	setCurrentBlockState(0);

	int startIndex = 0;
	if (previousBlockState() != 1){
		startIndex = m_commentStartExpression.indexIn(szText);
	}

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
#endif
}

QStringList QSqlHighlighterController::makeKeywordList()
{
	//Defining the list of keywords here, could maybe be defined in a text file
	QStringList szKeywordList;
	//Words are sorted alphabetically
	szKeywordList 	<< "\\bABORT\\b" << "\\bACTION\\b" << "\\bADD\\b" << "\\bAFTER\\b" << "\\bALL\\b" << "\\bALTER\\b"
					<< "\\bANALYZE\\b" << "\\bAND\\b" << "\\bAS\\b" << "\\bASC\\b" << "\\bATTACH\\b" << "\\bAUTOINCREMENT\\b" << "\\bBEFORE\\b"
					<< "\\bBEGIN\\b" << "\\bBETWEEN\\b" << "\\bBY\\b" << "\\bCASCADE\\b" << "\\bCASE\\b" << "\\bCAST\\b" << "\\bCHECK\\b" << "\\bCOLLATE\\b"
					<< "\\bCOLUMN\\b" << "\\bCOMMIT\\b" << "\\bCONFLICT\\b" << "\\bCONSTRAINT\\b" << "\\bCREATE\\b" << "\\bCROSS\\b"
					<< "\\bCURRENT_DATE\\b" << "\\bCURRENT_TIME\\b" << "\\bCURRENT_TIMESTAMP\\b" << "\\bDATABASE\\b"
					<< "\\bDEFAULT\\b" << "\\bDEFERRABLE\\b" << "\\bDEFERRED\\b" << "\\bDELETE\\b" << "\\bDESC\\b" << "\\bDETACH\\b"
					<< "\\bDISTINCT\\b" << "\\bDROP\\b" << "\\bEACH\\b" << "\\bELSE\\b" << "\\bEND\\b" << "\\bESCAPE\\b" << "\\bEXCEPT\\b"
					<< "\\bEXCLUSIVE\\b" << "\\bEXISTS\\b" << "\\bEXPLAIN\\b" << "\\bFAIL\\b" << "\\bFOR\\b" << "\\bFOREIGN\\b" << "\\bFROM\\b"
					<< "\\bFULL\\b" << "\\bGLOB\\b" << "\\bGROUP\\b" << "\\bHAVING\\b" << "\\bIF\\b" << "\\bIGNORE\\b" << "\\bIMMEDIATE\\b" << "\\bIN\\b"
					<< "\\bINDEX\\b" << "\\bINDEXED\\b" << "\\bINITIALLY\\b" << "\\bINNER\\b" << "\\bINSERT\\b" << "\\bINSTEAD\\b"
					<< "\\bINTERSECT\\b" << "\\bINTO\\b" << "\\bIS\\b" << "\\bISNULL\\b" << "\\bJOIN\\b" << "\\bKEY\\b" << "\\bLEFT\\b" << "\\bLIKE\\b"
					<< "\\bLIMIT\\b" << "\\bMATCH\\b" << "\\bNATURAL\\b" << "\\bNO\\b" << "\\bNOT\\b" << "\\bNOTNULL\\b" << "\\bNULL\\b" << "\\bOF\\b"
					<< "\\bOFFSET\\b" << "\\bON\\b" << "\\bOR\\b" << "\\bORDER\\b" << "\\bOUTER\\b" << "\\bPLAN\\b" << "\\bPRAGMA\\b" << "\\bPRIMARY\\b"
					<< "\\bQUERY\\b" << "\\bRAISE\\b" << "\\bRECURSIVE\\b" << "\\bREFERENCES\\b" << "\\bREGEXP\\b" << "\\bREINDEX\\b"
					<< "\\bRELEASE\\b" << "\\bRENAME\\b" << "\\bREPLACE\\b" << "\\bRESTRICT\\b" << "\\bRIGHT\\b" << "\\bROLLBACK\\b" << "\\bROW\\b"
					<< "\\bSAVEPOINT\\b" << "\\bSELECT\\b" << "\\bSET\\b" << "\\bTABLE\\b" << "\\bTEMP\\b" << "\\bTEMPORARY\\b" << "\\bTHEN\\b"
					<< "\\bTO\\b" << "\\bTRANSACTION\\b" << "\\bTRIGGER\\b" << "\\bUNION\\b" << "\\bUNIQUE\\b" << "\\bUPDATE\\b" << "\\bUSING\\b"
					<< "\\bVACUUM\\b" << "\\bVALUES\\b" << "\\bVIEW\\b" << "\\bVIRTUAL\\b" << "\\bWHEN\\b" << "\\bWHERE\\b" << "\\bWITH\\b"
					<< "\\bWITHOUT";
	szKeywordList 	<< "\\bTEXT\\b" << "\\bINTEGER\\b" << "\\bREAL\\b" << "\\bNUMERIC\\b" << "\\bNONE\\b" << "\\bBLOB\\b";
	szKeywordList 	<< "\\bTRUE\\b" << "\\bFALSE\\b";

	return szKeywordList;
}
