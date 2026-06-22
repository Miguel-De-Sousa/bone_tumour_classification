#include "animatedstackedwidget.h"
#include <QPointer>

AnimatedStackedWidget::AnimatedStackedWidget(QWidget *parent)
    : QStackedWidget(parent), m_animationDuration(300) {}

void AnimatedStackedWidget::fadeToIndex(int index) {
    if (currentIndex() == index) return;

    QWidget *oldWidget = currentWidget();
    QWidget *newWidget = widget(index);

    if (!oldWidget || !newWidget) {
        setCurrentIndex(index);
        return;
    }

    QPointer<QWidget> safeNewWidget = newWidget;
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect(newWidget);
    QPointer<QGraphicsOpacityEffect> safeOpacityEffect = opacityEffect;

    newWidget->setGraphicsEffect(opacityEffect);

    QPropertyAnimation *anim = new QPropertyAnimation(opacityEffect, "opacity");
    anim->setDuration(m_animationDuration);
    anim->setStartValue(0.0);
    anim->setEndValue(1.0);
    anim->setEasingCurve(QEasingCurve::OutQuad);

    connect(anim, &QPropertyAnimation::finished, anim, [safeNewWidget, safeOpacityEffect, anim]() {
        if (safeNewWidget && safeOpacityEffect) {
            if (safeNewWidget->graphicsEffect() == safeOpacityEffect) {
                safeNewWidget->setGraphicsEffect(nullptr);
            }
        }
        
        if (safeOpacityEffect) {
            safeOpacityEffect->deleteLater();
        }
        anim->deleteLater();
    });

    setCurrentIndex(index);
    anim->start();
}

void AnimatedStackedWidget::setAnimationDuration(int ms) {
    m_animationDuration = ms;
}