#include "installprocessinfoview.h"
#include <QAbstractTextDocumentLayout>
#include <QTextDocumentFragment>
#include <QScrollBar>
#include <QEvent>
#include <QDebug>

ShowInstallInfoTextEdit::ShowInstallInfoTextEdit(QWidget *parent):
    QTextEdit(parent)
{
    setAttribute(Qt::WA_AcceptTouchEvents);
    grabGesture(Qt::TapGesture);                        //获取触控点击事件
    grabGesture(Qt::TapAndHoldGesture);                 //获取触控点击长按事件

    //滑动鼠标时选中的效果
    connect(this, &ShowInstallInfoTextEdit::selectionChanged, this, &ShowInstallInfoTextEdit::onSelectionArea);
}

