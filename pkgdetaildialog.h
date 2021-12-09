#ifndef PKGDETAILDIALOG_H
#define PKGDETAILDIALOG_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>

#include <QVector>

#include "datastructs.h"

class PkgDetailDialog : public QDialog
{
    Q_OBJECT
public:
    PkgDetailDialog(QWidget *parent = nullptr);
    ~PkgDetailDialog();

    void initData(RPMInfoStruct rpmInfo);

    void initUI();

private:
    /**
     * @brief 为 content 加上标题样式
     * @param content 要被加样式的内容
     */
    QString headerStyle(QString content);

    /**
     * @brief 为 content 加上次级标题样式
     * @param content 要被加样式的内容
     */
    QString subHeaderStyle(QString content);

    /**
     * @brief 为 content 加上带左缩进的样式
     * @param content 要被加缩进的内容
     */
    QString marginStyle(QString content);

    /**
     * @brief 为 content 加上文本块风格的样式
     * @param content 要被加样式的内容
     */
    QString docblockStyle(QString content);

    /**
     * @brief 将字符串向量变为带样式的 html 文本, 每个向量元素一行
     * @param list 列表
     */
    QString styledList(QVector<QString> list);

    RPMInfoStruct *rpmInfo = nullptr;

    QWidget *viewport = nullptr;
    QScrollArea *mainScrollArea = nullptr;

    QVBoxLayout *mainLayout = nullptr;
    QVBoxLayout *dialogLayout = nullptr;

    QLabel *name = nullptr;
    QLabel *archLabel = nullptr;
    QLabel *arch = nullptr;
    QLabel *licenseLabel = nullptr;
    QLabel *license = nullptr;
    QLabel *summaryLabel = nullptr;
    QLabel *summary = nullptr;
    QLabel *descriptionLabel = nullptr;
    QLabel *description = nullptr;
    QLabel *versionReleaseLabel = nullptr;
    QLabel *versionRelease = nullptr;

    QLabel *requiresLabel = nullptr;
    QLabel *requiresArea = nullptr;
    QScrollArea *requiresScrollArea = nullptr;
    QLabel *providesLabel = nullptr;
    QLabel *providesArea = nullptr;
    QScrollArea *providesScrollArea = nullptr;
};

#endif // PKGDETAILDIALOG_H
