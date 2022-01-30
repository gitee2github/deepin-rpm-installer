#include "rpmselectpage.h"

#include "common/consts.h"
#include "common/datastructs.h"

#include <QFileDialog>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QSvgRenderer>
#include <QPainter>

RpmSelectPage::RpmSelectPage(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void RpmSelectPage::initUI()
{
    QSvgRenderer *svgRender = new QSvgRenderer();
    svgRender->load(QString(":/icon_install_light_resized.svg"));
    QPixmap *pixmap = new QPixmap(150, 150);
    pixmap->fill(Qt::transparent);
    QPainter p(pixmap);
    svgRender->render(&p);

    QLabel *label = new QLabel();
    label->setPixmap(*pixmap);
    label->setFixedSize(QSize(150, 150));

    QLabel *notice = new QLabel(tr("Drop RPM file here"));
    notice->setStyleSheet(smallTextStyle);

    QPushButton *selectFileBtn = new QPushButton("Select File");
    selectFileBtn->setFixedSize(QSize(160, 30));
    selectFileBtn->setStyleSheet(normalBtnStyle);
    connect(selectFileBtn, &QPushButton::clicked, this, &RpmSelectPage::selectBtnHandler);

    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    mainlayout->addSpacing(50);
    mainlayout->addWidget(label, 0, Qt::AlignCenter);
    mainlayout->addSpacing(10);
    mainlayout->addWidget(notice, 0, Qt::AlignCenter);
    mainlayout->addSpacing(30);
    mainlayout->addWidget(selectFileBtn, 0, Qt::AlignCenter);
    mainlayout->addSpacing(50);
}

void RpmSelectPage::selectBtnHandler()
{
    QStringList filenames = QFileDialog::getOpenFileNames(this, tr("Select RPM files"), QDir::homePath(), "*.rpm;;");

    QVector<RPMInfoStruct> RPMArray;
    for(int i=0; i<filenames.length(); i++) {
        RPMInfoStruct tmp;
        tmp.dir = filenames[i];
        RPMArray.push_back(tmp);
    }

    if(filenames.length()==0) return;
    if(filenames.length()==1) {
        emit toSingleInstallPage(RPMArray[0]);
        return;
    }
    // 这里后续增加一个到多包安装界面的信号，用于对应的跳转
}
