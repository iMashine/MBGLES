

#ifndef EMITTERLIST_H
#define EMITTERLIST_H

#include "mpemitter.h"


#include <QAbstractItemModel>
#ifdef QT_DEBUG
#include <QDebug>
#endif

class EmitterList : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit EmitterList(QObject *parent = Q_NULLPTR);

    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override final;

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override final;

    virtual QModelIndex parent(const QModelIndex &index) const override final;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override final;

    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override final;

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override final;

    virtual Qt::ItemFlags flags(const QModelIndex &index) const override final;

    bool add(unsigned int id, unsigned int index = -1);

    bool add(Emitter *emitter, unsigned int index = -1);

    bool remove(unsigned int index, unsigned int id = -1);

    bool edit(unsigned int index, MPEmitter *emitter, unsigned int id = -1);

    bool replace(unsigned int oldIndex, unsigned int newIndex);

    void clear();

    void selectItem(QModelIndex index);

    QModelIndex selectItem();

    Emitter *get(QModelIndex index);

    Emitter *get(unsigned int id);

    Emitter *first();

    Emitter *next();

    Emitter *prev();

    Emitter *last();

    bool isEmpty();

private:
    unsigned int getArrayIndex(unsigned int id);

private:
    QList<Emitter *> m_list;

    QModelIndex m_select;
};

#endif // EMITTERLIST_H
