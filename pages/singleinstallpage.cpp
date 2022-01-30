#include "singleinstallpage.h"

#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

SingleInstallPage::SingleInstallPage(QWidget *parent) : QWidget(parent)
{
    container = new QStackedLayout(this);
    initLoadingUI();
}

void SingleInstallPage::loadRpm(RPMInfoStruct selectedRpm)
{
    rpm.name = "pkgName";
    rpm.versionRelease = "1.0.0-1.up1";
    rpm.summary = "ldfs akjflad sjifa sd\nfslkdj flsjdlf\nf dslkjfs ldkjfklsdj flkjsld fjkls djflf dslkjfs ldkjfklsdj flkjsld fjkls djfl";
    rpm.status = PkgStatus::sameVerInstalled;
}

void SingleInstallPage::initLoadingUI()
{
    QPushButton *dbg = new QPushButton("makeLoaded");
    connect(dbg, &QPushButton::clicked, this, &SingleInstallPage::initLoadedUI);

    QWidget *loadingWidget = new QWidget();
    QVBoxLayout *loadingLayout = new QVBoxLayout(loadingWidget);
    QLabel *loadingText = new QLabel(tr("Loading RPM"));
    loadingLayout->addStretch();
    loadingLayout->addWidget(loadingText, 0, Qt::AlignCenter);
    loadingLayout->addWidget(dbg, 0, Qt::AlignCenter);
    loadingLayout->addStretch();

    container->addWidget(loadingWidget);
    container->setCurrentIndex(0);
}

void SingleInstallPage::initLoadedUI()
{
    QQuickWidget *loadedWidget = new QQuickWidget(QUrl("qrc:/SingleInstallUI.qml"));
    // qmlWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);    // 设置后，无需设置 anchor，root 节点会根局 QQuickWidget 大小改变
    container->addWidget(loadedWidget);
    container->setCurrentIndex(1);
}

void SingleInstallPage::initLoadFailUI()
{

}


