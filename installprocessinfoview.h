#ifndef INSTALLPROCESSINFOVIEW_H
#define INSTALLPROCESSINFOVIEW_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QTextEdit>
#include <QGestureEvent>

/**
 * @brief The ShowInstallInfoTextEdit class
 * QTextEdit控件 修改部分触控事件
 */
class ShowInstallInfoTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit ShowInstallInfoTextEdit(QWidget *parent = nullptr);

    // QObject interface
public:
    bool event(QEvent *event) override;

    // QWidget interface
protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

    // QWidget interface
protected:
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    /**
     * @brief gestureEvent 手势事件
     * @param event
     * @return
     */
    bool gestureEvent(QGestureEvent *event);

    /**
     * @brief tapGestureTriggered 单击手势事件
     */
    void tapGestureTriggered(QTapGesture *);

    /**
     * @brief tapAndHoldGestureTriggered 单指长按事件
     */
    void tapAndHoldGestureTriggered(QTapAndHoldGesture *);


    /**
     * @brief slideGesture 单指滑动手势(通过原生触摸屏事件进行抽象模拟)
     * @param diff
     * add for single refers to the sliding
     */
    void slideGesture(qreal diff);

    /**
     * @brief onSelectionArea 滑动选中事件
     */
    void onSelectionArea();

private:
    //触摸屏手势动作
    enum GestureAction {
        GA_null,        //无动作
        GA_tap,         //点击
        GA_slide,       //滑动
        GA_hold,        //长按
    };

    GestureAction m_gestureAction = GA_null;    //手势动作 默认误动作

    qint64 m_tapBeginTime = 0;                  //开始点击的时间

    bool m_slideContinue {false};               //是否持续滑动

    //add for single refers to the sliding
    //FlashTween tween;                           //滑动惯性

    qreal change = {0.0};                       //滑动变化量
    qreal duration = {0.0};                     //滑动方向

    //鼠标事件的位置
    int m_start = 0;                            //开始时鼠标的位置
    int m_end = 0;                              //结束时鼠标的位置
    qreal m_stepSpeed = 0;                      //移动的速度

    int m_lastMousepos;                         //上次移动后鼠标的位置

    ulong m_lastMouseTime;                      //上次移动鼠标的时间

    int m_nSelectEndLine;                       //< 选择结束时后鼠标所在行
    int m_nSelectStart;                         //< 选择开始时的鼠标位置
    int m_nSelectEnd;                           //< 选择结束时的鼠标位置

};
class InstallProcessInfoView : public QWidget
{
    Q_OBJECT
public:
    explicit InstallProcessInfoView(int w, int h, QWidget *parent = nullptr);
    virtual ~InstallProcessInfoView() override;

    /**
     * @brief appendTextx 向installProcessInfo中添加数据
     * @param text 要添加的数据
     */
    void appendText(QString text);

    /**
     * @brief setTextFontSize 设置字体大小
     * @param fontSize      字体大小   PS： 此参数无用
     * @param fontWeight    字体大小
     */
    void setTextFontSize(int fontSize, int fontWeight);

    /**
     * @brief clearText 清空目前installProcessInfo中的数据
     */
    void clearText();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    /**
     * @brief initUI 初始化ProcessInfo的大小
     * @param w 控件的宽度
     * @param h 控件的高度
     * 此处在SP3之后修改，增加宽度高度参数
     * 为适应配置框的大小与安装器installProcessInfo的大小
     */
    void initUI(int w, int h);

    ShowInstallInfoTextEdit *m_editor;                               //展示框 修改为自写控件
};



#endif // INSTALLPROCESSINFOVIEW_H
