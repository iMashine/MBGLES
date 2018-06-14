#ifndef EMITTERSLISTVIEW_H
#define EMITTERSLISTVIEW_H

#include "emitterlist.h"

#include <QListView>
#include <QMouseEvent>

class MainWidget;

// вьюха отображающая список эмиттеров
class EmittersListView : public QListView
{
    Q_OBJECT

public:
    EmittersListView(QWidget *parent = Q_NULLPTR);

    EmittersListView(EmitterList *list, QWidget *parent = Q_NULLPTR);

    void setCurrentItemIndex(QModelIndex index);

    Emitter *selection();

protected:
    virtual void mousePressEvent(QMouseEvent *e);

    virtual void mouseDoubleClickEvent(QMouseEvent *e);

    EmitterList *m_list;

    MainWidget *m_widget;

};

#endif // EMITTERSLISTVIEW_H
