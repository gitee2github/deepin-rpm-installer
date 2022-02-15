#include "singleinstallpage.h"

#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickWidget>

#include <QStringList>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

SingleInstallPage::SingleInstallPage(QWidget *parent) : QWidget(parent)
{
    qmlRegisterType<RPMInfo>("org.deepin.rpminstaller", 1, 0, "RPMInfo");

    container = new QStackedLayout(this);
    initLoadingUI();
}

void SingleInstallPage::loadRpm(RPMInfoStruct selectedRpm)
{
    rpmInfo.setName("abc");
    rpmInfo.setVersionRelease("1.0.0-1.up1");
    rpmInfo.setSummary("Something not so long summary, Something not so long summary, Something not so long summary, Something not so long summary, Something not so long summary, ");
    rpmInfo.setArch("aarch64");
    rpmInfo.setLicense("WTFPL");
    rpmInfo.setDescription("Something really looog description, Something really looog description, Something really looog description, Something really looog description, Something really looog description, Something really looog description, Something really looog description, Something really looog description, Something really looog description, Something really looog description, Something really looog description, Something really looog description, Something really looog description, Something really looog description, Something really looog description, Something really looog description, Something really looog description, Something really looog description, Something really looog description, Something really looog description, Something really looog description, Something really looog description, Something really looog description, Something really looog description, Something really looog description, Something really looog description, Something really looog description, Something really looog description, Something really looog description, Something really looog description,");
    rpmInfo.setRequires("a\nb\nc\nd\ne\nf\ng\nh\ni\nj");
    rpmInfo.setProvides("a\nb\nc\nd\ne\nf\ng\nh\ni\nj");
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
    loadedWidget->engine()->rootContext()->setContextProperty("rpmInfo", &rpmInfo);
    loadedWidget->engine()->rootContext()->setContextProperty("installPage", this);
    container->addWidget(loadedWidget);
    container->setCurrentIndex(1);
}

void SingleInstallPage::initLoadFailUI()
{

}

void SingleInstallPage::startPkgOperation(QString mode)
{
    qDebug("Run!");
    qDebug("%s", qPrintable(mode));
}

void SingleInstallPage::appendOperationLog(QString logLine)
{

}

void SingleInstallPage::onOperationFinished(OperateMode mode, OperateFinishStatus finishStatus)
{

}

