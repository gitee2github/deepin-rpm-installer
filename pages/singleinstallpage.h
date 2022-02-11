#ifndef SINGLECONFIRMPAGE_H
#define SINGLECONFIRMPAGE_H

#include "common/rpminfo.h"
#include "common/datastructs.h"

#include <QQuickView>
#include <QtQuickWidgets/QQuickWidget>

#include <QStackedLayout>
#include <QWidget>

class SingleInstallPage : public QWidget
{
    Q_OBJECT
public:
    explicit SingleInstallPage(QWidget *parent = nullptr);

    /**
     * @brief start rpm loading.
     */
    void loadRpm(RPMInfoStruct selectedRpm);
signals:
    /**
     * @brief go to Rpm select page.
     */
    void toRpmSelectPage();

public slots:
    /**
     * @brief init load success ui. called when rpm loaded.
     */
    void initLoadedUI();

    /**
     * @brief init load failed ui. called when rpm load failed.
     */
    void initLoadFailUI();

    /**
     * @brief append a new log line to log view.
     * 每次调用，更新的总 log 大字符串需要向 qml 同步
     */
    void appendOperationLog(QString logLine);

    /**
     * @brief update UI based on status. tirgged by working thread when operation finished.
     */
    void onOperationFinished(OperateMode mode, OperateFinishStatus finishStatus);

private:
    /**
     * @brief init loading UI
     */
    void initLoadingUI();

    /**
     * @brief start RPM Package operation ( install / uninstall / reinstall )
     */
    void startPkgOperation(OperateMode mode);


    RPMInfoStruct rpm;                          ///< rpm info for this page
    QStackedLayout *container = nullptr;        ///< UI container

    RPMInfo rpmInfo;
};

#endif // SINGLECONFIRMPAGE_H
