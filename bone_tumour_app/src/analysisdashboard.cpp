#include "analysisdashboard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QPainter>
#include <QMenuBar>
#include <QFrame>
#include <QLineEdit>
#include <QRadioButton>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    this->resize(1280, 800);
    this->setWindowTitle("Analysis Suite");
    setupComponents();
    setupLayouts();
    setupConnections();

    if (!classifier.loadModel("bonetumour_model.onnx")){
        QMessageBox::critical(this, tr("Error"), tr("AI Model failed to load"));
    };
}

void MainWindow::setupComponents()
{   
    // -----------------------
    //  -- PAGES & STACKS --
    // -----------------------
    mainViewRouter = new QComboBox();
    mainViewRouter->addItem("Page 1: Scan Upload");
    mainViewRouter->addItem("Page 2: Analysis");
    mainViewRouter->setFixedSize(0, 0);

    inputPage = new QWidget();
    assessmentPage = new QWidget();

    imageStageRouter = new QComboBox();
    imageStageRouter->addItem("Drop Zone");
    imageStageRouter->addItem("Preview Zone");
    imageStageRouter->setFixedSize(0, 0);

    // -----------------------
    // -- PAGE 1 COMPONENTS --
    // -----------------------

    titleLabel = new QLabel("X-Ray Analysis Suite");
    titleLabel->setObjectName("TitleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);

    // IMAGE DROP ZONE
    dropZone = new QFrame();
    dropZone->setObjectName("DropZone");
    
    dropLabel = new QLabel();
    dropLabel->setText(
    "<p align='center'>"
    "  <img src='/Users/miguel/GitHub/bone_tumour_classification/bone_tumour_app/assets/drag-and-drop.png' width='48' height='48'>"
    "</p>"
    "<p align='center' style='font-size: 16px; font-weight: bold; margin-top: 10px; color: #1e293b;'>"
    "  Drag & Drop <span style='color: #2563eb; text-decoration: underline;'>X-Ray file</span>"
    "</p>"
    );
    dropLabel->setAlignment(Qt::AlignCenter);

    formatLabel = new QLabel("Supported formats: PNG, JPG, JPEG");
    formatLabel->setObjectName("FormatText"); 
    formatLabel->setAlignment(Qt::AlignCenter);

    // IMAGE PREVIEW ZONE
    previewZone = new QFrame();
    previewZone->setObjectName("DetectionZone");

    previewContainer = new QLabel();
    previewContainer->setMinimumSize(100, 100);
    previewContainer->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    previewContainer->setAlignment(Qt::AlignCenter);

    // RIGHT COLUMN / SIDEBAR 
    page1Sidebar = new QWidget();
    page1Sidebar->setFixedWidth(320);

    // CARD 1: SYSTEM STAT
    statusCard = new QFrame();
    statusCard->setObjectName("Card"); 
    
    statusTitle = new QLabel("SYSTEM STATUS");
    statusTitle->setObjectName("CardTitle");
    
    modelLabel = new QLabel("Model: BoneTumourClassifier v2.1");
    modelLabel->setObjectName("CardText");
    
    statusLabel = new QLabel("Status: Ready");
    statusLabel->setObjectName("CardText");

    // CARD 2: PRE-ANALYSIS DETAILS
    detailCard = new QFrame();
    detailCard->setObjectName("Card"); 

    detailTitle = new QLabel("PRE-ANALYSIS CONTEXT");
    detailTitle->setObjectName("CardTitle");
    
    IdLabel = new QLabel("<sup style = 'color: #ff0000;'>*</sup> Patient ID");
    IdLabel->setObjectName("FieldLabel");
    
    IdInput = new QLineEdit();
    IdInput->setObjectName("InputField");
    
    viewLabel = new QLabel("<sup style = 'color: #ff0000;'>*</sup> Scan View");
    viewLabel->setObjectName("FieldLabel");

    APview = new QRadioButton("AP");
    APview->setObjectName("InputField");
    APview->setChecked(true);
    Lateralview = new QRadioButton("Lateral");
    Lateralview->setObjectName("InputField");

    // MODEL INFORMATION
    infoButton = new QPushButton(" Model Information");
    infoButton->setIcon(QIcon(":/info.png"));
    infoButton->setIconSize(QSize(16,16));
    infoButton->setFixedWidth(150);
    infoButton->setObjectName("InfoButton");

    infoMessageBox = new QMessageBox();
    infoMessageBox->setWindowTitle("Model Information");
    infoMessageBox->setText(
        "<h3 style='color: #1A202C; margin-bottom: 4px; font-size: 16px;'>YOLOv8 Bone Tumour Classifier</h3>"
        "<p style='color: #888888; font-size: 11px; text-transform: uppercase; letter-spacing: 0.5px; margin-bottom: 12px;'>Onnx Runtime Model</p>"
        "<hr style='border: none; background-color: #E5E9F0; height: 1px; margin-bottom: 12px;'/>"
        "<p style='color: #555555; font-size: 13px; line-height: 18px;'>""This suite executes computer vision classification using a deep learning model trained on publicly avaliable bone tumour data via Kaggle.<br><br>"
        "<b>Supported Types:</b> <ul> <li>Giant Cell Tumour</li> <li>Osteochondroma</li> <li>Osteosarcoma</li><li>Osteofibroma</li><li>Other Benign Tumour</li><li>Other Malignant Tumour</li>"
        "</p>"
        "<hr style='border: none; background-color: #E5E9F0; height: 1px; margin-bottom: 12px;'/>");
    infoMessageBox->setIconPixmap(QIcon(":/info.png").pixmap(32, 32));

    QPushButton *closeButton = infoMessageBox->addButton("Acknowledge", QMessageBox::AcceptRole);
    closeButton->setStyleSheet("background-color: #2563eb; color: white; font-weight: bold; border-radius: 6px;");
    closeButton->setFixedSize(120, 36); 

    // ACTION BUTTONS
    uploadButton = new QPushButton(" Upload");
    uploadButton->setObjectName("UploadButton"); 
    uploadButton->setIcon(QIcon(":/downloads.png"));
    uploadButton->setIconSize(QSize(16,16));
    uploadButton->setFixedHeight(45);

    analyseButton = new QPushButton(" Analyse");
    analyseButton->setObjectName("AnalyseButton");
    analyseButton->setIcon(QIcon(":/search.png"));
    analyseButton->setFixedHeight(45);
    analyseButton->setEnabled(false);
    
    // -----------------------
    // -- PAGE 2 COMPONENTS --
    // -----------------------

    // IMAGE DETECTION ZONE
    detectionZone = new QFrame();
    detectionZone->setObjectName("DetectionZone");

    detectionContainer = new QLabel();
    detectionContainer->setMinimumSize(100, 100);
    detectionContainer->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    detectionContainer->setAlignment(Qt::AlignCenter);
    
    // RIGHT COLUMN / SIDEBAR
    page2Sidebar = new QWidget();
    page2Sidebar->setFixedWidth(320);

    // CARD 1: DETECTED TUMOUR SEVERITY
    severityCard = new QFrame();
    severityCard->setObjectName("Card");

    severityTitle = new QLabel("CASE SEVERITY");
    severityTitle->setObjectName("CardTitle");
    severityLabel = new QLabel();
    severityLabel->setAlignment(Qt::AlignCenter);
    severityLabel->setFixedHeight(36);
    severityLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    
    // CARD 2: MODEL REPORT
    reportCard = new QFrame();
    reportCard->setObjectName("Card");

    confidenceTitle = new QLabel("MODEL CONFIDENCE");
    confidenceTitle->setObjectName("CardTitle");

    confidenceBar = new QProgressBar();
    confidenceBar->setObjectName("ConfidenceBar");
    confidenceBar->setOrientation(Qt::Horizontal);
    confidenceBar->setRange(0, 100);
    
    tumourNameTitle = new QLabel("TUMOUR CATEGORY");
    tumourNameTitle->setObjectName("CardTitle");
    tumourNameLabel = new QLabel();
    tumourNameLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #1A202C; border: none; background: transparent;");

    // ACTION BUTTONS
    restartButton = new QPushButton(" New Scan");
    restartButton->setIcon(QIcon(":/refresh.png"));
    restartButton->setFixedHeight(40);
    restartButton->setObjectName("AnalyseButton");

    exitButton = new QPushButton(" Close Suite");
    exitButton->setFixedHeight(40);
    exitButton->setObjectName("UploadButton");
}

void MainWindow::setupLayouts()
{
    // -----------------------
    //   -- PAGE 1 LAYOUT --
    // -----------------------
    QHBoxLayout *page1Layout = new QHBoxLayout(inputPage);
    page1Layout->setContentsMargins(20, 20, 20, 20);
    page1Layout->setSpacing(20);

    QVBoxLayout *dropZoneLayout = new QVBoxLayout(dropZone);
    dropZoneLayout->setContentsMargins(20, 40, 20, 20);
    dropZoneLayout->addStretch();
    dropZoneLayout->addWidget(dropLabel);
    dropZoneLayout->addStretch();
    dropZoneLayout->addWidget(formatLabel);

    QVBoxLayout *previewZoneLayout = new QVBoxLayout(previewZone);
    previewZoneLayout->setContentsMargins(20, 20, 20, 20);
    previewZoneLayout->addWidget(previewContainer);

    QVBoxLayout *page1SidebarLayout = new QVBoxLayout(page1Sidebar);
    page1SidebarLayout->setContentsMargins(0, 0, 0, 0);
    page1SidebarLayout->setSpacing(15);

    QVBoxLayout *statusLayout = new QVBoxLayout(statusCard);
    statusLayout->setContentsMargins(15, 15, 15, 15);
    statusLayout->setSpacing(8);
    statusLayout->addWidget(statusTitle);
    statusLayout->addWidget(modelLabel);
    statusLayout->addWidget(statusLabel);

    QVBoxLayout *detailLayout = new QVBoxLayout(detailCard);
    detailLayout->setContentsMargins(15, 15, 15, 15);
    detailLayout->setSpacing(8);
    detailLayout->addWidget(detailTitle);
    detailLayout->addWidget(IdLabel);
    detailLayout->addWidget(IdInput);
    detailLayout->addWidget(viewLabel);

    QHBoxLayout *radioLayout = new QHBoxLayout();
    radioLayout->setSpacing(20);
    radioLayout->addWidget(APview);
    radioLayout->addWidget(Lateralview);
    radioLayout->addStretch();

    detailLayout->addLayout(radioLayout);

    page1SidebarLayout->addWidget(titleLabel);
    page1SidebarLayout->addSpacing(20);
    page1SidebarLayout->addWidget(statusCard);
    page1SidebarLayout->addWidget(detailCard);
    page1SidebarLayout->addWidget(infoButton);
    page1SidebarLayout->addStretch();

    QHBoxLayout *page1ActionLayout = new QHBoxLayout();
    page1ActionLayout->setSpacing(10);
    page1ActionLayout->addWidget(uploadButton);
    page1ActionLayout->addWidget(analyseButton);
    page1SidebarLayout->addLayout(page1ActionLayout);

    imageStageStack = new QStackedWidget();
    imageStageStack->addWidget(dropZone);
    imageStageStack->addWidget(previewZone);

    page1Layout->addWidget(imageStageStack, 1);
    page1Layout->addWidget(page1Sidebar);

    // -----------------------
    //   -- PAGE 2 LAYOUT --
    // -----------------------
    QHBoxLayout *page2Layout = new QHBoxLayout(assessmentPage);
    page2Layout->setContentsMargins(20, 20, 20, 20);
    page2Layout->setSpacing(20);

    QVBoxLayout *detectionLayout = new QVBoxLayout(detectionZone);
    detectionLayout->setContentsMargins(20, 40, 20, 20);
    detectionLayout->addWidget(detectionContainer);

    QVBoxLayout *page2SidebarLayout = new QVBoxLayout(page2Sidebar);
    page2SidebarLayout->setContentsMargins(0, 0, 0, 0);
    page2SidebarLayout->setSpacing(15);

    QVBoxLayout *severityLayout = new QVBoxLayout(severityCard);
    severityLayout->setContentsMargins(15, 15, 15, 15);
    severityLayout->setSpacing(8);
    severityLayout->addWidget(severityTitle);
    severityLayout->addWidget(severityLabel);

    QVBoxLayout *reportLayout = new QVBoxLayout(reportCard);
    reportLayout->setContentsMargins(15, 15, 15, 15);
    reportLayout->setSpacing(8);
    reportLayout->addWidget(tumourNameTitle);
    reportLayout->addWidget(tumourNameLabel);
    reportLayout->setSpacing(20);
    reportLayout->addWidget(confidenceTitle);
    reportLayout->addWidget(confidenceBar);

    page2SidebarLayout->addStretch();
    page2SidebarLayout->addWidget(severityCard);
    page2SidebarLayout->addWidget(reportCard);
    page2SidebarLayout->addStretch();

    QHBoxLayout *page2ActionLayout = new QHBoxLayout();
    page2ActionLayout->setSpacing(10);
    page2ActionLayout->addWidget(restartButton, 1);
    page2ActionLayout->addWidget(exitButton, 1);
    page2SidebarLayout->addLayout(page2ActionLayout);

    page2Layout->addWidget(detectionZone, 1);
    page2Layout->addWidget(page2Sidebar);

    mainViewStack = new QStackedWidget(this);
    mainViewStack->addWidget(inputPage);
    mainViewStack->addWidget(assessmentPage);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setObjectName("centralWidget");
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    mainLayout->addWidget(mainViewStack);
    setCentralWidget(centralWidget);
}

void MainWindow::setupConnections()
{
    connect(infoButton, &QPushButton::clicked, infoMessageBox, &QMessageBox::exec);
    connect(mainViewRouter, &QComboBox::currentIndexChanged,
            mainViewStack, &QStackedWidget::setCurrentIndex);
    connect(imageStageRouter, &QComboBox::currentIndexChanged,
            imageStageStack, &QStackedWidget::setCurrentIndex);
    connect(uploadButton, &QPushButton::clicked, this, &MainWindow::uploadButton_clicked);
    connect(analyseButton, &QPushButton::clicked, this, &MainWindow::analyseButton_clicked);
    connect(restartButton, &QPushButton::clicked, this, &MainWindow::restartButton_clicked);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::close);
}

