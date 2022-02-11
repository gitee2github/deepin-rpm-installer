#ifndef SINGLECONFIRMPAGE_H
#define SINGLECONFIRMPAGE_H

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
     * @brief go to Rpm select page.
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

private:
    /**
     * @brief init loading UI
     */
    void initLoadingUI();

    RPMInfoStruct rpm;                          ///< rpm info for this page
    QStackedLayout *container = nullptr;        ///< UI container
};

#endif // SINGLECONFIRMPAGE_H
