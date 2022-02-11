#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pages/rpmselectpage.h"
#include "pages/singleinstallpage.h"

#include "common/datastructs.h"
#include "common/installthread.h"

#include <QMainWindow>
#include <QLabel>
#include <iostream>
#include <string>
#include <QString>
#include <QVector>
#include <QTextEdit>
#include <QStackedWidget>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(int argc, char **argv, QWidget *parent = nullptr);
    ~MainWindow();

    bool setArgc(int argsC);
    int getArgc();

    bool setArgPath(std::string);
    std::string getArgPath();

    void callShell();
    void sendNotify();

    void installRPM();

    QString getRPMSummary();

    void loadRpmInfo();

private:
    QStackedWidget *pages = nullptr;                    ///< 所有界面的容器

    RpmSelectPage *rpmSelectPage = nullptr;             ///< RPM 选择界面。在无参数输入启动、安装完成后返回时显示该页面

    SingleInstallPage *singleInstallPage = nullptr;     ///< 单包安装界面。在单参数启动时显示该页面

    QLabel *helloLabel;
    int argCount;
    std::string argPath;

    QTextEdit *textArea;

    QVector<RPMInfoStruct> rpmArray;

    InstallThread *installThread = nullptr;

    /**
     * @brief init UI for installer.
     *
     * 该函数会初始化 pages ，装入 RpmSelectPage ，但不会直接显示出来
     */
    void initUI();

public slots:
    /**
     * @brief Display RpmSelectPage
     */
    void toRpmSelectPage();

    /**
     * @brief Display SingleInstallPage
     */
    void toSingleInstallPage(RPMInfoStruct selectedRpm);



    void onRPMInfoLoaded(QVector<RPMInfoStruct> rpmArray);

    void appendLog(QString log);

    void onInstallFinish();

    void updateRPMArray(QVector<RPMInfoStruct> rpmArray);

    void exitOnFinished();

    void showMoreInfoDialog();

    void gotRpmHandler(QString filename);
};

#endif // MAINWINDOW_H
