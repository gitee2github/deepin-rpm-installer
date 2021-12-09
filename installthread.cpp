#include "installthread.h"

InstallThread::InstallThread(ThreadMode mode, QVector<RPMInfoStruct> rpmArray)
{
    qRegisterMetaType<QVector<RPMInfoStruct>>("QVector<rpmInfoStruct>");
    thMode = mode;
    this->rpmArray = rpmArray;
    getSystemArch();
}

InstallThread::~InstallThread()
{
    delete installProcess;
    installProcess = nullptr;
}

void InstallThread::run()
{
    installProcess = nullptr;

    switch (thMode) {
        case ThreadMode::getInfo:
            getInfo();
            break;
        case ThreadMode::installPackage:
            installPackage();
            break;
    }
}

void InstallThread::getSystemArch()
{
    QProcess getArchProcess;
    getArchProcess.start("arch");
    getArchProcess.waitForFinished();
    systemArch = getArchProcess.readAllStandardOutput();
    systemArch.chop(1);
}

void InstallThread::getInfo()
{
    QProcess getInfoProcess;
    QStringList paramList;
    paramList = QStringList() << "-mode"
                              << "getInfo"
                              << "-pkgs";
    for(int i=0; i<rpmArray.length(); i++) {
        qDebug("%s", qPrintable(rpmArray[i].dir));
        paramList << rpmArray[i].dir;
    }
    getInfoProcess.start("rpminstaller-dnfpy", paramList);
    getInfoProcess.waitForFinished(-1);

    QString res = getInfoProcess.readAllStandardOutput();
    qDebug("%s", qPrintable(res));

    parseJson(res);

    emit multiPkgInfo(rpmArray);
}

void InstallThread::parseJson(QString jsonStr)
{
    QVector<RPMInfoStruct> emptyArr;
    rpmArray.swap(emptyArr);            // 清空原变量
    bool existAlreadyAdded = false;     // 标记是否存在多次输入同一个包的情况, 若有则最后给个 notify

    QJsonDocument jdoc = QJsonDocument::fromJson(QByteArray::fromStdString(jsonStr.toStdString()));
    QJsonArray arr = jdoc.array();
    int arrSize = arr.size();

    for(int i=0; i<arrSize; i++) {
        QJsonObject obj = arr.at(i).toObject();

        if(!obj.value("name").toString().contains("解析失败") && rpmPkgNameSet.contains(obj.value("name").toString())) {
            existAlreadyAdded = true;
            continue;                               // 已经添加过, 直接进入新循环
        }

        rpmPkgNameSet.insert(obj.value("name").toString());     // 没添加过，加入集合

        RPMInfoStruct tmpStruct;

        tmpStruct.name = obj.value("name").toString();
        tmpStruct.dir = obj.value("dir").toString();
        if(tmpStruct.name.contains("解析失败")) {
            tmpStruct.status = parseErr;
            tmpStruct.statusDescription = tmpStruct.dir;
            tmpStruct.actionNotify = "将不会安装";
            rpmArray.push_back(tmpStruct);
            continue;
        }

        tmpStruct.arch = obj.value("arch").toString();
        tmpStruct.license = obj.value("license").toString();
        tmpStruct.versionRelease = obj.value("rpmVersionRelease").toString();
        tmpStruct.summary = obj.value("summary").toString();
        tmpStruct.description = obj.value("description").toString();
        tmpStruct.installed = obj.value("installed").toString();
        tmpStruct.installedVersion = obj.value("installedVersionRelease").toString();

        QJsonArray providesArr = obj.value("provides").toArray();
        int providesArrSize = providesArr.size();
        for(int j=0; j<providesArrSize; j++) {
            QString tmpProvidesName = providesArr.at(j).toString();
            tmpStruct.pkgProvides.push_back(tmpProvidesName);
        }

        QJsonArray requiresArr = obj.value("requires").toArray();
        int requiresSize = requiresArr.size();
        for(int j=0; j<requiresSize; j++) {
            QString tmpRequiresName = requiresArr.at(j).toString();
            tmpStruct.pkgRequires.push_back(tmpRequiresName);
        }

        bool isArchOk = false;
        // on x86 machine
        if(systemArch.contains("86") && (tmpStruct.arch.contains("86") || tmpStruct.arch.contains("noarch")) ) {
            isArchOk = true;
        }
        // on aarch64 machine
        if(systemArch.contains("aarch64") && (tmpStruct.arch.contains("armv7") || tmpStruct.arch.contains("aarch64") || tmpStruct.arch.contains("noarch"))) {
            isArchOk = true;
        }
        if(!isArchOk) {
            tmpStruct.status = wrongArch;
            tmpStruct.statusDescription = "软件包架构与本机不符";
            tmpStruct.actionNotify = "将不会安装";
            rpmArray.push_back(tmpStruct);
            continue;
        }

        if(tmpStruct.installedVersion.contains("None")) {
            tmpStruct.status = readyToInstall;
            tmpStruct.statusDescription = tmpStruct.summary;
            tmpStruct.actionNotify = "准备安装";
        } else {
            if(tmpStruct.versionRelease==tmpStruct.installedVersion) {
                tmpStruct.status = sameVerInstalled;
                tmpStruct.statusDescription = "已安装相同版本";
            } else {
                tmpStruct.status = differVerInstalled;
                tmpStruct.statusDescription = QString("已安装版本:")+tmpStruct.installedVersion;
            }
            tmpStruct.actionNotify = "将不会安装";
        }
        rpmArray.push_back(tmpStruct);
    }
}

void InstallThread::installPackage()
{
    for (int i=0; i<rpmArray.length(); i++) {
        qDebug("=====TAG 1=====");
        if(willExit) break;
        qDebug("=====TAG 2=====");
        installProcess = new QProcess();
        connect(installProcess, &QProcess::readyReadStandardOutput, this, &InstallThread::onReadData);
        installProcess->setProcessChannelMode(QProcess::MergedChannels);
        installProcess->start("pkexec", QStringList()
                              << "rpminstaller-dnfpy"
                              << "-mode"
                              << "install"
                              << "-pkgname"
                              << rpmArray[i].dir);
        installProcess->waitForFinished(-1);

        if(lastLog=="success") {
            rpmArray[i].actionNotify = "安装成功";
        } else {
            rpmArray[i].actionNotify = "安装失败";
        }
    }
    emit updatePkgInfo(rpmArray);
    emit installFinish();
}

void InstallThread::onReadData()
{
    if(willExit) return;
    installProcess->setReadChannel(QProcess::StandardOutput);
    QTextCodec* utfCodec = QTextCodec::codecForName("UTF-8");
    QByteArray tmpArr = installProcess->readAll();

    if(tmpArr.isEmpty()) {       // 输出为空时只会徒增空行, 没有续日志的必要
        return;
    }

    QString result = utfCodec->toUnicode(tmpArr);
    qDebug("%s", qPrintable(result));

    if(result.end()->isNull()) {
        result = result.remove(result.length()-1, 1);
    }

    if(result.contains("Not authorized") || result.contains("Request dismissed")) {   // 只在确认认证失败的情况下才可以运行, 其他的仍然往后加log
        qDebug("=====pkexec fail=====");
        willExit = true;
        emit onProcessErr();
    }

    if(result.contains("success")) {
        lastLog="success";
    } else if(result.contains("failed")) {
        lastLog="failed";
    }

    emit appendLog(result);
}
