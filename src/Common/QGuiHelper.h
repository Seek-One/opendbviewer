/*
 * QGuiHelper.h
 *
 *  Created on: 6 sept. 2017
 *      Author: ebeuque
 */

#ifndef SRC_COMMON_QGUIHELPER_H_
#define SRC_COMMON_QGUIHELPER_H_

class QTreeWidget;

class QGuiHelper {
public:
	QGuiHelper();
	virtual ~QGuiHelper();

	static void resizeColumnsToContents(QTreeWidget *pTreeWidget);
};

#endif /* SRC_COMMON_QGUIHELPER_H_ */
