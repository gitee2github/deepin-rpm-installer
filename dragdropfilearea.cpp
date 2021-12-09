#include "dragdropfilearea.h"

DragDropFileArea::DragDropFileArea()
{
    this->m_isDrag=false;
    this->setAcceptDrops(true);
    this->counter=0;
}

// 已经放下的事件
void DragDropFileArea::dropEvent(QDropEvent *event)
{
    this->m_filename = event->mimeData()->urls()[0].toLocalFile();
    emit this->fileDropped(this->m_filename);   // 给自己发信号，带参数
}

// 拖拽进来的事件
void DragDropFileArea::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    } else {
        event->ignore();
    }
}

void DragDropFileArea::dragMoveEvent(QDragMoveEvent *event)
{

}
