#include "pkgdetaildialog.h"

PkgDetailDialog::PkgDetailDialog(QWidget *parent)
    : QDialog(parent)
{
    rpmInfo = nullptr;
    resize(QSize(650, 750));
    // setAttribute(Qt::WA_DeleteOnClose, true);
    setContentsMargins(20, 10, 20, 10);
    setWindowTitle("RPM 包详细信息");
}

PkgDetailDialog::~PkgDetailDialog()
{
    delete rpmInfo;
    delete requiresArea;
    delete providesArea;

    rpmInfo = nullptr;
    requiresArea = nullptr;
    providesArea = nullptr;
    qDebug("PkgInfoDialog destroy");
}

void PkgDetailDialog::initData(RPMInfoStruct _rpmInfo)
{
    delete rpmInfo;
    qDebug("name: %s", qPrintable(_rpmInfo.name));
    rpmInfo = new RPMInfoStruct();
    *rpmInfo = _rpmInfo;
}

void PkgDetailDialog::initUI()
{
    mainLayout = new QVBoxLayout(this);

    archLabel = new QLabel();
    licenseLabel = new QLabel();
    summaryLabel = new QLabel();
    descriptionLabel = new QLabel();
    versionReleaseLabel = new QLabel();
    requiresLabel = new QLabel();
    providesLabel = new QLabel();

    name = new QLabel();
    arch = new QLabel();
    license = new QLabel();
    summary = new QLabel();
    description = new QLabel();
    versionRelease = new QLabel();
    requiresArea = new QLabel();
    requiresScrollArea = new QScrollArea();
    providesArea = new QLabel();
    providesScrollArea = new QScrollArea();

    name->setText(headerStyle(rpmInfo->name));
    archLabel->setText(subHeaderStyle("架构"));
    arch->setText(marginStyle(rpmInfo->arch));
    licenseLabel->setText(subHeaderStyle("开源协议"));
    license->setText(marginStyle(rpmInfo->license));
    license->setWordWrap(true);
    summaryLabel->setText(subHeaderStyle("简介"));
    summary->setText(docblockStyle(rpmInfo->summary));
    descriptionLabel->setText(subHeaderStyle("描述"));
    description->setWordWrap(true);
    description->setText(docblockStyle(rpmInfo->description));
    versionReleaseLabel->setText(subHeaderStyle("版本"));
    versionRelease->setText(marginStyle(rpmInfo->versionRelease));
    requiresLabel->setText(subHeaderStyle("Requires"));
    requiresArea->setText(styledList(rpmInfo->pkgRequires));
    requiresScrollArea->setWidget(requiresArea);
    providesLabel->setText(subHeaderStyle("Provides"));
    providesArea->setText(styledList(rpmInfo->pkgProvides));
    providesScrollArea->setWidget(providesArea);

    mainLayout->addWidget(name);
    mainLayout->addWidget(licenseLabel);
    mainLayout->addWidget(license);
    mainLayout->addWidget(archLabel);
    mainLayout->addWidget(arch);
    mainLayout->addWidget(versionReleaseLabel);
    mainLayout->addWidget(versionRelease);
    mainLayout->addWidget(summaryLabel);
    mainLayout->addWidget(summary);
    mainLayout->addWidget(descriptionLabel);
    mainLayout->addWidget(description);
    mainLayout->addWidget(requiresLabel);
    mainLayout->addWidget(requiresScrollArea, 1);
    mainLayout->addWidget(providesLabel);
    mainLayout->addWidget(providesScrollArea, 1);
    mainLayout->addStretch();
}

QString PkgDetailDialog::headerStyle(QString content)
{
    content = "<div style='font-size: xx-large; font-weight: bold'>"
            + content
            + "</div><hr>";
    return content;
}

QString PkgDetailDialog::subHeaderStyle(QString content)
{
    content = "<div style='font-weight: bold'>"
            + content
            + "</div>";
    return content;
}

QString PkgDetailDialog::marginStyle(QString content)
{
    QString ret;
    ret = "<div style='margin-left: 20'>";
    ret += content;
    ret += "</div>";
    return ret;
}

QString PkgDetailDialog::docblockStyle(QString content)
{
    QString ret = "<div style='margin-left: 20'><div style='border: 1px solid #ffffff; border-radius:3px'>";
    ret += content;
    ret += "</div></div>";
    return ret;
}

QString PkgDetailDialog::styledList(QVector<QString> list)
{
    QString ret = "<div style='margin-left: 20'><div style='border: 1px solid #a1a1a1; border-radius:3px'>";
    for(int i=0; i<list.size(); i++) {
        ret += "<div>" + list[i] + "</div>";
    }
    ret += "</div></div>";
    return ret;
}
