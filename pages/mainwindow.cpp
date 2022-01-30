#include "pages/mainwindow.h"
#include "pages/pkgdetaildialog.h"
#include "pages/selectpage.h"

#include "common/dragdropfilearea.h"
#include "common/installthread.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <string>
#include <QProcess>
#include <QStringList>
#include <QSvgRenderer>
#include <QPainter>
#include <QTextEdit>

MainWindow::MainWindow(int argc, char **argv, QWidget *parent)
    : QMainWindow(parent)
{
    initUI();
    if(argc==1) {                                 // 无参数启动，初始化文件选择界面
        toRpmSelectPage();
    } else {                                      // 有参数启动，目前无视连续输入多个的情况，只会处理第一个
        RPMInfoStruct tmp;
        tmp.dir = QString(argv[1]);
        toSingleInstallPage(tmp);
    }
    QDesktopWidget *desktop = QApplication::desktop();
    move((desktop->width()-this->width())/2,(desktop->height()-this->height())/2);
}

MainWindow::~MainWindow()
{
    delete installThread;
    installThread = nullptr;
}

void MainWindow::initUI()
{
    setStyleSheet("background-color:#F8F8F8");
    setWindowTitle(tr("Deepin RPM Installer"));

    pages = new QStackedWidget();
    rpmSelectPage = new RpmSelectPage();
    singleInstallPage = new SingleInstallPage();
    pages->addWidget(rpmSelectPage);                            // index: 0
    pages->addWidget(singleInstallPage);                        // index: 1
    connect(rpmSelectPage, &RpmSelectPage::toSingleInstallPage, this, &MainWindow::toSingleInstallPage);
    connect(singleInstallPage, &SingleInstallPage::toRpmSelectPage, this, &MainWindow::toRpmSelectPage);

    setCentralWidget(pages);
}

void MainWindow::toRpmSelectPage()
{
    pages->setCurrentIndex(0);
    setFixedSize(500,350);
}

void MainWindow::toSingleInstallPage(RPMInfoStruct selectedRpm)
{
    pages->setCurrentIndex(1);
    singleInstallPage->loadRpm(selectedRpm);

    setFixedSize(700,500);
    QDesktopWidget *desktop = QApplication::desktop();
    move((desktop->width()-this->width())/2,(desktop->height()-this->height())/2);
}


bool MainWindow::setArgc(int argsC)
{
    this->argCount=argsC;
    return true;
}

int MainWindow::getArgc()
{
    qDebug("well:%d", this->argCount);
    return this->argCount;
}

bool MainWindow::setArgPath(std::string argPath)
{
    this->argPath=argPath;
    return true;
}

std::string MainWindow::getArgPath()
{
    qDebug("this->argPath = %s", this->argPath.c_str());
    return this->argPath;
}

void MainWindow::sendNotify()
{
    QProcess process;
    process.start("notify-send", QStringList() << QString(this->argPath.c_str()));
    process.waitForFinished();

    QString res = process.readAllStandardOutput();
    qDebug("%s", qPrintable(res));
}

