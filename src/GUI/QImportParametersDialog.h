/*
 * QImportParametersDialog.h
 *
 *  Created on: 23 mars 2021
 *      Author: gsegard
 */

#ifndef SRC_GUI_QIMPORTPARAMETERSDIALOG_H_
#define SRC_GUI_QIMPORTPARAMETERSDIALOG_H_

#include <QDialog>
#include <QComboBox>

class QImportParametersDialog : public QDialog {
	Q_OBJECT
public :
	enum FieldSeparatorType {
		FieldSeparatorType_Comma,
		FieldSeparatorType_Semicolon,
		FieldSeparatorType_Colon,
		FieldSeparatorType_Space,
		FieldSeparatorType_TabKey,
	};

	enum StringSeparatorType {
		StringSeparatorType_DoubleQuote,
		StringSeparatorType_SimpleQuote,
	};

public:
	QImportParametersDialog(QWidget* parent = 0);
	virtual ~QImportParametersDialog();

	QString getFilePath() const;
	QString getTextFieldSeparator() const;
	QString getTextStringSeparator() const;

public slots :
	void explore();

private:
	QComboBox* m_pComboBoxFieldSeparator;
	QComboBox* m_pComboBoxStringSeparator;
	QLineEdit* m_pFileExplorerLineEdit;

	QString m_szCurrentPath;
};

#endif /* SRC_GUI_QIMPORTPARAMETERSDIALOG_H_ */
