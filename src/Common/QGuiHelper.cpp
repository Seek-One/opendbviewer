/*
 * QGuiHelper.cpp
 *
 *  Created on: 6 sept. 2017
 *      Author: ebeuque
 */

#include <QTreeWidget>

#include "QGuiHelper.h"

void QGuiHelper::resizeColumnsToContents(QTreeWidget *pTreeWidget)
{
  int cCols = pTreeWidget->columnCount();
  int cItems = pTreeWidget->topLevelItemCount();
  int w;
  int col;
  int i;
  for( col = 0; col < cCols; col++ ) {
    w = pTreeWidget->header()->sectionSizeHint( col );
    for( i = 0; i < cItems; i++ ){
      w = qMax( w, pTreeWidget->topLevelItem( i )->text( col ).size()*7 + (col == 0 ? pTreeWidget->indentation() : 0) );
    }
    pTreeWidget->header()->resizeSection( col, w );
  }
}
