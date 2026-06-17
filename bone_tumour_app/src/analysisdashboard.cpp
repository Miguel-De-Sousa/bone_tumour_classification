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

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    this->resize(1280, 800);
    this->setWindowTitle("Analysis Suite");
    
    // -----------------------------------------------------------------
    // QSS
    // -----------------------------------------------------------------
    this->setStyleSheet(R"(
        /* Main Window Base Color */
        QMainWindow, QWidget#centralWidget {
            background-color: #f5f7fa;
        }
        
        /* Window Title */
        QLabel#TitleLabel {
            font-size: 20px; 
            font-weight: bold; 
            color: #010101;
        }

        /* Drop Container */
        QFrame#DropZone {
            background-color: #ffffff;
            border: 2px dashed #cbd5e1;
            border-radius: 8px;
        }

        QLabel#FormatText {
            font-size: 12px;
            color: #64748b;
        }

        /* Sidebar Cards */
        QFrame#Card {
            background-color: #ffffff;
            border: 1px solid #e2e8f0;
            border-radius: 8px;
        }

        QLabel#CardTitle {
            font-size: 11px;
            font-weight: bold;
            color: #64748b;
            text-transform: uppercase;
        }

        QLabel#CardText {
            color: #334155;
            font-size: 13px;
        }

        /* Form Inputs */
        QLabel#FieldLabel {
            font-size: 12px;
            font-weight: bold;
            color: #475569;
        }

        QLineEdit#InputField {
            background-color: #f8fafc;
            border: 1px solid #cbd5e1;
            border-radius: 6px;
            padding: 8px;
            color: #0f172a;
            font-size: 13px;
        }
        QLineEdit#InputField:focus {
            border: 1px solid #2563eb;
            background-color: #ffffff;
        }

        /* Model Information Button */
        QPushButton#InfoButton {
            background-color: transparent;
            color: #64748b;    
            border: none;                   
            border-radius: 6px;                 
            padding: 8px;
            font-size: 12px;
            font-weight: bold;
            text-align: left;
        }
        QPushButton#InfoButton:hover {
            background-color: #e2e8f0; 
            color: #0f172a;
        }

        /* Action Buttons */
        QPushButton#UploadButton {
            background-color: #ffffff;
            border: 1px solid #2563eb;
            color: #2563eb;
            font-size: 14px;
            font-weight: bold;
            border-radius: 8px;
            padding: 10px 24px;
        }
        QPushButton#UploadButton:hover {
            background-color: #eff6ff;
        }
        QPushButton#UploadButton:pressed {
            background-color: #dbeafe;
        }

        QPushButton#AnalyseButton {
            background-color: #2563eb;
            color: #ffffff;   
            font-size: 14px;
            font-weight: bold;
            border: none;                       
            border-radius: 8px;                 
            padding: 10px 24px;
        }
        QPushButton#AnalyseButton:hover {
            background-color: #1d4ed8;
        }
        QPushButton#AnalyseButton:pressed {
            background-color: #1e40af;          
        }
    )");

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
    //       -- PAGES --
    // -----------------------
    comboBox = new QComboBox();
    comboBox->addItem("Page 1");
    comboBox->addItem("Page 2");
    comboBox->setFixedSize(0, 0);

    page1 = new QWidget();
    page2 = new QWidget();

    // -----------------------
    // -- PAGE 1 COMPONENTS --
    // -----------------------

    titleLabel = new QLabel("X-Ray Analysis Suite", this);
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
    
    IdLabel = new QLabel("Patient ID");
    IdLabel->setObjectName("FieldLabel");
    
    IdInput = new QLineEdit();
    IdInput->setObjectName("InputField");
    
    viewLabel = new QLabel("Scan View");
    viewLabel->setObjectName("FieldLabel");

    viewInput = new QLineEdit();
    viewInput->setObjectName("InputField");
    viewInput->setPlaceholderText("AP or Lateral");

    // MODEL INFORMATION
    infoButton = new QPushButton(" Model Information");
    infoButton->setIcon(QIcon("/Users/miguel/GitHub/bone_tumour_classification/bone_tumour_app/assets/info.png"));
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
    infoMessageBox->setIconPixmap(QIcon("/Users/miguel/GitHub/bone_tumour_classification/bone_tumour_app/assets/info.png").pixmap(32, 32));

    QPushButton *closeButton = infoMessageBox->addButton("Acknowledge", QMessageBox::AcceptRole);
    closeButton->setStyleSheet("background-color: #2563eb; color: white; font-weight: bold; border-radius: 6px;");
    closeButton->setFixedSize(120, 36); 

    // ACTION BUTTONS
    uploadButton = new QPushButton(" Upload");
    uploadButton->setObjectName("UploadButton"); 
    uploadButton->setIcon(QIcon("/Users/miguel/GitHub/bone_tumour_classification/bone_tumour_app/assets/downloads.png"));
    uploadButton->setIconSize(QSize(16,16));
    uploadButton->setFixedHeight(45);

    analyseButton = new QPushButton(" Analyse");
    analyseButton->setObjectName("AnalyseButton");
    analyseButton->setIcon(QIcon("/Users/miguel/GitHub/bone_tumour_classification/bone_tumour_app/assets/search.png"));
    analyseButton->setFixedHeight(45);
    
    // -----------------------
    // -- PAGE 2 COMPONENTS --
    // -----------------------

    // IMAGE DETECTION ZONE
    detectionZone = new QFrame();
    detectionZone->setObjectName("DetectionZone");

    detectionContainer = new QLabel();
    detectionContainer->setStyleSheet("background-color: #000000; border-radius: 12px; border: 1px solid #E0E0E0;");
    detectionContainer->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    detectionContainer->setAlignment(Qt::AlignCenter);
    
    // RIGHT COLUMN / SIDEBAR
    page2Sidebar = new QWidget();
    page2Sidebar->setFixedWidth(320);

    // CARD 1: DETECTED TUMOUR SEVERITY
    severityCard = new QFrame();
    severityCard->setObjectName("Card");

    severityLabel = new QLabel();
    severityLabel->setAlignment(Qt::AlignCenter);
    severityLabel->setFixedHeight(36);
    severityLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    
    // CARD 2: MODEL REPORT
    reportCard = new QFrame();
    reportCard->setObjectName("Card");

    confidenceLabel = new QLabel();
    confidenceLabel->setStyleSheet("font-size: 24px; font-weight: 800; color: #2B76E3; border: none; background: transparent;");
    
    tumourNameLabel = new QLabel();
    tumourNameLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #1A202C; border: none; background: transparent;");

    // ACTION BUTTONS
    restartButton = new QPushButton(" New Scan");
    restartButton->setIcon(QIcon("/Users/miguel/GitHub/bone_tumour_classification/bone_tumour_app/assets/refresh.png"));
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
    QHBoxLayout *page1Layout = new QHBoxLayout(page1);
    page1Layout->setContentsMargins(20, 20, 20, 20);
    page1Layout->setSpacing(20);

    QVBoxLayout *dropZoneLayout = new QVBoxLayout(dropZone);
    dropZoneLayout->setContentsMargins(20, 40, 20, 20);
    dropZoneLayout->addStretch();
    dropZoneLayout->addWidget(dropLabel);
    dropZoneLayout->addStretch();
    dropZoneLayout->addWidget(formatLabel);

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
    detailLayout->addWidget(viewInput);

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

    page1Layout->addWidget(dropZone, 1);
    page1Layout->addWidget(page1Sidebar);


    // -----------------------
    //   -- PAGE 2 LAYOUT --
    // -----------------------
    QHBoxLayout *page2Layout = new QHBoxLayout(page2);
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
    severityLayout->addWidget(severityLabel);

    QVBoxLayout *reportLayout = new QVBoxLayout(reportCard);
    reportLayout->setContentsMargins(15, 15, 15, 15);
    reportLayout->setSpacing(8);
    reportLayout->addWidget(tumourNameLabel);
    reportLayout->addWidget(confidenceLabel);

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

    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(page1);
    stackedWidget->addWidget(page2);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setObjectName("centralWidget");
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    mainLayout->addWidget(stackedWidget);
    setCentralWidget(centralWidget);
}