void MainWindow::installRPM()
{
    qDebug("=====begin install=====");
    QWidget *installingWid = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout;
    installingWid->setLayout(mainLayout);
    QLabel *installingText = new QLabel("安装中，日志如下");

    textArea = new QTextEdit();
    textArea->setReadOnly(true);
    textArea->setFixedWidth(500);
    textArea->setFixedHeight(230);

    mainLayout->addStretch();
    mainLayout->addWidget(installingText, 0, Qt::AlignCenter);
    mainLayout->addSpacing(5);
    mainLayout->addWidget(textArea, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    setCentralWidget(installingWid);

    installThread = new InstallThread(ThreadMode::installPackage, rpmArray);
    installThread->start();

    connect(installThread, &InstallThread::appendLog, this, &MainWindow::appendLog);
    connect(installThread, &InstallThread::updatePkgInfo, this, &MainWindow::updateRPMArray);
    connect(installThread, &InstallThread::installFinish, this, &MainWindow::onInstallFinish);

}

void MainWindow::updateRPMArray(QVector<RPMInfoStruct> rpmArray)
{
    this->rpmArray = rpmArray;
}

void MainWindow::onInstallFinish()
{
    qDebug("=====finished=====");
    QWidget *installedWid = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout;
    installedWid->setLayout(mainLayout);
    QLabel *installedText = new QLabel(rpmArray[0].actionNotify);
    QPushButton *finishBtn = new QPushButton("完成");
    finishBtn->setFixedWidth(200);
    connect(finishBtn, &QPushButton::clicked, this, &MainWindow::exitOnFinished);

    mainLayout->addStretch();
    mainLayout->addWidget(installedText, 0, Qt::AlignCenter);
    mainLayout->addSpacing(15);
    mainLayout->addWidget(textArea, 0, Qt::AlignCenter);
    mainLayout->addSpacing(15);
    mainLayout->addWidget(finishBtn, 0, Qt::AlignCenter);
    mainLayout->addStretch();
    setCentralWidget(installedWid);
}

void MainWindow::appendLog(QString log)
{
    if(log.isEmpty()) return;
    textArea->append(log);
    textArea->moveCursor(QTextCursor::End);
}

QString MainWindow::getRPMSummary()
{
    QString summary = QString("");
    std::string rpmInfoStr = "";
    QProcess process;
    process.start("rpm", QStringList() << "-qip" << QString(this->argPath.c_str()));
    process.waitForFinished();
    rpmInfoStr = process.readAllStandardOutput().toStdString();
    size_t summaryIndex = rpmInfoStr.find("\nSummary");
    size_t descriptionIndex = rpmInfoStr.find("Description")-1;

    rpmInfoStr = rpmInfoStr.substr(summaryIndex+1, descriptionIndex-summaryIndex);
    rpmInfoStr = rpmInfoStr.substr(rpmInfoStr.find(": ")+2);
    std::string::iterator infoEnd = rpmInfoStr.end();

    // 解决可能存在的，Summary 长度超过一行的情况（应该不会出现吧
    for(std::string::iterator iter=rpmInfoStr.begin(); iter!=infoEnd; iter++) {
        if(*iter=='\n') *iter=' ';
    }

    summary = QString::fromStdString(rpmInfoStr);
    qDebug("%d", summary.length());
    qDebug("%s", qPrintable(summary));

    return summary;
}

void MainWindow::loadRpmInfo()
{
    QWidget *loadingWid = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout;
    loadingWid->setLayout(mainLayout);
    QLabel *loadingLabel = new QLabel("加载中，请稍候");
    mainLayout->addStretch();
    mainLayout->addWidget(loadingLabel, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    setCentralWidget(loadingWid);

    RPMInfoStruct tmp;
    tmp.dir = QString::fromStdString(argPath);
    rpmArray.push_back(tmp);

    installThread = new InstallThread(ThreadMode::getInfo, rpmArray);
    installThread->start();
    connect(installThread, &InstallThread::multiPkgInfo, this, &MainWindow::onRPMInfoLoaded);

}

void MainWindow::onRPMInfoLoaded(QVector<RPMInfoStruct> rpmArray) {
    delete installThread;
    installThread = nullptr;

    this->rpmArray = rpmArray;

    QWidget *mainContentWid = new QWidget;          // MainWindow 中的 Content，其实是个装 QVBoxLayout 的容器
    QVBoxLayout *mainLayout = new QVBoxLayout;      // 定义 QVBoxLayout
    mainContentWid->setLayout(mainLayout);          // 设置布局

    QPixmap pixmap;

    QSvgRenderer svgRender(QString(":/rpm.svg"));   // 使用 painter 和 renderer
    QImage image(120, 120, QImage::Format_ARGB32);
    image.fill(0x00FFFFFF);
    QPainter painter(&image);
    svgRender.render(&painter);

    pixmap.convertFromImage(image);

    QLabel *installIconLabel = new QLabel();
    installIconLabel->setFixedSize(QSize(120, 120));
    installIconLabel->setPixmap(pixmap);

    QLabel *titleText = new QLabel();
    titleText->setText(this->rpmArray[0].name);
    titleText->setFixedWidth(400);

    QLabel *rpmInfoText = new QLabel();
    rpmInfoText->setText(this->rpmArray[0].summary);
    rpmInfoText->setStyleSheet("QLabel{font-size:12px}");
    rpmInfoText->setFixedWidth(400);
    rpmInfoText->setWordWrap(true);

    QPushButton *buttonInstallRPM = new QPushButton();
    buttonInstallRPM->setFixedWidth(200);

    QPushButton *pkgDetailBtn = new QPushButton();
    pkgDetailBtn->setText("查看详细信息");
    pkgDetailBtn->setFixedWidth(150);
    pkgDetailBtn->setStyleSheet("QPushButton{color:#0099FF; background-color:transparent; padding:0px; border; focus{border:none; outline: none;}}");
    connect(pkgDetailBtn, &QPushButton::clicked, this, &MainWindow::showMoreInfoDialog);
    mainLayout->addWidget(installIconLabel, 0, Qt::AlignCenter);
    mainLayout->addWidget(titleText, 0, Qt::AlignCenter);
    mainLayout->addWidget(rpmInfoText, 0, Qt::AlignCenter);
    mainLayout->addStretch();
    mainLayout->addWidget(pkgDetailBtn, 0, Qt::AlignCenter);

    if(rpmArray[0].status==readyToInstall) {
        buttonInstallRPM->setText("开始安装");
        connect(buttonInstallRPM, &QPushButton::clicked, this, &MainWindow::installRPM);
    } else {
        QLabel *cannotInstallDescription = new QLabel(this->rpmArray[0].statusDescription+"   "+this->rpmArray[0].actionNotify);
        cannotInstallDescription->setStyleSheet("QLabel{font-size:16px; color: green}");
        buttonInstallRPM->setText("完成");
        connect(buttonInstallRPM, &QPushButton::clicked, this, &MainWindow::exitOnFinished);
        mainLayout->addStretch();
        mainLayout->addWidget(cannotInstallDescription, 0, Qt::AlignCenter);
    }
    mainLayout->addStretch();
    mainLayout->addWidget(buttonInstallRPM, 0, Qt::AlignCenter);
    mainLayout->addStretch();
    this->setCentralWidget(mainContentWid);

}

void MainWindow::showMoreInfoDialog()
{
    PkgDetailDialog *Dialog = new PkgDetailDialog(this);
    Dialog->initData(rpmArray[0]);
    Dialog->initUI();
    Dialog->setModal(true);
    Dialog->show();
}

void MainWindow::gotRpmHandler(QString filename)
{
    this->setArgPath(filename.toStdString());
    loadRpmInfo();
}

void MainWindow::exitOnFinished()
{
    QApplication::quit();
}
