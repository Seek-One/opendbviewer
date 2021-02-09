/*
 * QExportParametersDialog.h
 *
 *  Created on: 4 feb. 2021
 *      Author: gsegard
 */

#ifndef QEXPORTPARAMETERSDIALOG_H_
#define QEXPORTPARAMETERSDIALOG_H_

#include <QDialog>
#include <QComboBox>
#include <QCheckBox>

class QExportParametersDialog : public QDialog {
	Q_OBJECT
public:
	enum FieldSeparatorType {
		FieldSeparatorType_Comma,
		FieldSeparatorType_Semicolon,
		FieldSeparatorType_Colon,
		FieldSeparatorType_Space,
		FieldSeparatorType_TabKey,
	};

	enum StringSeparatorType {
		StringSeparatorType_None,
		StringSeparatorType_SimpleQuote,
		StringSeparatorType_DoubleQuote,
	};

	enum LineBreakSeparatorType {
		LineBreakSeparatorType_LF,
		LineBreakSeparatorType_CRLF,
	};

public:
	QExportParametersDialog(QWidget* pParent = 0);
	virtual ~QExportParametersDialog();

	QString getFilePath() const;
	QString getTextFieldSeparator() const;
	QString getTextStringSeparator() const;
	QString getTextLineBreakSeparator() const;
	bool isIncludesHeaders();

public slots :
	void explore();

private:
	QComboBox* m_pComboBoxFieldSeparator;
	QComboBox* m_pComboBoxStringSeparator;
	QComboBox* m_pComboBoxLineBreakSeparator;
	QCheckBox* m_pCheckBoxHeader;
	QLineEdit* m_pFileExplorerLineEdit;

	bool m_bIncludesHeaders;

};

#endif /* QEXPORTPARAMETERSDIALOG_H_ */
