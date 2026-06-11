#ifndef ANALYSISDASHBOARD_H
#define ANALYSISDASHBOARD_H

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QStackedWidget>
#include <QLabel>
#include "bonetumourclassifier.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void uploadButton_clicked(); 
    void acceptButton_clicked();
    void denyButton_clicked();

private:
    BoneTumourClassifier classifier;
    QPushButton *uploadButton;
    QPushButton *acceptButton;
    QPushButton *denyButton;
    QPushButton *infoButton;
    QLabel *dragdropLabel;
    QLabel *imageDisplayLabel;
    QLabel *resultsLabel;
    QComboBox *comboBox;
    QStackedWidget *stackedWidget;
    
};

#endif 