MainWindow::~MainWindow(){}

void MainWindow::uploadButton_clicked()
{
    filePath = QFileDialog::getOpenFileName(
        this, tr("Open X-Ray Scan"), "", tr("Images (*.png *.jpg *.jpeg)")
    );

    if (filePath.isEmpty()) return;

    QPixmap inputPixmap(filePath);

    if (inputPixmap.isNull()){
        QMessageBox::warning(this, tr("Error"), tr("Failed to load image"));
        return;
    }

    zoneSize = dropZone->size();
    previewContainer->setPixmap(inputPixmap.scaled(zoneSize.width(), zoneSize.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    analyseButton->setEnabled(true);
    imageStageRouter->setCurrentIndex(1);
}

void MainWindow::analyseButton_clicked()
{
    DetectionResult report = classifier.predict(filePath);
    QPixmap inputPixmap(filePath);

    if (!report.success) {
        severityLabel->setText("System Error");
        severityLabel->setStyleSheet("color: #FFFFFF; background-color: #D32F2F; font-weight: bold; font-size: 13px; border-radius: 4px; border: none;");
        return;
    }

    if (report.severity == "Malignant") {
        severityLabel->setText("  CRITICAL MALIGNANT FINDING");
        severityLabel->setStyleSheet("color: #D32F2F; background-color: #FFEBEE; font-weight: bold; font-size: 12px; border-radius: 18px; border: 1px solid #FFCDD2; padding: 5px;");
    } else if (report.severity == "Aggressive Benign") {
        severityLabel->setText("  WARNING: LOCALLY AGGRESSIVE");
        severityLabel->setStyleSheet("color: #B78103; background-color: #FFFDE7; font-weight: bold; font-size: 12px; border-radius: 18px; border: 1px solid #FFF9C4; padding: 5px;");
    } else if (report.severity == "Benign") {
        severityLabel->setText("  BENIGN CONDITION DETECTED");
        severityLabel->setStyleSheet("color: #1E4620; background-color: #E8F5E9; font-weight: bold; font-size: 12px; border-radius: 18px; border: 1px solid #C8E6C9; padding: 5px;");
    } else {
        severityLabel->setText("  NO TUMOUR DETECTED");
        severityLabel->setStyleSheet("color: #0D47A1; background-color: #E3F2FD; font-weight: bold; font-size: 12px; border-radius: 18px; border: 1px solid #BBDEFB; padding: 5px;");
    }

    tumourNameLabel->setText(report.className);
    
    int percentage = report.confidence * 100;
    confidenceBar->setValue(percentage);

    QString percentageString = QString::number(percentage);
    confidenceBar->setFormat(percentageString);

    if (!report.boundingBox.isNull()){
        QPainter painter(&inputPixmap);

        QPen pen;
        pen.setStyle(Qt::DotLine);
        if (report.severity == "Malignant") {
            pen.setColor(QColor("#D32F2F"));
        } else if (report.severity == "Aggressive Benign") {
            pen.setColor(QColor("#B78103")); 
        } else {
            pen.setColor(QColor("#1E4620"));
        }

        pen.setWidth(4);
        painter.setPen(pen);

        painter.drawRect(report.boundingBox);
        painter.end();
    }

    detectionContainer->setPixmap(inputPixmap.scaled(zoneSize.width(), zoneSize.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    mainViewRouter->setCurrentIndex(1);
}

void MainWindow::restartButton_clicked()
{
    analyseButton->setEnabled(false);
    mainViewRouter->setCurrentIndex(0);
}