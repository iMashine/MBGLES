#include "emitterlist.h"

EmitterList::EmitterList(QObject *parent) : QAbstractItemModel(parent)
{
    m_select = QModelIndex();
}

QVariant EmitterList::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (m_list.isEmpty()) {
        return QVariant();
    }

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return tr("Emitter name");
        default:
            return QVariant();
        }
    }

    return QVariant();
}

QModelIndex EmitterList::index(int row, int column, const QModelIndex &parent) const
{
    if (m_list.isEmpty()) {
        return QModelIndex();
    }

    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    return createIndex(row, column, Q_NULLPTR);//QModelIndex()); //m_list.at(row));
}

QModelIndex EmitterList::parent(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return QModelIndex();
}

int EmitterList::rowCount(const QModelIndex &parent) const
{
    if (m_list.isEmpty()) {
        return 0;
    }

    if (!parent.isValid()) {
        return m_list.count();
    }

    return 0;
}

int EmitterList::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant EmitterList::data(const QModelIndex &index, int role) const
{
    if (m_list.isEmpty()) {
        return QVariant();
    }

    if (!index.isValid()) {
        return QVariant();
    }

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return m_list.at(index.row())->GetEmitterName();
        default:
            return QVariant();
        }
    }

    return QVariant();
}

Qt::ItemFlags EmitterList::flags(const QModelIndex &index) const
{
    if (m_list.isEmpty()) {
        return Qt::NoItemFlags;
    }

    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool EmitterList::add(unsigned int id, unsigned int index)
{
    if (m_list.count() == 0) {
        beginInsertRows(QModelIndex(), 0, 0);
        m_list.append(new Emitter(id));
    }
    else {
        if (index == -1) {
            beginInsertRows(QModelIndex(), m_list.count(), m_list.count());
            m_list.append(new Emitter(id));
        }
        else {
            if (index >= m_list.count()) return false;
            beginInsertRows(QModelIndex(), index, index);
            m_list.append(new Emitter(id));
        }
    }
    endInsertRows();

    return true;
}

bool EmitterList::add(Emitter *emitter, unsigned int index)
{
    if (m_list.count() == 0) {
        beginInsertRows(QModelIndex(), 0, 0);
        m_list.append(emitter);
    }
    else {
        if (index == -1) {
            beginInsertRows(QModelIndex(), m_list.count(), m_list.count());
            m_list.append(emitter);
        }
        else {
            if (index >= m_list.count()) return false;
            beginInsertRows(QModelIndex(), index, index);
            m_list.insert(index, emitter);
        }
    }
    endInsertRows();

    return true;
}

bool EmitterList::remove(unsigned int index, unsigned int id)
{
    if (m_list.isEmpty()) return false;

    if (id == -1) {
        beginRemoveRows(QModelIndex(), index, index);
        m_list.removeAt(index);
        endRemoveRows();
    }
    else {
        unsigned int removeIndex = -1;
        if ((removeIndex = getArrayIndex(id)) == -1) return false;
        remove(removeIndex);
    }

    return true;
}

bool EmitterList::edit(unsigned int index, Emitter *emitter, unsigned int id)
{
    if (m_list.isEmpty()) return false;

    if (index != -1) {
        if (index >= m_list.count()) return false;
        dataChanged(createIndex(index, 0, m_list.at(index)), createIndex(index, columnCount() - 1, m_list.at(index)));
        m_list.replace(index, emitter);
    }
    else {
        unsigned int editIndex = -1;
        if ((editIndex = getArrayIndex(id)) == -1) return false;
        dataChanged(createIndex(editIndex, 0, m_list.at(editIndex)), createIndex(editIndex, columnCount() - 1, m_list.at(editIndex)));
        m_list.replace(editIndex, emitter);
    }

    return true;
}

bool EmitterList::replace(unsigned int oldIndex, unsigned int newIndex)
{
    if (m_list.isEmpty())
        return false;

    if (getArrayIndex(oldIndex) == -1 || getArrayIndex(newIndex) == -1) return false;
    beginMoveRows(QModelIndex(), oldIndex, oldIndex, QModelIndex(), newIndex);
    m_list.move(oldIndex, newIndex);
    endMoveRows();

    return true;
}

void EmitterList::clear()
{
    beginResetModel();
    m_list.clear();
    endResetModel();
}

void EmitterList::selectItem(QModelIndex index)
{
    if (!m_list.isEmpty()) {
        if (m_select.row() == -1) {
            m_select = index;
            m_list.at(m_select.row())->SetState(MAGIC_STATE_UPDATE);
            Q_ASSERT(MAGIC_SUCCESS == Magic_SetBBoxPeriod(m_list.at(m_select.row())->GetId(), 1));
        }
        else {
            m_list.at(m_select.row())->SetState(MAGIC_STATE_STOP);
            m_select = index;
            m_list.at(m_select.row())->SetState(MAGIC_STATE_UPDATE);
        }
    }
}

QModelIndex EmitterList::selectItem()
{
    if (m_select.row() != -1) return m_select;

    return QModelIndex();
}

Emitter *EmitterList::get(QModelIndex index)
{
    if (m_list.isEmpty()) return Q_NULLPTR;
    if (index.row() == -1) return Q_NULLPTR;
    if (index.row() == m_list.count()) return Q_NULLPTR;
    return m_list.at(index.row());
}

Emitter *EmitterList::get(unsigned int id)
{
    QModelIndex index = createIndex(getArrayIndex(id), 0);
    if (m_list.isEmpty()) return Q_NULLPTR;
    if (index.row() == -1) return Q_NULLPTR;
    if (index.row() == m_list.count()) return Q_NULLPTR;
    return m_list.at(index.row());
}

Emitter *EmitterList::first()
{
    if (m_list.isEmpty()) return Q_NULLPTR;

    m_select = index(0, 0, QModelIndex());
    return m_list.first();
}

Emitter *EmitterList::next()
{
    if (m_list.isEmpty()) return Q_NULLPTR;
    if (m_select.row() == -1) return Q_NULLPTR;
    if (m_select.row() + 1 == m_list.count()) return first();

    m_select = index(m_select.row() + 1, 0, QModelIndex());
    return m_list.at(m_select.row());
}

Emitter *EmitterList::prev()
{
    if (m_list.isEmpty()) return Q_NULLPTR;
    if (m_select.row() == -1) return Q_NULLPTR;
    if (m_select.row() - 1 < 0) return last();

    m_select = index(m_select.row() - 1, 0, QModelIndex());
    return m_list.at(m_select.row());
}

Emitter *EmitterList::last()
{
    if (m_list.isEmpty()) return Q_NULLPTR;

    m_select = index(m_list.count() - 1, 0, QModelIndex());
    return m_list.last();
}

unsigned int EmitterList::getArrayIndex(unsigned int id)
{
    unsigned int Index = -1;

    for (int i = 0; i < m_list.count(); i++) {
        if (m_list[i]->GetId() == id) {
            Index = i;
            break;
        }
    }

    return Index;
}
