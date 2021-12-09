#include "mainwindow.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QPushButton>
#include <QWidget>
#include <QString>
#include <QMenuBar>
#include <string>

int main(int argc, char *argv[])                    // 整个程序的主函数，在这里实例化各个对象
{
    QApplication a(argc, argv);                     // 这应该就是 app 对象本身了
    MainWindow w(argc, argv);       // window 可以先实例化，然后再调用 show 来显示

    w.show();

    return a.exec();                // 进入程序主消息循环，是qt程序全生命周期的开始
}
