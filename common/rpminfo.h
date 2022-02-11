#ifndef RPMINFO_H
#define RPMINFO_H
#include "common/datastructs.h"

#include <QObject>
#include <QString>
#include <QVariant>

class RPMInfo : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString dir READ dir)
    Q_PROPERTY(QString name READ name)
    Q_PROPERTY(QString arch READ arch)
    Q_PROPERTY(QString license READ license)
    Q_PROPERTY(QString versionRelease READ versionRelease)
    Q_PROPERTY(QString summary READ summary)
    Q_PROPERTY(QString description READ description)
    Q_PROPERTY(QStringList pkgProvides READ pkgProvides)
    Q_PROPERTY(QStringList pkgRequires READ pkgRequires)
    Q_PROPERTY(QString installedVersion READ installedVersion)
    Q_PROPERTY(QString statusDescription READ statusDescription)
    Q_PROPERTY(QString actionNotify READ actionNotify)

public:
    void setDir(const QString &dir) { m_dir = dir; }
    void setName(const QString &name) { m_name = name; }
    void setArch(const QString &arch) { m_arch = arch; }
    void setLicense(const QString &license) { m_license = license; }
    void setVersionRelease(const QString &versionRelease) { m_versionRelease = versionRelease; }
    void setSummary(const QString &summary) { m_summary = summary; }
    void setDescription(const QString &description) { m_description = description; }
    void setProvides(const QStringList pkgProvides) { m_pkgProvides = pkgProvides; }
    void setRequires(const QStringList pkgRequires) { m_pkgRequires = pkgRequires; }
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
    QStringList pkgProvides() const { return m_pkgProvides; }
    QStringList pkgRequires() const { return m_pkgRequires; }
    QString installedVersion() const { return m_installedVersion; }
    QString statusDescription() const { return m_statusDescription; }
    QString actionNotify() const { return m_actionNotify; }

private:
    QString m_dir;                    ///< 包目录位置
    QString m_name;                   ///< %{name}
    QString m_arch;                   ///< %{arch}
    QString m_license;                ///< %{license}
    QString m_versionRelease;         ///< ${version}-${release}
    QString m_summary;                ///< ${summary}
    QString m_description;            ///< %{description}

    QStringList m_pkgProvides;        ///< provides
    QStringList m_pkgRequires;        ///< requires

    QString m_installedVersion;       ///< 已安装的版本（当且仅当已有值，值不为空）

    PkgStatus m_status;               ///< 标记包的状态

    QString m_statusDescription;      ///< 对包状态的自然语言描述
    QString m_actionNotify;           ///< 描述将对包进行的操作(准备安装, 将不会安装等)
};

#endif // RPMINFO_H
