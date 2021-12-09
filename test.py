#!/usr/bin/python3
# -*- coding: utf-8 -*-
import argparse
import os
import sys
import dnf
import dnf.cli.progress
import logging
import subprocess as sup
import rpm

TOOL_VERSION = "0.1.0"
CMD_NAME = os.path.basename(__file__)
ARGS = {}

def parse_args():
    parser = argparse.ArgumentParser(description="Perform rpm install and more. Service for rpm-installer",
        epilog=f"example: os.path.basename(__file__) -mode install -pkgname /path/to/rpm.rpm")
    parser.add_argument('-v', action='version', version='dnfpy'+TOOL_VERSION)
    parser.add_argument('-mode', help='set installer mode.')
    parser.add_argument('-pkgname', help='set rpm package dir and name.')
    parser.add_argument('-pkgs', dest='pkg_list', nargs='+', help='set multiple pkg names')
  
    return parser.parse_args()

def init_logger():
    myLogger = logging.getLogger()
    myLogger.setLevel(logging.NOTSET)
    fmt = '%(asctime)s : %(message)s'
    format_str = logging.Formatter(fmt)
    sh = logging.StreamHandler(sys.stdout)
    sh.setFormatter(format_str)
    myLogger.addHandler(sh)
    return myLogger

def init_base():
    try:
        base = dnf.Base()
        base.read_all_repos()
        base.fill_sack()
        return base
    except Exception as e:
        print(e)
        print("repo error")
        print("failed")
        return None

def print_exceptions(e):
    print("\n===== Error: exceptions occur =====")
    print(e)
    print("\n===================================\n")
    print("failed")

def install_package():
    dnf_logger = init_logger()
    base = init_base()

    rpms = [ARGS.pkgname]

    try:
        package = base.add_remote_rpms(rpms)        # 添加rpm包到base库中并获取package实例对象
    except IOError as e:
        print(e)
        print("load error")
        return

    # 安装这些包
    for pkg in package:
        try:
            installRet = base.package_install(pkg)
        except dnf.exceptions.Error as e:
            print_exceptions(e)
            return
    try:
        base.resolve()
    except dnf.exceptions.Error as e:
        print_exceptions(e)
        return

    print("===== deal dependency =====")
    progress = dnf.cli.progress.MultiFileProgressMeter(fo = sys.stdout)

    try:
        # 处理依赖问题，安装包
        download_pkg_ret = base.download_packages(base.transaction.install_set, progress = progress)
    except dnf.exceptions.Error as e:
        print_exceptions(e)
        return

    try:
        tx_ret = base.do_transaction()
        print("success")
    except dnf.exceptions.Error as e:
        print("\n===== Error: exceptions occur =====")
        print(e)
        print("do_transaction ret val:")
        print(tx_ret)
        print("\n===================================\n")
        print("failed")
        
    return

def get_rpm_info():
    base = init_base()

    if(base==None):
        pass

    returnVal = '['
    unfilted_rpms = ARGS.pkg_list               # 用户直接输入的,没有过滤过的 rpm 包文件列表
    rpms = []
    load_failed_rpms = []
    for rpm in unfilted_rpms:                   # 对每个加进来的包进行检查, 检查通过才会加入到安装列表中
        try:
            base.add_remote_rpms([rpm])
            rpms.append(rpm)
        except IOError as e:
            load_failed_rpms.append(rpm)
            continue
            
    package = base.add_remote_rpms(rpms)

    q = base.sack.query()
    i = q.installed()

    for index in range(len(rpms)):
        returnVal += '{'
        returnVal += '"name": "' + package[index].name + '", '
        returnVal += '"dir": "' + rpms[index] + '", '
        returnVal += '"arch": "' + package[index].arch + '", '
        summary = package[index].summary.replace('"', '\\"')
        returnVal += '"summary": "' + summary + '", '
        returnVal += '"rpmVersionRelease": "' + package[index].version + '-' + package[index].release + '", '

        returnVal += '"provides": ['
        for provide in package[index].provides:
            returnVal += '"' + str(provide) + '",'
        returnVal = returnVal.strip(',')
        returnVal += '],'

        returnVal += '"requires": ['
        for require in package[index].requires:
            returnVal += '"' + str(require) + '",'
        returnVal = returnVal.strip(',')
        returnVal += '],'

        returnVal += '"license": "' + package[index].license + '", '
        returnVal += '"description": "' + (package[index].description.replace('\n', ' ')).replace('"', '\\"') + '", '

        installFiltedList = list(i.filter(name=package[index].name))

        if installFiltedList==[]:
            returnVal += '"installed": ' + '"False"' + ', '
            returnVal += '"installedVersionRelease": "' + 'None' + '"'
        else:
            returnVal += '"installed": ' + '"True"' + ', '
            returnVal += '"installedVersionRelease": "' + installFiltedList[0].version + '-' + installFiltedList[0].release + '"'
        returnVal += '},'
    for err_pkg in load_failed_rpms:            # 解析失败的包
        returnVal += '{'
        returnVal += '"name": "解析失败", '
        returnVal += '"dir": "' + err_pkg + '"'
        returnVal += '},'
    returnVal = returnVal.strip(',')
    returnVal += ']'
    
    print(returnVal)


def main():
    global ARGS
    ARGS = parse_args()

    if ARGS.mode=="getInfo":                # 获取包信息
        # usage: rpm-installer-dnfpy -mode getInfo -pkgs /.../botan.rpm /.../jpegoptim.rpm ...
        get_rpm_info()

    elif ARGS.mode=="install":              # 安装
        # usage: rpm-installer-dnfpy -mode install -pkgs /.../botan.rpm /.../jpegoptim.rpm ...
        install_package()


if __name__ == "__main__":
    main()
