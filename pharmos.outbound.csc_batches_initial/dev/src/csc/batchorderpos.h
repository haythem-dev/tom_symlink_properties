#ifndef BATCHORDERPOS_H
#define BATCHORDERPOS_H

#include <QDialog>
#include "ui_batchorderpos.h"

class BatchOrderPos : public QDialog
{
    Q_OBJECT

public:
    BatchOrderPos(QWidget *parent = 0);
    ~BatchOrderPos();

private:
    Ui::BatchOrderPosClass ui;
};

#endif // BATCHORDERPOS_H
