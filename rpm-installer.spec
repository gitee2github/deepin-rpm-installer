%global             debug_package  %{nil}

Name:               deepin-rpm-installer
Version:            1.0
Release:            1
Summary:            A really lightweight GUI RPM installer
License:            MulanPSL-2.0
URL:                https://gitee.com/openeuler/deepin-rpm-installer
Source0:            %{name}-%{version}.tar.gz

BuildRequires:      gcc, qt5-qtbase-devel, qt5-qtsvg-devel, make, zlib-devel
BuildRequires:      python3, python-pip, dnf, python3-dnf

Requires:           qt5-qtbase, dnf, qt5-qtsvg

%description
A really lightweight RPM installer

%prep
%setup -q

%build
python3 -m pip install pyinstaller
pyinstaller -F ./dnfpy/rpminstaller-dnfpy.py

qmake-qt5 ./rpminstaller.pro
make

%install
%{__mkdir_p} %{buildroot}%{_bindir}/
%{__mkdir_p} %{buildroot}%{_datadir}/applications/

install -m 755 ./rpminstaller %{buildroot}%{_bindir}/
install -m 755 ./dist/rpminstaller-dnfpy %{buildroot}%{_bindir}/

install -m 644 ./resources/rpminstaller.desktop %{buildroot}%{_datadir}/applications/

%files
%{_bindir}/rpminstaller
%{_bindir}/rpminstaller-dnfpy

%{_datadir}/applications/rpminstaller.desktop

%changelog
* Sun Sep 12 2021 Li Jingwei <marscatcn@live.com> - 1.0-1
- Package init
