#ifndef ANALYSISDASHBOARD_H
#define ANALYSISDASHBOARD_H

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QStackedWidget>
#include <QLabel>
#include <QMessageBox>
#include "bonetumourclassifier.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void setupUIComponents();
    void setupPages();
    void setupConnections();
    void uploadButton_clicked(); 
    void acceptButton_clicked();
    void denyButton_clicked();

private:
    BoneTumourClassifier classifier;
    QPushButton *uploadButton;
    QPushButton *acceptButton;
    QPushButton *denyButton;
    QPushButton *infoButton;
    QPushButton *restartButton;
    QPushButton *exitButton;
    QLabel *titleLabel;
    QLabel *supportLabel;
    QLabel *dragdropLabel;
    QLabel *imageDisplayLabel;
    QLabel *imageFinalLabel;
    QLabel *tumourNameLabel;
    QLabel *severityLabel;
    QLabel *confidenceLabel;
    QComboBox *comboBox;
    QStackedWidget *stackedWidget;
    QString filePath;
    QMessageBox *infoMessageBox;
};

#endif 