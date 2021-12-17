#ifndef SELECTPAGE_H
#define SELECTPAGE_H

#include "common/dragdropfilearea.h"

#include <QWidget>

class SelectPage : public QWidget
{
    Q_OBJECT
public:
    explicit SelectPage(QWidget *parent = nullptr);

    void selectRpmBtnHandler();

signals:
    void filenameReceived(QString filename);

    void selectRpmBtnClicked();

public slots:

private:
    void initUI();



    DragDropFileArea *dragDropArea = nullptr;
};

#endif // SELECTPAGE_H
