#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QStackedWidget>
#include <QTextEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void uploadButton_clicked(); 

private:
    QPushButton *uploadButton;
    QComboBox *comboBox;
    QStackedWidget *stackedWidget;
    QTextEdit *textEditPage2;
};

#endif 