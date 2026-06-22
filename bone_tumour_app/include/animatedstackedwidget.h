#ifndef ANIMATEDSTACKEDWIDGET_H
#define ANIMATEDSTACKEDWIDGET_H

#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>

class AnimatedStackedWidget : public QStackedWidget {
    Q_OBJECT
public:
    explicit AnimatedStackedWidget(QWidget *parent = nullptr);
    void fadeToIndex(int index);
    void setAnimationDuration(int ms);

private:
    int m_animationDuration;
};

#endif