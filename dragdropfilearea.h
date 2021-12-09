#ifndef DRAGDROPFILEAREA_H
#define DRAGDROPFILEAREA_H

#include <QLabel>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QString>
#include <QWidget>
#include <QObject>

class DragDropFileArea: public QLabel
{
    Q_OBJECT
public:
    DragDropFileArea();

    void dropEvent(QDropEvent *event) Q_DECL_OVERRIDE;
    void dragEnterEvent(QDragEnterEvent*event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QDragMoveEvent *event) Q_DECL_OVERRIDE;

signals:
    void fileDropped(QString);

private:
    bool m_isDrag;
    QString m_filename;
    int counter;


};

#endif // DRAGDROPFILEAREA_H
