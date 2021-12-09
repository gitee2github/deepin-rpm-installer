#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <iostream>
#include <string>
#include <QString>
#include <QVector>
#include <QTextEdit>

#include "datastructs.h"
#include "installthread.h"

namespace Ui {
class MainWindow;
}

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

    std::string selectRpmBtnHandler();

    void callShell();
    void sendNotify();

    void installRPM();

    QString getRPMSummary();

    void loadRpmInfo();

private:
    Ui::MainWindow *ui;
    QLabel *helloLabel;         // 该类的实例，也就是主窗口，应该可以用这玩意
    int argCount;
    std::string argPath;

    QTextEdit *textArea;

    QVector<RPMInfoStruct> rpmArray;

    InstallThread *installThread = nullptr;

public slots:
    void dropFileHandler(QString filename);

    void onRPMInfoLoaded(QVector<RPMInfoStruct> rpmArray);

    void appendLog(QString log);

    void onInstallFinish();

    void updateRPMArray(QVector<RPMInfoStruct> rpmArray);

    void exitOnFinished();

    void showMoreInfoDialog();
};

#endif // MAINWINDOW_H
