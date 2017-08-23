/*
 * QAboutDialog.h
 *
 *  Created on: 11 juil. 2012
 *      Author: ebeuque
 */

#ifndef QABOUTDIALOG_H_
#define QABOUTDIALOG_H_

#include <QDialog>
#include <QLayout>
#include <QString>
#include <QAbstractButton>

/*
 *
 */
class QAboutDialog : public QDialog {
	Q_OBJECT
public:
	QAboutDialog(QWidget * parent = 0);
	virtual ~QAboutDialog();

public slots:
	void menuAboutQtClicked();
	void menuAboutFFmpegClicked();
	void menuAboutLibcurlClicked();

private:
	QAbstractButton* addLibraryButton(QLayout* pLayout, QWidget* pParent, const QString& szIconFile, const QString& szName);
};

#endif /* QABOUTDIALOG_H_ */
