/**
 * @brief  
 * connect(sender, signal(), (Switch*)receiver, toggleOn());
 * connect(sender, signal(), (Switch*)receiver, toggleOff());
 */
#ifndef SLIDINGTOGGLE_H
#define SLIDINGTOGGLE_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QResizeEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QDebug>
#include <QPropertyAnimation>
#include "math.h"

class SlidingToggle : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double progress READ getProgress WRITE setProgress)
public:
    explicit SlidingToggle(QWidget *parent = nullptr);

    void setState(bool state);
    bool state();
    // 设置动画时间
    void setAnimationDuration(int dur);
    // 设置开关颜色
    void setRoundBallColorOn(QColor color);
    void setRoundBallColorOff(QColor color);
    void setColorOn(QColor color);
    void setColorOff(QColor color);    

signals:
    void toggleOnSignal();
    void toggleOffSignal();

public slots:
    void toggleOnSlot();
    void toggleOffSlot();

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QPainterPath setBackgroundPath() const;
    QColor setBackgroundColor() const;    
    QPainterPath setRoundBallPath(double progress);    
    QColor setRoundColor() const;
    //开始动画
    void startToggleAnimation();

    double getProgress();
    void setProgress(double progress);

private:
    // 状态
    bool currentToggleState = false;
    bool toggleState = false;
    bool isAnimationFinished = true;

    // 颜色
    QColor roundColorOn = QColor(Qt::darkYellow);
    QColor roundColorOff = QColor(Qt::white);
    QColor colorOn = QColor(63, 221, 63);
    QColor colorOff = QColor(Qt::lightGray);    

    // 动画
    double progress = 0; // 进度(0~1)
    int switchAnimationDuration = 300; // 动画时长(ms)
    // 几何
    double radiusRatio = 0.9;

signals:

};

#endif // SLIDINGTOGGLE_H
