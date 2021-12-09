#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dragdropfilearea.h"
#include "installthread.h"
#include "pkgdetaildialog.h"

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

MainWindow::MainWindow(int argc, char **argv, QWidget *parent) :   // 直接通过构造器来搞
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("RPM 安装器");

    // 下面两行让窗口创建在屏幕正中间
    QDesktopWidget *desktop = QApplication::desktop();
    setFixedSize(550,350);
    move((desktop->width()-this->width())/2,(desktop->height()-this->height())/2);

    delete ui->mainToolBar;         // 不需要 toolbar ，删掉
    delete ui->statusBar;

    this->setArgc(argc);
    this->getArgc();
    if(argc==1) {
        // 没有启动参数输入，应该是啥也不干. 选择文件安装的界面应该在这里面绘制
        // TODO: 把这玩意抽出去，单独封装一下
        QWidget *mainContentWid = new QWidget;          // MainWindow 中的 Content，其实是个装 QVBoxLayout 的容器
                                                        // 这玩意的正确用法，或许应该是单拿出去做个类，和 main 拆开

        QVBoxLayout *mainLayout = new QVBoxLayout;      // 定义 QVBoxLayout
        mainContentWid->setLayout(mainLayout);          // 设置布局

        QPixmap *pixmap = new QPixmap();                // 多种格式的图片都可以存为 pixmap

        QSvgRenderer svgRender(QString(":/icon_install_light_resized.svg"));   // 使用 painter 和 renderer
        QImage image(200, 200, QImage::Format_ARGB32);
        image.fill(0x00FFFFFF);
        QPainter painter(&image);
        svgRender.render(&painter);

        pixmap->convertFromImage(image);

        DragDropFileArea *installIconLabel = new DragDropFileArea();
        installIconLabel->setPixmap(*pixmap);
        connect(installIconLabel, &DragDropFileArea::fileDropped, this, &MainWindow::dropFileHandler);

        QPixmap splitLinePixmap;
        splitLinePixmap.load(":/split_line.svg");
        QLabel * splitLine = new QLabel();
        splitLine->setPixmap(splitLinePixmap);

        QLabel *dropHereText = new QLabel();
        dropHereText->setText("拖拽 rpm 包到此");

        // 按钮尺寸相关见：https://blog.csdn.net/hyongilfmmm/article/details/83015729
        QPushButton *buttonSelectRPM = new QPushButton("选择 rpm 包文件");
        buttonSelectRPM->setStyleSheet("QPushButton{color:#0099FF; background-color:transparent; font-size:12px}");
        buttonSelectRPM->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        buttonSelectRPM->setFixedWidth(200);

        // 通过这种方式，把按钮的事件（clicked）和处理函数“连接”起来
        connect(buttonSelectRPM, &QPushButton::clicked, this, &MainWindow::selectRpmBtnHandler);

        mainLayout->addWidget(installIconLabel, 0, Qt::AlignCenter);
        mainLayout->addWidget(dropHereText, 0, Qt::AlignCenter);
        mainLayout->addSpacing(10);
        mainLayout->addWidget(splitLine, 0, Qt::AlignCenter);
        mainLayout->addSpacing(10);
        mainLayout->addWidget(buttonSelectRPM, 0, Qt::AlignCenter);
        mainLayout->addStretch();

        this->setCentralWidget(mainContentWid);

    } else {
        // 这里有参数输入了，先不管连续输入多个的情况，只管装第一个
        this->setArgPath(std::string(argv[1]));
        loadRpmInfo();
    }

}

MainWindow::~MainWindow()
{
    delete ui;
    delete installThread;
    installThread = nullptr;
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

std::string MainWindow::selectRpmBtnHandler()
{
    std::string res = "";
    QString fileName = QFileDialog::getOpenFileName(nullptr, "打开文件", QDir::homePath(), "*.rpm");
    bool isSelectSuccess = true;
    if(fileName.isEmpty()) {
        isSelectSuccess = false;
    }
    if(isSelectSuccess) {   // 选择成功的情况
        this->setArgPath(fileName.toStdString());
        loadRpmInfo();      // 开始加载信息，显示加载中界面
    } else {
        // 这里留给没选择 RPM 包的情况
    }

    return res;
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

void MainWindow::dropFileHandler(QString filename)
{
    this->setArgPath(filename.toStdString());
    loadRpmInfo();      // 开始加载信息，显示加载中界面
}

void MainWindow::exitOnFinished()
{
    QApplication::quit();
}
