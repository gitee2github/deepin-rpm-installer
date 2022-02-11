#ifndef RPMSELECTPAGE_H
#define RPMSELECTPAGE_H

#include "common/datastructs.h"

#include <QWidget>

class RpmSelectPage : public QWidget
{
    Q_OBJECT
public:
    explicit RpmSelectPage(QWidget *parent = nullptr);

signals:
    /**
     * @brief single rpm selected, to single install page.
     */
    void toSingleInstallPage(RPMInfoStruct selectedRpm);

public slots:
    /**
     * @brief handle file select button click event. open filedialog and process result
     */
    void selectBtnHandler();

private:
    /**
     * @brief init UI for this page. should be invoked by constructor
     */
    void initUI();


};

#endif // RPMSELECTPAGE_H
