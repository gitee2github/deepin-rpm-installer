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


class RPMInfo : public QObject
{
    Q_OBJECT
public:
    void setDir(const QString &dir) { m_dir = dir; }
    void setName(const QString &name) { m_name = name; }
    void setArch(const QString &arch) { m_arch = arch; }
    void setLicense(const QString &license) { m_license = license; }
    void setVersionRelease(const QString &versionRelease) { m_versionRelease = versionRelease; }
    void setSummary(const QString &summary) { m_summary = summary; }
    void setDescription(const QString &description) { m_description = description; }
    void setProvides(const QVector<QString> pkgProvides) { m_pkgProvides = pkgProvides; }
    void setRequires(const QVector<QString> pkgRequires) { m_pkgRequires = pkgRequires; }
    void setInstalledVersion(const QString &installedVersion) { m_installedVersion = installedVersion; }
    void setStatus(const PkgStatus &status) { m_status = status; }
    void setStatusDescription(const QString &statusDescription) { m_statusDescription = statusDescription; }
    void setActionNotify(const QString &actionNotify) { m_actionNotify = actionNotify; }

    QString dir() const { return m_dir; }
    QString name() const { return m_name; }
    QString arch() const { return m_arch; }
    QString license() const { return m_license; }
    QString versionRelease() const { return m_versionRelease; }
    QString summary() const { return m_summary; }
    QString description() const { return m_description; }


private:
    QString m_dir;                    ///< 包目录位置
    QString m_name;                   ///< %{name}
    QString m_arch;                   ///< %{arch}
    QString m_license;                ///< %{license}
    QString m_versionRelease;         ///< ${version}-${release}
    QString m_summary;                ///< ${summary}
    QString m_description;            ///< %{description}

    QString m_installed;              ///< 是否已安装。用于兼容原单包安装代码。新代码中已用 status 代替

    QVector<QString> m_pkgProvides;   ///< provides
    QVector<QString> m_pkgRequires;   ///< requires

    QString m_installedVersion;       ///< 已安装的版本（当且仅当已有值，值不为空）

    PkgStatus m_status;               ///< 标记包的状态
    QString m_statusDescription;      ///< 对包状态的自然语言描述
    QString m_actionNotify;           ///< 描述将对包进行的操作(准备安装, 将不会安装等)
};


#endif

#endif // DATASTRUCTS_H
