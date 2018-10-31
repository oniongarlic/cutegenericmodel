#ifndef DUMMYITEMMODEL_H
#define DUMMYITEMMODEL_H

#include <QObject>

#include "abstractobjectmodel.h"
#include "dummyitem.h"

class DummyItemModel : public Cute::AbstractObjectModel
{
    Q_OBJECT
public:
    explicit DummyItemModel(QObject *parent = nullptr);

    Q_INVOKABLE DummyItem *getItem(int index) const;
};

#endif // DUMMYITEMMODEL_H
