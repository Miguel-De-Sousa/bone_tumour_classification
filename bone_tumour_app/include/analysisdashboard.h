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
#include <QTextEdit>
#include "bonetumourclassifier.h"
#include "animatedstackedwidget.h"

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
    void initialiseModel();
    void uploadButton_clicked(); 
    void analyseButton_clicked();
    void restartButton_clicked();
    void advDetailButton_clicked();
    void analyseButton_state();

private:
    BoneTumourClassifier classifier;
    AnimatedStackedWidget *mainViewStack;
    AnimatedStackedWidget *imageStageStack;
    
    // Buttons
    QPushButton *uploadButton;
    QPushButton *analyseButton;
    QPushButton *infoButton;
    QPushButton *restartButton;
    QPushButton *exitButton;
    QPushButton *approveButton;
    QPushButton *editDiagnosis;
    QPushButton *pdfButton;
    
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
    QPushButton *advDetailTitle;
    QLabel *clinicalNotesTitle;
    QLabel *IdLabel;
    QLabel *viewLabel;
    QLabel *severityTitle;
    QLabel *confidenceTitle;
    QLabel *tumourNameTitle;
    QLabel *exportTitle;
    QLabel *verificationTitle;
    QLabel *confidenceValueLabel;

    QProgressBar *confidenceBar;
    
    // Inputs & Window layout
    QLineEdit *IdInput;
    QRadioButton *APview;
    QRadioButton *Lateralview;
    QComboBox *mainViewRouter;
    QComboBox *imageStageRouter;
    QMessageBox *infoMessageBox;
    QFrame *dropZone;
    QFrame *previewZone;
    QFrame *detectionZone;
    QFrame *statusCard;
    QFrame *detailCard;
    QFrame *advDetailCard;
    QFrame *severityCard;
    QFrame *reportCard;
    QFrame *verificationCard;
    QFrame *exportCard;
    QWidget *page1Sidebar;
    QWidget *page2Sidebar;
    QWidget *inputPage;
    QWidget *assessmentPage;
    QTextEdit *clinicalNotes;

    // File Pixmap Handling
    QString filePath;
    QSize zoneSize;

    // Separators
    QFrame *separatorActionPage1;
    QFrame *separatorTitlePage1;
    QFrame *separatorReportPage2;
    QFrame *separatorActionPage2;
};

#endif 