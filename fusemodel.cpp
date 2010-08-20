#include "fusemodel.h"
#include <QDebug>

QVariant FuseModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        if (section == 0)
            return tr("Name");
        else
            return tr("Value");
    } else {
        return section;
    }
}

void FuseModel::reloadModel()
{
    reset();
}

Qt::ItemFlags FuseModel::flags(const QModelIndex &index) const
{
    if (index.column() == 1) {
        return Qt::ItemIsEditable | Qt::ItemIsEnabled;
    }
    return Qt::ItemIsEnabled;
}

QVariant FuseModel::data(const QModelIndex &index, int role) const
{
    int rowCounter = 0;
    for (int i = 0; i<part->fuseRegs.count(); i++) {
        for (int j = 0; j<part->fuseRegs[i].bitFields.count(); j++) {
            if (rowCounter == index.row()) {
                switch (index.column()) {
                case 0: { // fuse name
                        switch(role) {
                            case Qt::DisplayRole: return part->fuseRegs[i].bitFields[j].shortName; break;
                            case Qt::ToolTipRole: return part->fuseRegs[i].bitFields[j].text; break;
                        }; // switch role
                    } break; // column 0
                case 1: { // fuse value
                        if (role == Qt::UserRole) {
                            FuseBitField ret;
                            if (part->fuseRegs[i].bitFields[j].enumValues.isEmpty()) {
                                ret.isEnum = false;
                                return QVariant::fromValue(ret);
                            } else {
                                ret.isEnum = true;
                                ret.enumValues = part->fuseRegs[i].bitFields[j].enumValues;
                                return QVariant::fromValue(ret);
                            }
                        } else if (role ==  Qt::DisplayRole) {
                            return part->fuseRegs[i].bitFields[j].value;
                        }
                    } break; // column 1
                } // switch index
                break;
            } // we have found the searched bitfield
            rowCounter++;
        } // this loops on the fuses bitfields
    } // this cycle loops on the (HIGH LOW EXTENDED FUSE) part->fuseRegs
    return QVariant();
}

bool FuseModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    int rowCounter = 0;
    if ((role == Qt::DisplayRole)  || (role == Qt::EditRole)) {
        if (index.column() == 1) { //is the index points to the fuse value column?
            for (int i = 0; i<part->fuseRegs.count(); i++) {// this cycle loops on the (HIGH LOW EXTENDED FUSE) part->fuseRegs
                for (int j = 0; j<part->fuseRegs[i].bitFields.count(); j++) { // this loops on the fuses bitfields
                    if (rowCounter == index.row()) {
                        part->fuseRegs[i].bitFields[j].value = value.toInt();
                        return true;
                    } // we have found the searched bitfield
                    rowCounter++;
                } // this loops on the fuses bitfields
            } // this cycle loops on the (HIGH LOW EXTENDED FUSE) part->fuseRegs
        }
    }
    return false;
}

int FuseModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    int rowCount = 0;
    for (int i = 0; i<part->fuseRegs.count(); i++) {
        rowCount += part->fuseRegs[i].bitFields.count();
    }
    return rowCount;
}

int FuseModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return 2;
}

int FuseValuesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    if (currentMode == FuseBitDisplayMode_BinaryDetailed)
        return part->fuseRegs.size()*2;
    return part->fuseRegs.size();
}

int FuseValuesModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    if (currentMode == FuseBitDisplayMode_BinaryDetailed)
        return 9;
    return 2;
}

QVariant FuseValuesModel::data(const QModelIndex &index, int role) const
{
    if (currentMode == FuseBitDisplayMode_BinaryDetailed) {
        if (index.row()%2 ==  1) { // value row
            if ((index.column() == 0) && (role == Qt::DisplayRole)) {
                return part->fuseRegs[(index.row() - 1)/2].name;
            } else {
                return (bool)(part->fuseRegs[(index.row() - 1)/2].value & (1<<(index.column()-1)));
            }
        } else { // bits name row
            if (((index.column() > 0) && (index.column() < 9)) && (role == Qt::DisplayRole)) {
                return part->getFuseRegisterBitName((index.row())/2, index.column()-1);
            }
        }
    } else {
        if (index.row() < part->fuseRegs.size()) {
            if ((index.column() == 0) && (role == Qt::DisplayRole)) {
                return part->fuseRegs[index.row()].name;
            }

            if ((index.column() == 1) && (role == Qt::DisplayRole)) {
                return part->fuseRegs[index.row()].value;
            }
        }
    }
    return QVariant();
}

bool FuseValuesModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.row() < part->fuseRegs.size()) {
        if ((index.column() == 1) && (role == Qt::DisplayRole)){
            part->fuseRegs[index.row()].value = value.toInt();
            return true;
        }
    }
    return false;
}

QVariant FuseValuesModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        if (section == 0)
            return tr("Name");
        else
            return tr("Value");
    } else {
        return section;
    }
}

Qt::ItemFlags FuseValuesModel::flags(const QModelIndex &index) const
{
    if (currentMode == FuseBitDisplayMode_BinaryDetailed) {
        if (index.row() %2 == 0) {
            return Qt::ItemIsEnabled;
        } else {
            if (index.column() > 0)
                return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
        }
    } else {
        if (index.column() == 1) {
            return Qt::ItemIsEditable | Qt::ItemIsEnabled;
        }
    }
    return Qt::ItemIsEnabled;
}

void FuseValuesModel::reloadModel()
{
    reset();
}

void FuseValuesModel::setDisplayMode(FuseBitDisplayMode mode)
{
    currentMode = mode;
    if (mode == FuseBitDisplayMode_BinaryDetailed) {
        emit dataChanged(this->index(0,0), this->index(part->fuseRegs.size()*2, 9));
    }
    this->reset();
}

