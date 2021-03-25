/*
 * QExportParametersProgressBarDialog.h
 *
 *  Created on: 23 févr. 2021
 *      Author: gsegard
 */

#ifndef SRC_GUI_QEXPORTPARAMETERSPROGRESSBARDIALOG_H_
#define SRC_GUI_QEXPORTPARAMETERSPROGRESSBARDIALOG_H_

#include <QDialog>
#include <QApplication>
#include <QDialogButtonBox>

class QProgressBar;

class QExportParametersProgressBarDialog : public QDialog
{
	Q_OBJECT
public:
	QExportParametersProgressBarDialog(QString szTitle, QWidget* parent);
	virtual ~QExportParametersProgressBarDialog();

	QDialogButtonBox* getCancelButton() const;
	bool isCancel() const;
	void setData(int iData);
	void setMaximumData(int maxData);

public slots:
	void setCancel();

private:
	QProgressBar* m_pProgressBar;
	QDialogButtonBox* m_pCancelButtonBox;
	bool m_bCancel;
};

#endif /* SRC_GUI_QEXPORTPARAMETERSPROGRESSBARDIALOG_H_ */
