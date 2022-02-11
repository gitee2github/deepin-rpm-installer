#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H
#ifndef STRUCTS_H
#define STRUCTS_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QVector>

/** @brief 标记 installThread 的工作模式。
 * InstallThread 的 run 函数中通过这个值来决定自己的工作模式，执行对应代码
 */
enum ThreadMode {
    getInfo,                        ///< 获取一个 rpm 包的信息
    installPackage                  ///< 安装一个 rpm 包
};

/** @brief 标记已加载的 RPM 包状态
 * 用于标记软件包的状态，根据该值确定界面显示及具体执行的功能
 */
enum PkgStatus {
    notParsed,                      ///< 该包刚刚添加，尚未解析
    parseErr,                       ///< 解析错误
    readyToInstall,                 ///< 通过检查，可以安装
    higherVerInstalled,             ///< 已安装更高版本
    sameVerInstalled,               ///< 已安装相同版本
    differVerInstalled,             ///< 已安装不同版本
    lowerVerInstalled,              ///< 已安装更低版本
    wrongArch,                      ///< 所选包架构与设备不符
    installSuccess,                 ///< 已安装，安装成功
    installFailed                   ///< 安装失败
};

/** @brief 标记要进行的操作模式
 * 安装、卸载、重装三个功能，都是由界面提交给工作线程去做的。界面将该值传给工作线程，工作线程根局该值决定工作模式
 */
enum OperateMode {
    install,
    uninstall,
    reinstall
};

/** @brief 标记操作完成的结果，由工作线程传回界面，构造相应界面
 */
enum OperateFinishStatus {
    success,                        ///< 操作成功
    error                           ///< 操作失败
};

struct RPMInfoStruct {
    QString dir;                    ///< 包目录位置
    QString name;                   ///< %{name}
    QString arch;                   ///< %{arch}
    QString license;                ///< %{license}
    QString versionRelease;         ///< ${version}-${release}
    QString summary;                ///< ${summary}
    QString description;            ///< %{description}

    QString installed;              ///< 是否已安装。用于兼容原单包安装代码。新代码中已用 status 代替

    QVector<QString> pkgProvides;   ///< provides
    QVector<QString> pkgRequires;   ///< requires

    QString installedVersion;       ///< 已安装的版本（当且仅当已有值，值不为空）

    PkgStatus status;               ///< 标记包的状态
    QString statusDescription;      ///< 对包状态的自然语言描述
    QString actionNotify;           ///< 描述将对包进行的操作(准备安装, 将不会安装等)

};

Q_DECLARE_METATYPE(RPMInfoStruct)


#endif

#endif // DATASTRUCTS_H
