#ifndef ANALYSISDASHBOARD_H
#define ANALYSISDASHBOARD_H

#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QStackedWidget>
#include <QLabel>
#include <QMessageBox>
#include <QFrame>
#include <QLineEdit>
#include <QRadioButton>
#include <QProgressBar>
#include "bonetumourclassifier.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void setupComponents();
    void setupLayouts();
    void setupConnections();
    void uploadButton_clicked(); 
    void analyseButton_clicked();
    void restartButton_clicked();

private:
    BoneTumourClassifier classifier;
    
    // Buttons
    QPushButton *uploadButton;
    QPushButton *analyseButton;
    QPushButton *infoButton;
    QPushButton *restartButton;
    QPushButton *exitButton;
    
    // Labels
    QLabel *titleLabel;
    QLabel *formatLabel;
    QLabel *detectionContainer;
    QLabel *previewContainer;
    QLabel *tumourNameLabel;
    QLabel *severityLabel;
    QLabel *dropLabel;
    QLabel *statusTitle;
    QLabel *statusLabel;
    QLabel *modelLabel;
    QLabel *detailTitle;
    QLabel *IdLabel;
    QLabel *viewLabel;
    QLabel *severityTitle;
    QLabel *confidenceTitle;
    QLabel *tumourNameTitle;

    QProgressBar *confidenceBar;
    
    // Inputs & Window layout
    QLineEdit *IdInput;
    QRadioButton *APview;
    QRadioButton *Lateralview;
    QComboBox *mainViewRouter;
    QComboBox *imageStageRouter;
    QStackedWidget *mainViewStack;
    QStackedWidget *imageStageStack;
    QMessageBox *infoMessageBox;
    QFrame *dropZone;
    QFrame *previewZone;
    QFrame *detectionZone;
    QFrame *statusCard;
    QFrame *detailCard;
    QFrame *severityCard;
    QFrame *reportCard;
    QWidget *page1Sidebar;
    QWidget *page2Sidebar;
    QWidget *inputPage;
    QWidget *assessmentPage;

    // File Pixmap Handling
    QString filePath;
    QSize zoneSize;
};

#endif 