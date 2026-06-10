#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QStackedWidget>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void uploadButton_clicked(); 
    void denyButton_clicked();

private:
    QPushButton *uploadButton;
    QPushButton *acceptButton;
    QPushButton *denyButton;
    QPushButton *infoButton;
    QLabel *dragdropLabel;
    QLabel *imageDisplayLabel;
    QComboBox *comboBox;
    QStackedWidget *stackedWidget;
    
};

#endif 