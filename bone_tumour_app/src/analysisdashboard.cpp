#include "analysisdashboard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QMenuBar>
#include <QFrame>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    this->setFixedSize(800, 600);
    this->setWindowTitle("Analysis Suite");
    this->setStyleSheet("background-color: #F8F9FA;");

    setupUIComponents();
    setupPages();
    setupConnections();

    if (!classifier.loadModel("bonetumour_model.onnx")){
        QMessageBox::critical(this, tr("Error"), tr("AI Model failed to load"));
    };

}

void MainWindow::setupUIComponents()
{
    titleLabel = new QLabel("X-Ray Analysis Suite", this);
    titleLabel->setStyleSheet("font-size: 20px; font-weight: bold; color: #010101;");
    titleLabel->setAlignment(Qt::AlignCenter);

    dragdropLabel = new QLabel(this);
    dragdropLabel->setText(
    "<p align='center'>"
    "  <img src='/Users/miguel/GitHub/bone_tumour_classification/bone_tumour_app/assets/drag-and-drop.png' width='48' height='48'>"
    "</p>"
    "<p align='center' style='font-size: 14px; margin-top: 10px; color: #555555;'>"
    "  Drag & Drop <span style='color: #2B76E3; text-decoration: underline;'>X-Ray file</span>"
    "</p>"
    );
    dragdropLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    dragdropLabel->setAlignment(Qt::AlignCenter);
    dragdropLabel->setStyleSheet(R"(
        background-color: #F1F6FE;
        color: #555555; 
        border: 2px dashed #2B76E3;
        border-radius: 8px;
        )");

    supportLabel = new QLabel("Supported formats: PNG, JPG, JPEG", this);
    supportLabel->setStyleSheet("color: #555555; ");

    infoButton = new QPushButton(" Model Information", this);
    infoButton->setIcon(QIcon("/Users/miguel/GitHub/bone_tumour_classification/bone_tumour_app/assets/info.png"));
    infoButton->setIconSize(QSize(16,16));
    infoButton->setFixedWidth(150);
    infoButton->setStyleSheet(R"(
    QPushButton {
        background-color: #F8F9FA;
        color: #555555;    
        border: none;                   
        border-radius: 8px;                 
        padding: 10px 10px;
    }
    QPushButton:hover {
        background-color: #F5F5F5; 
    }
    QPushButton:pressed {
        background-color: #EAEAEA;          
    }
    )");

    infoMessageBox = new QMessageBox(this);
    infoMessageBox->setWindowTitle("Model Information");
    infoMessageBox->setText(
        "<h3 style='color: #1A202C; margin-bottom: 4px; font-size: 16px;'>YOLOv8 Bone Tumour Classifier</h3>"
        "<p style='color: #888888; font-size: 11px; text-transform: uppercase; letter-spacing: 0.5px; margin-bottom: 12px;'>Onnx Runtime Model</p>"
        "<hr style='border: none; background-color: #E5E9F0; height: 1px; margin-bottom: 12px;'/>"
        "<p style='color: #555555; font-size: 13px; line-height: 18px;'>"
        "This suite executes computer vision classification using a deep learning model trained on publicly avaliable bone tumour data via Kaggle.<br><br>"
        "<b>Supported Types:</b> <ul> <li>Giant Cell Tumour</li> <li>Osteochondroma</li> <li>Osteosarcoma</li><li>Osteofibroma</li><li>Other Benign Tumour</li><li>Other Malignant Tumour</li>"
        "</p>"
        "<hr style='border: none; background-color: #E5E9F0; height: 1px; margin-bottom: 12px;'/>");
    infoMessageBox->setIconPixmap(QIcon("/Users/miguel/GitHub/bone_tumour_classification/bone_tumour_app/assets/info.png").pixmap(32, 32));

    QPushButton *closeBtn = infoMessageBox->addButton("Acknowledge", QMessageBox::AcceptRole);
    closeBtn->setStyleSheet("background-color: #2B76E3;");
    closeBtn->setFixedSize(120, 36);

    uploadButton = new QPushButton(" Upload", this);
    uploadButton->setIcon(QIcon("/Users/miguel/GitHub/bone_tumour_classification/bone_tumour_app/assets/downloads.png"));
    uploadButton->setIconSize(QSize(16,16));
    uploadButton->setFixedSize(200, 45);
    uploadButton->setStyleSheet(R"(
        QPushButton{
            background-color: #2B76E3;
            color: #FFFFFF;   
            font-size: 14px;
            font-weight: bold;
            border: none;                       
            border-radius: 8px;                 
            padding: 10px 24px;
        }
        QPushButton:hover{
            background-color: #1A62CD;
        }
        QPushButton:pressed {
            background-color: #154FA5;          
        }
    )");

    acceptButton = new QPushButton(" Accept File ", this);
    acceptButton->setIcon(QIcon("/Users/miguel/GitHub/bone_tumour_classification/bone_tumour_app/assets/angle-right.png"));
    acceptButton->setIconSize(QSize(16,16));
    acceptButton->setLayoutDirection(Qt::RightToLeft);
    acceptButton->setStyleSheet(R"(
    QPushButton {
        background-color: #2B76E3;
        color: #FFFFFF;    
        font-size: 14px;
        font-weight: bold;                    
        border-radius: 8px;               
        padding: 10px 28px; 
    }
    QPushButton:hover {
        background-color: #1A62CD; 
    }
    QPushButton:pressed {
        background-color: #154FA5;          
    }
    )");

    denyButton = new QPushButton(" File Selection ", this);
    denyButton->setIcon(QIcon("/Users/miguel/GitHub/bone_tumour_classification/bone_tumour_app/assets/refresh.png"));
    denyButton->setIconSize(QSize(16,16));
    denyButton->setStyleSheet(R"(
    QPushButton {
        background-color: #FFFFFF;
        color: #555555; 
        font-size: 14px;
        border: 1px solid #CCCCCC;                   
        border-radius: 8px;                 
        padding: 10px 28px;
    }
    QPushButton:hover {
        background-color: #F5F5F5; 
    }
    QPushButton:pressed {
        background-color: #EAEAEA;          
    }
    )");

    imageDisplayLabel = new QLabel(this);
    imageDisplayLabel->setStyleSheet("background-color: #000000; border-radius: 12px; border: 1px solid #E0E0E0;");
    imageDisplayLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    imageDisplayLabel->setAlignment(Qt::AlignCenter);

    imageFinalLabel = new QLabel(this);
    imageFinalLabel->setStyleSheet("background-color: #000000; border-radius: 12px; border: 1px solid #E0E0E0;");
    imageFinalLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    imageFinalLabel->setAlignment(Qt::AlignCenter);

    severityLabel = new QLabel(this);
    severityLabel->setAlignment(Qt::AlignCenter);
    severityLabel->setFixedHeight(36);
    severityLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    confidenceLabel = new QLabel(this);
    confidenceLabel->setStyleSheet("font-size: 24px; font-weight: 800; color: #2B76E3; border: none; background: transparent;");
    
    tumourNameLabel = new QLabel(this);
    tumourNameLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #1A202C; border: none; background: transparent;");

    restartButton = new QPushButton(" Analyse New Scan", this);
    restartButton->setIcon(QIcon("/Users/miguel/GitHub/bone_tumour_classification/bone_tumour_app/assets/refresh.png"));
    restartButton->setFixedHeight(40);
    restartButton->setStyleSheet(R"(
        QPushButton {
            background-color: #F1F6FE;
            color: #2B76E3;
            font-size: 13px;
            font-weight: bold;
            border: 1px solid #D2E3FC;
            border-radius: 6px;
            padding: 5px;
        }
        QPushButton:hover { background-color: #E1EEFE; }
        QPushButton:pressed { background-color: #D2E3FC; }
    )");

    exitButton = new QPushButton(" Close Suite", this);
    exitButton->setFixedHeight(40);
    exitButton->setStyleSheet(R"(
        QPushButton {
            background-color: #FFFFFF;
            color: #555555;
            font-size: 13px;
            border: 1px solid #CCCCCC;
            border-radius: 6px;
            padding: 5px;
        }
        QPushButton:hover { background-color: #F5F5F5; }
        QPushButton:pressed { background-color: #EAEAEA; }
    )");

}

void MainWindow::setupPages()
{
    comboBox = new QComboBox(this);
    comboBox->addItem("Page 1");
    comboBox->addItem("Page 2");
    comboBox->addItem("Page 3");
    comboBox->setFixedSize(0, 0);

    QWidget *page1 = new QWidget();
    QVBoxLayout *page1Layout = new QVBoxLayout(page1);
    QHBoxLayout *page1supportLayout = new QHBoxLayout(page1);
    page1supportLayout->addWidget(supportLabel);
    page1supportLayout->addWidget(infoButton, Qt::AlignRight);

    page1Layout->addWidget(titleLabel);
    page1Layout->addSpacing(10);
    page1Layout->addWidget(comboBox);
    page1Layout->addWidget(dragdropLabel);
    page1Layout->addSpacing(-12);
    page1Layout->addLayout(page1supportLayout);
    page1Layout->addSpacing(20);
    page1Layout->addWidget(uploadButton, 0, Qt::AlignRight); 
    page1Layout->setAlignment(Qt::AlignCenter);

    QWidget *page2 = new QWidget();
    QHBoxLayout *page2MainLayout = new QHBoxLayout(page2);
    page2MainLayout->setContentsMargins(24, 24, 24, 24);
    page2MainLayout->setSpacing(20);

    page2MainLayout->addWidget(imageDisplayLabel, 6);

    QFrame *page2RightPanelCard = new QFrame(page2);
    page2RightPanelCard->setStyleSheet(R"(
        QFrame {
            background-color: #FFFFFF;
            border-radius: 12px;
            border: 1px solid #E5E9F0;
        }
    )");

    QVBoxLayout *page2CardLayout = new QVBoxLayout(page2RightPanelCard);
    page2CardLayout->setContentsMargins(24, 24, 24, 24);
    page2CardLayout->setSpacing(16);

    QLabel *verificationHeader = new QLabel("Verify Scan Selection", page2RightPanelCard);
    verificationHeader->setStyleSheet("font-size: 14px; font-weight: bold; color: #888888; border: none; text-transform: uppercase; letter-spacing: 1px; background: transparent;");
    page2CardLayout->addWidget(verificationHeader);

    QLabel *page2StatusLabel = new QLabel("⚠️  PENDING CLINICAL CONFIRMATION", page2RightPanelCard);
    page2StatusLabel->setAlignment(Qt::AlignCenter);
    page2StatusLabel->setFixedHeight(36);
    page2StatusLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    page2StatusLabel->setStyleSheet("color: #B78103; background-color: #FFFDE7; font-weight: bold; font-size: 11px; border-radius: 18px; border: 1px solid #FFF9C4; padding: 5px;");
    page2CardLayout->addWidget(page2StatusLabel);

    QFrame *page2Line = new QFrame(page2RightPanelCard);
    page2Line->setFrameShape(QFrame::HLine);
    page2Line->setFrameShadow(QFrame::Sunken);
    page2Line->setStyleSheet("background-color: #E5E9F0; border: none; min-height: 1px; max-height: 1px;");
    page2CardLayout->addWidget(page2Line);

    QLabel *page2DescLabel = new QLabel("Confirm that the loaded asset matches a valid radiography scan layout before passing image to the detection pipeline.", page2RightPanelCard);
    page2DescLabel->setWordWrap(true);
    page2DescLabel->setStyleSheet("font-size: 13px; color: #555555; border: none; background: transparent; line-height: 18px;");
    page2CardLayout->addWidget(page2DescLabel);

    QLabel *pipelineTitleLabel = new QLabel("Targeted Model Pipeline:", page2RightPanelCard);
    pipelineTitleLabel->setStyleSheet("font-size: 12px; color: #888888; border: none; background: transparent; margin-top: 10px;");
    page2CardLayout->addWidget(pipelineTitleLabel);

    QLabel *pipelineValueLabel = new QLabel("YOLOv8 Bone Tumour Classifier", page2RightPanelCard);
    pipelineValueLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #1A202C; border: none; background: transparent;");
    page2CardLayout->addWidget(pipelineValueLabel);

    page2CardLayout->addStretch();

    QHBoxLayout *page2DecisionLayout = new QHBoxLayout();
    page2DecisionLayout->setSpacing(12);
    page2DecisionLayout->addWidget(acceptButton);
    page2DecisionLayout->addWidget(denyButton);
    page2CardLayout->addLayout(page2DecisionLayout);

    page2MainLayout->addWidget(page2RightPanelCard, 4);

    QWidget *page3 = new QWidget();
    QHBoxLayout *page3MainLayout = new QHBoxLayout(page3);
    page3MainLayout->setContentsMargins(24, 24, 24, 24);
    page3MainLayout->setSpacing(20);

    page3MainLayout->addWidget(imageFinalLabel, 6);

    QFrame *rightPanelCard = new QFrame(page3);
    rightPanelCard->setStyleSheet(R"(
        QFrame {
            background-color: #FFFFFF;
            border-radius: 12px;
            border: 1px solid #E5E9F0;
        }
    )");
    
    QVBoxLayout *cardLayout = new QVBoxLayout(rightPanelCard);
    cardLayout->setContentsMargins(24, 24, 24, 24);
    cardLayout->setSpacing(16);

    QLabel *analysisHeader = new QLabel("Diagnostic Assessment", rightPanelCard);
    analysisHeader->setStyleSheet("font-size: 14px; font-weight: bold; color: #888888; border: none; text-transform: uppercase; letter-spacing: 1px; background: transparent;");
    cardLayout->addWidget(analysisHeader);
    cardLayout->addWidget(severityLabel);

    QFrame *line = new QFrame(rightPanelCard);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setStyleSheet("background-color: #E5E9F0; border: none; min-height: 1px; max-height: 1px;");
    cardLayout->addWidget(line);

    QGridLayout *metricsGrid = new QGridLayout();
    metricsGrid->setSpacing(12);

    QLabel *classTitleLabel = new QLabel("Detected Pathology:", rightPanelCard);
    classTitleLabel->setStyleSheet("font-size: 13px; color: #666666; border: none; background: transparent;");
    
    QLabel *confTitleLabel = new QLabel("Model Confidence:", rightPanelCard);
    confTitleLabel->setStyleSheet("font-size: 13px; color: #666666; border: none; background: transparent;");

    metricsGrid->addWidget(classTitleLabel, 0, 0);
    metricsGrid->addWidget(tumourNameLabel, 1, 0);
    metricsGrid->addWidget(confTitleLabel, 2, 0);
    metricsGrid->addWidget(confidenceLabel, 3, 0);
    
    cardLayout->addLayout(metricsGrid);
    cardLayout->addStretch();

    QHBoxLayout *actionButtonLayout = new QHBoxLayout();
    actionButtonLayout->setSpacing(12);
    actionButtonLayout->addWidget(restartButton);
    actionButtonLayout->addWidget(exitButton);
    cardLayout->addLayout(actionButtonLayout);

    page3MainLayout->addWidget(rightPanelCard, 4);

    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(page1);
    stackedWidget->addWidget(page2);
    stackedWidget->addWidget(page3);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    mainLayout->addWidget(stackedWidget);
    
    setCentralWidget(centralWidget);

}

void MainWindow::setupConnections()
{
    connect(infoButton, &QPushButton::clicked, infoMessageBox, &QMessageBox::exec);
    connect(comboBox, &QComboBox::currentIndexChanged,
            stackedWidget, &QStackedWidget::setCurrentIndex);
    connect(acceptButton, &QPushButton::clicked, this, &MainWindow::acceptButton_clicked);
    connect(denyButton, &QPushButton::clicked, this, &MainWindow::denyButton_clicked);
    connect(uploadButton, &QPushButton::clicked, this, &MainWindow::uploadButton_clicked);
    connect(restartButton, &QPushButton::clicked, this, &MainWindow::denyButton_clicked);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::close);
}

MainWindow::~MainWindow(){}

void MainWindow::uploadButton_clicked()
{
    filePath = QFileDialog::getOpenFileName(
        this, tr("Open X-Ray Scan"), "", tr("Images (*.png *.jpg *.jpeg)")
    );

    if (filePath.isEmpty()) return;

    QPixmap medicalImage(filePath);

    if (medicalImage.isNull()){
        QMessageBox::warning(this, tr("Error"), tr("Failed to load image"));
        return;
    }

    int w = stackedWidget->width();
    int h = stackedWidget->height();
    imageDisplayLabel->setPixmap(medicalImage.scaled(w - 100, h - 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    imageFinalLabel->setPixmap(medicalImage.scaled(w - 100, h - 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    comboBox->setCurrentIndex(1);
}

void MainWindow::acceptButton_clicked()
{
    if (filePath.isEmpty()) return;

    DetectionResult report = classifier.predict(filePath);

    if (!report.success) {
        severityLabel->setText("System Error");
        severityLabel->setStyleSheet("color: #FFFFFF; background-color: #D32F2F; font-weight: bold; font-size: 13px; border-radius: 4px; border: none;");
        return;
    }

    if (report.severity == "Malignant") {
        severityLabel->setText("🔴  CRITICAL MALIGNANT FINDING");
        severityLabel->setStyleSheet("color: #D32F2F; background-color: #FFEBEE; font-weight: bold; font-size: 12px; border-radius: 18px; border: 1px solid #FFCDD2; padding: 5px;");
    } else if (report.severity == "Aggressive Benign") {
        severityLabel->setText("🟡  WARNING: LOCALLY AGGRESSIVE");
        severityLabel->setStyleSheet("color: #B78103; background-color: #FFFDE7; font-weight: bold; font-size: 12px; border-radius: 18px; border: 1px solid #FFF9C4; padding: 5px;");
    } else if (report.severity == "Benign") {
        severityLabel->setText("🟢  BENIGN CONDITION DETECTED");
        severityLabel->setStyleSheet("color: #1E4620; background-color: #E8F5E9; font-weight: bold; font-size: 12px; border-radius: 18px; border: 1px solid #C8E6C9; padding: 5px;");
    } else {
        severityLabel->setText("🔵  NO TUMOUR DETECTED");
        severityLabel->setStyleSheet("color: #0D47A1; background-color: #E3F2FD; font-weight: bold; font-size: 12px; border-radius: 18px; border: 1px solid #BBDEFB; padding: 5px;");
    }

    tumourNameLabel->setText(report.className);
    
    QString percentageStr = QString("%1%").arg(QString::number(report.confidence * 100, 'f', 1));
    confidenceLabel->setText(percentageStr);

    comboBox->setCurrentIndex(2);
}

void MainWindow::denyButton_clicked()
{
    comboBox->setCurrentIndex(0);
}