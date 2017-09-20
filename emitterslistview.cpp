#include "emitterslistview.h"
#include "mainwidget.h"

EmittersListView::EmittersListView(QWidget *parent)
    : QListView(parent)
{
    setSelectionMode(QAbstractItemView::ExtendedSelection);
}

EmittersListView::EmittersListView(EmitterList *list, QWidget *parent)
    : QListView(parent)
{
    m_list = list;
    this->setModel(m_list);
    m_widget = (MainWidget *)parent;
}

void EmittersListView::setCurrentItemIndex(QModelIndex index)
{
    if (index.row() != -1) {
        this->setCurrentIndex(index);
        QItemSelection selection(index, index);
        this->selectionModel()->select(selection, QItemSelectionModel::Select);
        setSelectionBehavior(QAbstractItemView::SelectItems);
    }
}

void EmittersListView::mousePressEvent(QMouseEvent *e)
{
    QListView::mousePressEvent(e);
}

void EmittersListView::mouseDoubleClickEvent(QMouseEvent *e)
{
    QListView::mouseDoubleClickEvent(e);
    m_list->selectItem(this->indexAt(e->pos()));
    setCurrentItemIndex(m_list->selectItem());
    m_widget->refreshUI();
}

Emitter *EmittersListView::selection()
{
    if (this->currentIndex().row() != -1)
        return m_list->get(this->currentIndex());
    return Q_NULLPTR;
}
