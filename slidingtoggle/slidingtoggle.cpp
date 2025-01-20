#include "slidingtoggle.h"

/**
 * @brief  使用方法:在外部用两个信号分别连接开和关
 * connect(sender, signal(), (Switch*)receiver, toggleOn());
 * connect(sender, signal(), (Switch*)receiver, toggleOff());
 */

SlidingToggle::SlidingToggle(QWidget *parent) : QWidget(parent)
{

}

void SlidingToggle::setState(bool state)
{
    this->currentToggleState = state;
}

bool SlidingToggle::state()
{
    return currentToggleState;
}

void SlidingToggle::setAnimationDuration(int dur)
{
    this->switchAnimationDuration = dur;
}

void SlidingToggle::setRoundBallColorOn(QColor color)
{
    roundColorOn =  color;
}

void SlidingToggle::setRoundBallColorOff(QColor color)
{
    roundColorOff = color;
}

void SlidingToggle::setColorOn(QColor color)
{
    colorOn = color;
}

void SlidingToggle::setColorOff(QColor color)
{
    colorOff = color;
}

void SlidingToggle::toggleOnSlot()
{
    if (!currentToggleState)
    {
        toggleState = true;
        startToggleAnimation();
    }
}

void SlidingToggle::toggleOffSlot()
{
    if (currentToggleState)
    {
        toggleState = false;
        startToggleAnimation();
    }
}

void SlidingToggle::mousePressEvent(QMouseEvent *event)
{
    //动画通过递增/减this->progress的值[0,1]，再调用update刷新绘制
    if (event->button() == Qt::LeftButton && setBackgroundPath().contains(event->pos()))
    {
        if (!currentToggleState)
        {
            emit toggleOnSignal();
        }
        else
        {
            emit toggleOffSignal();
        }
        return ;
    }
}

/**
 * @brief 重绘
 * @param event
 */
void SlidingToggle::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    //背景
    QPainterPath backgroundPath = setBackgroundPath();
    painter.fillPath(backgroundPath, setBackgroundColor());

    //圆球开关
    QPainterPath circlePath = setRoundBallPath(progress);
    painter.fillPath(circlePath, setRoundColor());

}

/*设置背景形状*/
QPainterPath SlidingToggle::setBackgroundPath() const
{
    const double R = height() / 2;
    QPainterPath path;
    path.moveTo(R, 0);
    const QRectF leftSemicircleRect(0, 0, 2 * R, 2 * R);
    const QRectF rightSemicircleRect(width() - 2 * R, 0, 2 * R, 2 * R);
    path.arcTo(leftSemicircleRect, 90, 180);
    path.lineTo(width() - 2 * R, 2 * R);
    path.arcTo(rightSemicircleRect, -90, 180);
    path.lineTo(R, 0);

    return path;
}

QColor SlidingToggle::setBackgroundColor() const
{
    double prog = progress;
    if (prog < 0)
        prog = 0;
    else if (prog > 1)
        prog = 1;

    return QColor(static_cast<int>(colorOff.red() + (colorOn.red() - colorOff.red())*prog),
                  static_cast<int>(colorOff.green() + (colorOn.green() - colorOff.green())*prog),
                  static_cast<int>(colorOff.blue() + (colorOn.blue() - colorOff.blue())*prog),
                  static_cast<int>(colorOff.alpha() + (colorOn.alpha() - colorOff.alpha())*prog));
}

QPainterPath SlidingToggle::setRoundBallPath(double progress)
{
    const double r = height() / 2 * radiusRatio;
    QPointF leftTopPos((1 - radiusRatio) * (r / radiusRatio) + progress * (width() - (2 * r / radiusRatio)), (1 - radiusRatio) * (r / radiusRatio));//(0.1R + (w-2R)*p, 0.1R)
    QPainterPath path;
    path.moveTo(leftTopPos.x() + r, leftTopPos.y());
    const QRectF circleRect(leftTopPos.x(), leftTopPos.y(), 2 * r, 2 * r);
    path.arcTo(circleRect, 90, 360);

    return path;
}

QColor SlidingToggle::setRoundColor() const
{
    double prog = progress;
    if (prog < 0)
        prog = 0;
    else if (prog > 1)
        prog = 1;

    return QColor(static_cast<int>(roundColorOff.red() + (roundColorOn.red() - roundColorOff.red())*prog),
                  static_cast<int>(roundColorOff.green() + (roundColorOn.green() - roundColorOff.green())*prog),
                  static_cast<int>(roundColorOff.blue() + (roundColorOn.blue() - roundColorOff.blue())*prog),
                  static_cast<int>(roundColorOff.alpha() + (roundColorOn.alpha() - roundColorOff.alpha())*prog));
}

/**
 * @brief 开始动画
 */
void SlidingToggle::startToggleAnimation()
{
    if (isAnimationFinished)
    {
        isAnimationFinished = false;
        double startProgressValue = currentToggleState ? 1 : 0;
        double endProgressValue = currentToggleState ? 0 : 1;
        QPropertyAnimation *animation = new QPropertyAnimation(this, "progress");
        animation->setStartValue(startProgressValue);
        animation->setEndValue(endProgressValue);
        animation->setDuration(switchAnimationDuration);
        animation->setEasingCurve(QEasingCurve::InOutCubic);//InOutCubic 缓和曲线立方（T^3）功能：加速至一半，然后再减速。
        connect(animation, &QPropertyAnimation::finished, this, [this]{
            isAnimationFinished = true;
            currentToggleState = toggleState;
        });
        connect(animation, SIGNAL(valueChanged(const QVariant &)), this, SLOT(update()));
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

double SlidingToggle::getProgress()
{
    return this->progress;
}

void SlidingToggle::setProgress(double progress)
{
    this->progress = progress;
    update();
}