void MainWindow::setupConnections()
{
    connect(infoButton, &QPushButton::clicked, infoMessageBox, &QMessageBox::exec);
    connect(comboBox, &QComboBox::currentIndexChanged,
            stackedWidget, &QStackedWidget::setCurrentIndex);
    connect(uploadButton, &QPushButton::clicked, this, &MainWindow::uploadButton_clicked);
    connect(analyseButton, &QPushButton::clicked, this, &MainWindow::analyseButton_clicked);
    connect(restartButton, &QPushButton::clicked, this, &MainWindow::restartButton_clicked);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::close);
}

MainWindow::~MainWindow(){}

void MainWindow::uploadButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this, tr("Open X-Ray Scan"), "", tr("Images (*.png *.jpg *.jpeg)")
    );

    if (filePath.isEmpty()) return;

    QPixmap medicalImage(filePath);

    if (medicalImage.isNull()){
        QMessageBox::warning(this, tr("Error"), tr("Failed to load image"));
        return;
    }

    DetectionResult report = classifier.predict(filePath);

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
    
    QString percentageStr = QString("%1%").arg(QString::number(report.confidence * 100, 'f', 1));
    confidenceLabel->setText(percentageStr);

    if (!report.boundingBox.isNull()){
        QPainter painter(&medicalImage);

        QPen pen;
        if (report.severity == "Malignant") {
            pen.setColor(Qt::red);
        } else if (report.severity == "Aggressive Benign") {
            pen.setColor(QColor(255, 165, 03)); 
        } else {
            pen.setColor(Qt::green);
        }

        pen.setWidth(4);
        painter.setPen(pen);

        painter.drawRect(report.boundingBox);
        painter.end();
    }

    int w = stackedWidget->width();
    int h = stackedWidget->height();

    detectionContainer->setPixmap(medicalImage.scaled(w - 100, h - 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::analyseButton_clicked()
{
    comboBox->setCurrentIndex(1);
}

void MainWindow::restartButton_clicked()
{
    comboBox->setCurrentIndex(0);
}