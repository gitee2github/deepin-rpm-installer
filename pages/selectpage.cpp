#include "selectpage.h"

#include "mainwindow.h"

#include <QVBoxLayout>
#include <QSvgRenderer>
#include <QPainter>
#include <QPushButton>
#include <QFileDialog>

SelectPage::SelectPage(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void SelectPage::initUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout;      // 定义 QVBoxLayout
    setLayout(mainLayout);

    QPixmap *pixmap = new QPixmap();                // 多种格式的图片都可以存为 pixmap

    QSvgRenderer svgRender(QString(":/icon_install_light_resized.svg"));   // 使用 painter 和 renderer
    QImage image(200, 200, QImage::Format_ARGB32);
    image.fill(0x00FFFFFF);
    QPainter painter(&image);
    svgRender.render(&painter);

    pixmap->convertFromImage(image);

    dragDropArea = new DragDropFileArea();
    dragDropArea->setPixmap(*pixmap);
    connect(dragDropArea, &DragDropFileArea::fileDropped, this, &SelectPage::filenameReceived);

    QPixmap splitLinePixmap;
    splitLinePixmap.load(":/split_line.svg");
    QLabel * splitLine = new QLabel();
    splitLine->setPixmap(splitLinePixmap);

    QLabel *dropHereText = new QLabel();
    dropHereText->setText("拖拽 rpm 包到此");

    QPushButton *buttonSelectRPM = new QPushButton("选择 rpm 包文件");
    buttonSelectRPM->setStyleSheet("QPushButton{color:#0099FF; background-color:transparent; font-size:12px}");
    buttonSelectRPM->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    buttonSelectRPM->setFixedWidth(200);

    connect(buttonSelectRPM, &QPushButton::clicked, this, &SelectPage::selectRpmBtnHandler);

    mainLayout->addWidget(dragDropArea, 0, Qt::AlignCenter);
    mainLayout->addWidget(dropHereText, 0, Qt::AlignCenter);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(splitLine, 0, Qt::AlignCenter);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(buttonSelectRPM, 0, Qt::AlignCenter);
    mainLayout->addStretch();

}

void SelectPage::selectRpmBtnHandler()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, "打开文件", QDir::homePath(), "*.rpm");
    bool isSelectSuccess = true;
    if(fileName.isEmpty()) {
        isSelectSuccess = false;
    }
    if(isSelectSuccess) {   // 选择成功的情况
        emit filenameReceived(fileName);
    } else {
        // 这里留给没选择 RPM 包的情况
    }
}
