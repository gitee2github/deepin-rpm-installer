#ifndef INSTALLTHREAD_H
#define INSTALLTHREAD_H

#include "datastructs.h"

#include <QObject>
#include <QThread>
#include <QProcess>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QSet>
#include <QTextCodec>

class InstallThread : public QThread
{
    Q_OBJECT
public:
    InstallThread(ThreadMode mode, QVector<RPMInfoStruct> rpmArray);
    ~InstallThread();

    void run();

signals:
    void multiPkgInfo(QVector<RPMInfoStruct> rpmArray);
    void installFinish();
    void onProcessErr();
    void appendLog(QString result);

    void updatePkgInfo(QVector<RPMInfoStruct> rpmArray);

public slots:


private:
    /**
     * @brief 终端输出的处理函数。每次终端输出新内容，都会调用该函数
     */
    void onReadData();

    /**
     * @brief 获取 rpm 包的信息，包路径及返回的信息均存在本对象的 RPMArray 中
     */
    void getInfo();

    /**
     * @brief 安装 rpm 包
     */
    void installPackage();

    /**
     * @brief 获取系统架构
     */
    void getSystemArch();

    /**
     * @brief 解析 json
     */
    void parseJson(QString jsonStr);

    ThreadMode thMode;
    QProcess *installProcess = nullptr;
    bool willExit = false;
    QString lastLog;

    QVector<RPMInfoStruct> rpmArray;
    QSet<QString> rpmPkgNameSet;

    QString systemArch;                 ///< 当前系统的架构
};

#endif // INSTALLTHREAD_H
