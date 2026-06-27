#include "analysisdashboard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QPixmap>
#include <QPainter>
#include <QBuffer>
#include <QApplication>
#include <QPrinter>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>
#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent)
{
    this->resize(1280, 800);
    this->setWindowTitle("Analysis Suite");
    setupComponents();
    setupLayouts();
    setupConnections();
    initialiseModel();
    QApplication::setCursorFlashTime(0);
}

void MainWindow::setupComponents()
{  
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

    titleLabel = new QLabel();
    titleLabel->setPixmap(QPixmap(":/SOLIS-xray.png").scaledToWidth(250, Qt::SmoothTransformation));
    titleLabel->setObjectName("TitleLabel");
    titleLabel->setAlignment(Qt::AlignCenter);

    separatorTitlePage1 = new QFrame();
    separatorTitlePage1->setFrameShape(QFrame::HLine);
    separatorTitlePage1->setFrameShadow(QFrame::Sunken);
    separatorTitlePage1->setObjectName("SidebarDivider");

    page1Leftbar = new QWidget();

    containerLabel = new QLabel("PREVIEW REGION");
    containerLabel->setObjectName("CardTitle");

    separatorContainer = new QFrame();
    separatorContainer->setFrameShape(QFrame::HLine);
    separatorContainer->setFrameShadow(QFrame::Sunken);
    separatorContainer->setObjectName("SidebarDivider");

    dropZone = new QFrame();
    dropZone->setObjectName("DropZone");
    dropZone->setAcceptDrops(true);
    dropZone->installEventFilter(this);
    
    dropLabel = new QLabel();
    dropLabel->setText(
    "<p align='center'>"
    "  <img src=':/drag-and-drop.png' width='48' height='48'>"
    "</p>"
    "<p align='center' style='font-size: 16px; font-weight: bold; margin-top: 10px; color: #E2E8F0;'>"
    "  Drag & Drop <span style='color: #2563eb; text-decoration: underline;'>X-Ray file</span>"
    "</p>"
    "<p align='center' style='font-size: 16px; font-weight: bold; margin-top: 10px; color: #E2E8F0;'>"
    " or "
    "</p>"
    );
    dropLabel->setAlignment(Qt::AlignCenter);

    uploadButton = new QPushButton(" Upload");
    uploadButton->setObjectName("ActionButton"); 
    uploadButton->setIcon(QIcon(":/downloads.png"));
    uploadButton->setIconSize(QSize(16,16));
    uploadButton->setFixedHeight(45);
    uploadButton->setFixedWidth(135);
    uploadButton->setCursor(Qt::PointingHandCursor);

    formatLabel = new QLabel("Supported formats: PNG, JPG, JPEG");
    formatLabel->setObjectName("FormatText"); 
    formatLabel->setAlignment(Qt::AlignCenter);

    pathLabel = new QLabel();
    pathLabel->setObjectName("CardText");

    testScan = new QPushButton("Onboarding");
    testScan->setObjectName("TestButton");
    testScan->setIcon(QIcon(":/onboard.png"));
    testScan->setIconSize(QSize(16,16));
    testScan->setCursor(Qt::PointingHandCursor);

    previousId = new QPushButton("Last Patient");
    previousId->setObjectName("PreviousButton");
    previousId->setIcon(QIcon(":/id.png"));
    previousId->setIconSize(QSize(16,16));
    previousId->setCursor(Qt::PointingHandCursor);
    previousId->setEnabled(false);

    previewZone = new QFrame();
    previewZone->setObjectName("DetectionZone");

    previewContainer = new QLabel();
    previewContainer->setMinimumSize(100, 100);
    previewContainer->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    previewContainer->setAlignment(Qt::AlignCenter);

    page1Sidebar = new QWidget();
    page1Sidebar->setFixedWidth(320);

    statusCard = new QFrame();
    statusCard->setObjectName("Card"); 
    
    statusTitle = new QLabel("SYSTEM STATUS");
    statusTitle->setObjectName("CardTitle");
    
    modelLabel = new QLabel("Model:<span style = 'font-weight:bold;'> BoneTumourClassifier v2.1 </span>");
    modelLabel->setObjectName("CardText");
    
    statusLabel = new QLabel("Status: <span style = 'color: #FFB703; font-weight: bold; font-size: 14px;'> WAITING </span><img src=':/hourglass.png' height='12' width='12'>");
    statusLabel->setObjectName("CardText");

    detailCard = new QFrame();
    detailCard->setObjectName("Card"); 

    detailTitle = new QLabel("PRE-ANALYSIS CONTEXT");
    detailTitle->setObjectName("CardTitle");
    
    IdLabel = new QLabel("<sup style = 'color: #ff0000;'>*</sup> Patient ID");
    IdLabel->setObjectName("FieldLabel");
    
    IdInput = new QLineEdit();
    IdInput->setObjectName("InputField");
    IdInput->setPlaceholderText("000-000-0000");
    IdInput->setInputMask("000-000-0000;-");
    
    viewLabel = new QLabel("<sup style = 'color: #ff0000;'>*</sup> Scan View");
    viewLabel->setObjectName("FieldLabel");

    Frontview = new QRadioButton("Front");
    Frontview->setObjectName("InputField");
    Frontview->setCursor(Qt::PointingHandCursor);

    Lateralview = new QRadioButton("Lateral");
    Lateralview->setObjectName("InputField");
    Lateralview->setCursor(Qt::PointingHandCursor);

    advDetailCard = new QFrame();
    advDetailCard->setObjectName("Card");

    advDetailTitle = new QPushButton("Advanced Options +");
    advDetailTitle->setObjectName("CardTitle");
    advDetailTitle->setCursor(Qt::PointingHandCursor);

    clinicalNotesTitle = new QLabel("Clinical Notes");
    clinicalNotesTitle->setObjectName("FieldLabel");
    clinicalNotesTitle->setVisible(false);

    clinicalNotes = new QTextEdit();
    clinicalNotes->setObjectName("ClinicalNotes");
    clinicalNotes->setPlaceholderText("Enter clinical notes here...");
    clinicalNotes->setVisible(false);

    infoButton = new QPushButton(" Model Information");
    infoButton->setIcon(QIcon(":/info.png"));
    infoButton->setIconSize(QSize(16,16));
    infoButton->setFixedWidth(150);
    infoButton->setObjectName("InfoButton");
    infoButton->setCursor(Qt::PointingHandCursor);

    infoMessageBox = new QMessageBox();
    infoMessageBox->setObjectName("InfoMessageBox");
    infoMessageBox->setWindowTitle("Model Information");
    infoMessageBox->setText(
    "<div style='min-width: 450px; font-family: \".AppleSystemUIFont\", \"Helvetica Neue\", Arial, sans-serif; color: #94A3B8; line-height: 1.5;'>"
        "<div style='display: flex; flex-direction: column; gap: 4px;'>"
            "<h2 style='margin: 0; color: #F8FAFC; font-size: 20px; font-weight: 700; letter-spacing: -0.3px;'>YOLOv8 Bone Tumour Classifier</h2>"
            "<div style='margin-top: 4px;'>"
                "<span style='background-color: rgba(0, 104, 255, 0.15); color: #38BDF8; font-size: 11px; font-weight: 700; padding: 4px 10px; border-radius: 12px; border: 1px solid rgba(56, 189, 248, 0.2); letter-spacing: 0.5px; text-transform: uppercase;'>"
                    "ONNX Runtime Engine"
                "</span>"
            "</div>"
        "</div>"
        
        "<hr style='border: none; background-color: #1E293B; height: 1px; margin: 16px 0;'/>"
        
        "<p style='font-size: 13px; color: #94A3B8; margin: 0;'>"
            "This suite executes computer vision classification using a deep learning model trained on publicly available bone tumour data via Kaggle."
        "</p>"
        
        "<p style='font-size: 12px; font-weight: 700; color: #E2E8F0; margin: 20px 0 8px 0; text-transform: uppercase; letter-spacing: 0.5px;'>"
            "Supported Pathology Targets"
        "</p>"
        
        "<ul style='margin: 0; padding-left: 18px; font-size: 13px; line-height: 1.7; color: #CBD5E1;'>"
            "<li style='margin-bottom: 4px;'><strong style='color: #F1F5F9;'>Giant Cell Tumour</strong></li>"
            "<li style='margin-bottom: 4px;'><strong style='color: #F1F5F9;'>Osteochondroma</strong></li>"
            "<li style='margin-bottom: 4px;'><strong style='color: #F1F5F9;'>Osteosarcoma</strong></li>"
            "<li style='margin-bottom: 4px;'><strong style='color: #F1F5F9;'>Osteofibroma</strong></li>"
            "<li style='margin-bottom: 4px;'><strong style='color: #94A3B8;'>Other Benign Variations</strong></li>"
            "<li style='margin-bottom: 4px;'><strong style='color: #94A3B8;'>Other Malignant Formations</strong></li>"
        "</ul>"
        
        "<hr style='border: none; background-color: #1E293B; height: 1px; margin: 18px 0 0 0;'/>"
    "</div>"
    );
    infoMessageBox->setIconPixmap(QIcon(":/info.png").pixmap(32, 32));

    separatorActionPage1 = new QFrame();
    separatorActionPage1->setFrameShape(QFrame::HLine);
    separatorActionPage1->setFrameShadow(QFrame::Sunken);
    separatorActionPage1->setObjectName("SidebarDivider");

    clearButton = new QPushButton(" Clear");
    clearButton->setObjectName("ActionButton"); 
    clearButton->setFixedHeight(45);
    clearButton->setCursor(Qt::PointingHandCursor);
    clearButton->setIcon(QIcon(":/bin.png"));
    clearButton->setIconSize(QSize(16,16));
    clearButton->setEnabled(false);

    analyseButton = new QPushButton(" Analyse");
    analyseButton->setObjectName("AnalyseButton");
    analyseButton->setIcon(QIcon(":/search.png"));
    analyseButton->setFixedHeight(45);
    analyseButton->setEnabled(false);
    analyseButton->setCursor(Qt::PointingHandCursor);
    
    detectionZone = new QFrame();
    detectionZone->setObjectName("DetectionZone");

    detectionContainer = new QLabel();
    detectionContainer->setMinimumSize(100, 100);
    detectionContainer->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    detectionContainer->setAlignment(Qt::AlignCenter);
    
    page2Sidebar = new QWidget();
    page2Sidebar->setFixedWidth(320);
    
    reportCard = new QFrame();
    reportCard->setObjectName("Card");

    confidenceTitle = new QLabel("MODEL CONFIDENCE");
    confidenceTitle->setObjectName("CardTitle");

    confidenceValueLabel = new QLabel();
    confidenceValueLabel->setObjectName("ConfidenceValueLabel");

    confidenceBar = new QProgressBar();
    confidenceBar->setObjectName("ConfidenceBar");
    confidenceBar->setOrientation(Qt::Horizontal);
    confidenceBar->setRange(0, 100);
    confidenceBar->setTextVisible(false);
    
    tumourNameTitle = new QLabel("TUMOUR CATEGORY");
    tumourNameTitle->setObjectName("CardTitle");
    tumourNameLabel = new QLabel();
    tumourNameLabel->setStyleSheet("font-size: 30px; font-weight: bold; color: #E2E8F0; border: none; background: transparent;");

    separatorReportPage2 = new QFrame();
    separatorReportPage2->setFrameShape(QFrame::HLine);
    separatorReportPage2->setFrameShadow(QFrame::Sunken);
    separatorReportPage2->setObjectName("SidebarDivider");

    severityCard = new QFrame();
    severityCard->setObjectName("Card");

    severityTitle = new QLabel("CASE SEVERITY");
    severityTitle->setObjectName("CardTitle");
    severityLabel = new QLabel();
    severityLabel->setObjectName("SeverityLabel");
    severityLabel->setAlignment(Qt::AlignCenter);
    severityLabel->setFixedHeight(40);
    severityLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    separatorActionPage2 = new QFrame();
    separatorActionPage2->setFrameShape(QFrame::HLine);
    separatorActionPage2->setFrameShadow(QFrame::Sunken);
    separatorActionPage2->setObjectName("SidebarDivider");

    verificationCard = new QFrame();
    verificationCard->setObjectName("Card");

    verificationTitle = new QLabel("<sup style = 'color: #ff0000;'>*</sup> CLINICAN VERIFICATION");
    verificationTitle->setObjectName("CardTitle");

    approveButton = new QPushButton("Approve");
    approveButton->setObjectName("ApproveButton");
    approveButton->setCheckable(true);
    approveButton->setCursor(Qt::PointingHandCursor);

    editDiagnosis = new QPushButton(" Edit Diagnosis  ");
    editDiagnosis->setObjectName("ActionButton");
    editDiagnosis->setIcon(QIcon(":/edit.png"));
    editDiagnosis->setIconSize(QSize(16,16));
    editDiagnosis->setLayoutDirection(Qt::RightToLeft);
    editDiagnosis->setCursor(Qt::PointingHandCursor);

    exportCard = new QFrame();
    exportCard->setObjectName("Card");

    exportTitle = new QLabel("EXPORT");
    exportTitle->setObjectName("CardTitle");

    pdfButton = new QPushButton(" Generate Report");
    pdfButton->setObjectName("ActionButton");
    pdfButton->setIcon(QIcon(":/report.png"));
    pdfButton->setIconSize(QSize(16,16));
    pdfButton->setEnabled(false);

    restartButton = new QPushButton(" New Scan");
    restartButton->setIcon(QIcon(":/refresh.png"));
    restartButton->setFixedHeight(40);
    restartButton->setObjectName("AnalyseButton");
    restartButton->setCursor(Qt::PointingHandCursor);

    exitButton = new QPushButton(" Close Suite");
    exitButton->setFixedHeight(40);
    exitButton->setObjectName("ActionButton");
    exitButton->setCursor(Qt::PointingHandCursor);
}

void MainWindow::setupLayouts()
{
    QHBoxLayout *page1Layout = new QHBoxLayout(inputPage);
    page1Layout->setContentsMargins(20, 20, 20, 20);
    page1Layout->setSpacing(20);

    QVBoxLayout *dropZoneLayout = new QVBoxLayout(dropZone);
    dropZoneLayout->setContentsMargins(20, 20, 20, 20);
    dropZoneLayout->addStretch();
    dropZoneLayout->addWidget(dropLabel);
    dropZoneLayout->addWidget(uploadButton, 0 ,Qt::AlignHCenter);
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
    statusLayout->addWidget(titleLabel);
    statusLayout->addWidget(separatorTitlePage1);
    statusLayout->addWidget(statusTitle);
    statusLayout->addWidget(modelLabel);
    statusLayout->addWidget(statusLabel);

    QVBoxLayout *detailLayout = new QVBoxLayout(detailCard);
    detailLayout->setContentsMargins(15, 15, 15, 15);
    detailLayout->setSpacing(8);
    detailLayout->addWidget(detailTitle);
    detailLayout->addWidget(IdLabel);
    detailLayout->addWidget(IdInput);
    detailLayout->addSpacing(10);
    detailLayout->addWidget(viewLabel);
    
    QHBoxLayout *radioLayout = new QHBoxLayout();
    radioLayout->setSpacing(20);
    radioLayout->addWidget(Frontview);
    radioLayout->addWidget(Lateralview);
    radioLayout->addStretch();

    detailLayout->addLayout(radioLayout);

    QVBoxLayout *advDetailLayout = new QVBoxLayout(advDetailCard);
    advDetailLayout->setContentsMargins(15, 15, 15, 15);
    advDetailLayout->setSpacing(8);
    advDetailLayout->addWidget(advDetailTitle);
    advDetailLayout->addWidget(clinicalNotesTitle);
    advDetailLayout->addWidget(clinicalNotes);

    page1SidebarLayout->addWidget(statusCard);
    page1SidebarLayout->addWidget(detailCard);
    page1SidebarLayout->addWidget(advDetailCard);
    page1SidebarLayout->addWidget(infoButton);
    page1SidebarLayout->addStretch();
    page1SidebarLayout->addWidget(separatorActionPage1);

    QHBoxLayout *page1ActionLayout = new QHBoxLayout();
    page1ActionLayout->setSpacing(10);
    page1ActionLayout->addWidget(analyseButton);
    page1ActionLayout->addWidget(clearButton);
    page1SidebarLayout->addLayout(page1ActionLayout);

    imageStageStack = new AnimatedStackedWidget();
    imageStageStack->addWidget(dropZone);
    imageStageStack->addWidget(previewZone);

    QHBoxLayout *page1HLeftbarLayout = new QHBoxLayout();
    page1HLeftbarLayout->addWidget(pathLabel);
    page1HLeftbarLayout->addStretch();
    page1HLeftbarLayout->addWidget(testScan);
    page1HLeftbarLayout->addWidget(previousId);

    QVBoxLayout *page1LeftbarLayout = new QVBoxLayout(page1Leftbar);
    page1LeftbarLayout->setContentsMargins(0, 0, 0, 0);
    page1LeftbarLayout->setSpacing(10);
    page1LeftbarLayout->addWidget(containerLabel);
    page1LeftbarLayout->addWidget(separatorContainer);
    page1LeftbarLayout->addWidget(imageStageStack);
    page1LeftbarLayout->addLayout(page1HLeftbarLayout);

    page1Layout->addWidget(page1Leftbar, 1);
    page1Layout->addWidget(page1Sidebar);

    QHBoxLayout *page2Layout = new QHBoxLayout(assessmentPage);
    page2Layout->setContentsMargins(20, 20, 20, 20);
    page2Layout->setSpacing(20);

    QVBoxLayout *detectionLayout = new QVBoxLayout(detectionZone);
    detectionLayout->setContentsMargins(20, 20, 20, 20);
    detectionLayout->addWidget(detectionContainer);

    QVBoxLayout *page2SidebarLayout = new QVBoxLayout(page2Sidebar);
    page2SidebarLayout->setContentsMargins(0, 0, 0, 0);
    page2SidebarLayout->setSpacing(15);

    QFrame *resultsPanel = new QFrame();
    resultsPanel->setObjectName("ResultsPanel");
    QVBoxLayout *resultsPanelLayout = new QVBoxLayout(resultsPanel);
    resultsPanelLayout->setContentsMargins(0, 0, 0, 0);
    resultsPanelLayout->setSpacing(15);

    QVBoxLayout *reportLayout = new QVBoxLayout(reportCard);
    reportLayout->setContentsMargins(15, 15, 15, 15);
    reportLayout->setSpacing(8);
    reportLayout->addWidget(tumourNameTitle);
    reportLayout->addWidget(tumourNameLabel);
    reportLayout->addSpacing(15);
    
    QHBoxLayout *confidenceHeaderLayout = new QHBoxLayout();
    confidenceHeaderLayout->addWidget(confidenceTitle);
    confidenceHeaderLayout->addStretch();
    confidenceHeaderLayout->addWidget(confidenceValueLabel);
    reportLayout->addLayout(confidenceHeaderLayout);
    reportLayout->addWidget(confidenceBar);

    QVBoxLayout *severityLayout = new QVBoxLayout(severityCard);
    severityLayout->setContentsMargins(15, 15, 15, 15);
    severityLayout->setSpacing(8);
    severityLayout->addWidget(severityTitle);
    severityLayout->addWidget(severityLabel);

    resultsPanelLayout->addWidget(reportCard);
    resultsPanelLayout->addWidget(separatorReportPage2);
    resultsPanelLayout->addWidget(severityCard);

    QFrame *actionsPanel = new QFrame();
    actionsPanel->setObjectName("ActionsPanel");
    QVBoxLayout *actionsPanelLayout = new QVBoxLayout(actionsPanel);
    actionsPanelLayout->setContentsMargins(0, 0, 0, 0);
    actionsPanelLayout->setSpacing(15);

    QVBoxLayout *verificationLayout = new QVBoxLayout(verificationCard);
    verificationLayout->setContentsMargins(15, 15, 15, 15);
    verificationLayout->setSpacing(8);
    verificationLayout->addWidget(verificationTitle);

    QHBoxLayout *editLayout = new QHBoxLayout();
    editLayout->setSpacing(10);
    editLayout->addWidget(approveButton);
    editLayout->addWidget(editDiagnosis);

    verificationLayout->addLayout(editLayout);

    QVBoxLayout *exportLayout = new QVBoxLayout(exportCard);
    exportLayout->setContentsMargins(15, 15, 15, 15);
    exportLayout->setSpacing(8);
    exportLayout->addWidget(exportTitle);
    exportLayout->addWidget(pdfButton);

    actionsPanelLayout->addWidget(verificationCard);
    actionsPanelLayout->addWidget(exportCard);

    page2SidebarLayout->addWidget(resultsPanel);
    page2SidebarLayout->addStretch();
    page2SidebarLayout->addWidget(actionsPanel);
    page2SidebarLayout->addWidget(separatorActionPage2);

    QHBoxLayout *page2ActionLayout = new QHBoxLayout();
    page2ActionLayout->setSpacing(10);
    page2ActionLayout->addWidget(restartButton, 1);
    page2ActionLayout->addWidget(exitButton, 1);
    page2SidebarLayout->addLayout(page2ActionLayout);

    page2Layout->addWidget(detectionZone, 1);
    page2Layout->addWidget(page2Sidebar);

    mainViewStack = new AnimatedStackedWidget();
    mainViewStack->addWidget(inputPage);
    mainViewStack->addWidget(assessmentPage);

    QWidget *centralWidget = new QWidget();
    centralWidget->setObjectName("centralWidget");
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    mainLayout->addWidget(mainViewStack);
    setCentralWidget(centralWidget);
}

void MainWindow::setupConnections()
{
    connect(infoButton, &QPushButton::clicked, infoMessageBox, &QMessageBox::exec);
    connect(editDiagnosis, &QPushButton::clicked, this, &MainWindow::editDiagnosis_clicked);
    connect(mainViewRouter, &QComboBox::currentIndexChanged, [this](int index) {
        mainViewStack->fadeToIndex(index);
    });
    
    connect(imageStageRouter, &QComboBox::currentIndexChanged, [this](int index) {
        imageStageStack->fadeToIndex(index);
    });
    connect(uploadButton, &QPushButton::clicked, this, &MainWindow::uploadButton_clicked);
    connect(analyseButton, &QPushButton::clicked, this, &MainWindow::analyseButton_clicked);
    connect(restartButton, &QPushButton::clicked, this, &MainWindow::restartButton_clicked);
    connect(clearButton, &QPushButton::clicked, this, &MainWindow::restartButton_clicked);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::close);
    connect(advDetailTitle, &QPushButton::clicked, this, &MainWindow::advDetailButton_clicked);
    connect(IdInput, &QLineEdit::textChanged, this, &MainWindow::analyseButton_state);
    connect(Frontview, &QRadioButton::toggled, this, &MainWindow::analyseButton_state);
    connect(Lateralview, &QRadioButton::toggled, this, &MainWindow::analyseButton_state);
    connect(previousId, &QPushButton::clicked, this, &MainWindow::lastPatient_ID);
    connect(approveButton, &QPushButton::toggled, this, [this](bool checked) {
    if (checked) {
        approveButton->setText(" Approved");
        approveButton->setIcon(QIcon(":/check.png"));
        approveButton->setIconSize(QSize(16,16));
        editDiagnosis->setVisible(false);
        pdfButton->setEnabled(true);
    } else {
        approveButton->setText("Approve");
        approveButton->setIcon(QIcon());
        editDiagnosis->setVisible(true);
        pdfButton->setEnabled(false);
    }
    });
    connect(pdfButton, &QPushButton::clicked, this, &MainWindow::generateReport_pdf);
}

void MainWindow::initialiseModel()
{
    if (!classifier.loadModel("bonetumour_model.onnx")){
        QMessageBox::critical(this, tr("Error"), tr("AI Model failed to load"));
        this->close();
    };
}

MainWindow::~MainWindow(){}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == dropZone) {
        if (event->type() == QEvent::DragEnter) {
            QDragEnterEvent *dragEvent = static_cast<QDragEnterEvent*>(event);
            if (dragEvent->mimeData()->hasUrls()) {
                dragEvent->acceptProposedAction();

                dropZone->setProperty("dragHover", true);
                dropZone->style()->unpolish(dropZone);
                dropZone->style()->polish(dropZone);

                return true;
            }
        }
        else if (event->type() == QEvent::DragLeave) {
            dropZone->setProperty("dragHover", false);
            dropZone->style()->unpolish(dropZone);
            dropZone->style()->polish(dropZone);
            return true;
        }
        else if (event->type() == QEvent::Drop) {
            QDropEvent *dropEvent = static_cast<QDropEvent*>(event);
            const QMimeData *mimeData = dropEvent->mimeData();

            if (mimeData->hasUrls() && !mimeData->urls().isEmpty()) {
                QString droppedFilePath = mimeData->urls().first().toLocalFile();
                QFileInfo fileInfo(droppedFilePath);
                QString ext = fileInfo.suffix().toLower();

                if (ext == "png" || ext == "jpg" || ext == "jpeg") {
                    dropEvent->acceptProposedAction();
                    
                    this->filePath = droppedFilePath;
                    pathLabel->setText(filePath);
                    analyseButton_state();

                    QPixmap inputPixmap(filePath);
                    if (!inputPixmap.isNull()) {
                        zoneSize = dropZone->size();
                        previewContainer->setPixmap(inputPixmap.scaled(zoneSize.width(), zoneSize.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                        statusLabel->setText("Status: <span style = 'color: #2A9D8F; font-weight: bold;'> READY </span>");
                        clearButton->setEnabled(true);
                        imageStageRouter->setCurrentIndex(1);
                    } else {
                        QMessageBox::warning(this, tr("Error"), tr("Failed to load dropped image"));
                    }
                } else {
                    QMessageBox::warning(this, tr("Unsupported Format"), tr("Please drop a PNG, JPG, or JPEG file."));
                }
                return true;
            }
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::uploadButton_clicked()
{
    clearButton->setEnabled(true);
    filePath = QFileDialog::getOpenFileName(
        this, tr("Open X-Ray Scan"), "", tr("Images (*.png *.jpg *.jpeg)")
    );

    if (filePath.isEmpty()) return;
    analyseButton_state();

    pathLabel->setText(filePath);

    QPixmap inputPixmap(filePath);

    if (inputPixmap.isNull()){
        QMessageBox::warning(this, tr("Error"), tr("Failed to load image"));
        return;
    }

    zoneSize = dropZone->size();
    previewContainer->setPixmap(inputPixmap.scaled(zoneSize.width(), zoneSize.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    statusLabel->setText("Status: <span style = 'color: #2A9D8F; font-weight: bold; font-size: 14;'> READY </span><img src=':/checked.png' height='12' width='12'>");
    imageStageRouter->setCurrentIndex(1);
}

void MainWindow::analyseButton_clicked()
{
    DetectionResult report = classifier.predict(filePath);
    inputPixmap = QPixmap(filePath);
    QPixmap workingPixmap = inputPixmap;

    if (!report.success) {
        severityLabel->setText("System Error");
        return;
    }

    if (!report.boundingBox.isNull()){
        QPainter painter(&workingPixmap);
        QPen boxPen;

        QString statusValue = "";

        if (report.severity == "Malignant") {
            severityLabel->setText("  CRITICAL MALIGNANT FINDING");
            statusValue = "Malignant";
            boxPen.setColor(QColor("#E63946"));

        } else if (report.severity == "Aggressive Benign") {
            severityLabel->setText("  WARNING: LOCALLY AGGRESSIVE");
            statusValue = "Local-Aggressive";
            boxPen.setColor(QColor("#FFB703"));

        } else if (report.severity == "Benign") {
            severityLabel->setText("  BENIGN CONDITION DETECTED");
            statusValue = "Benign";
            boxPen.setColor(QColor("#2A9D8F"));

        } else {
            severityLabel->setText("  NO TUMOUR DETECTED");
            statusValue = "None";
        }

        severityLabel->setProperty("status", statusValue);
        confidenceBar->setProperty("status", statusValue);

        severityLabel->style()->unpolish(severityLabel);
        severityLabel->style()->polish(severityLabel);
    
        confidenceBar->style()->unpolish(confidenceBar);
        confidenceBar->style()->polish(confidenceBar);

        boxPen.setWidth(3);
        boxPen.setJoinStyle(Qt::MiterJoin);
        painter.setPen(boxPen);

        QRect boundbox(report.boundingBox);
        painter.drawRoundedRect(boundbox, 8, 8);

        scanView = "";

        if (Frontview->isChecked()){
            scanView = "Front";
        }
        else if(Lateralview->isChecked()){
            scanView = "Lateral";
        }

        patientId = IdInput->text().trimmed();
        
        QPen linePen(Qt::white);
        linePen.setWidth(1);
        painter.setPen(linePen);
        painter.setFont(QFont("Arial", 14, QFont::Bold));
        painter.drawText(10, inputPixmap.height() - 60, "Patient ID: " + patientId);
        painter.drawText(10, inputPixmap.height() - 40, "Scan View: " + scanView);

        painter.end();
    }
    tumourNameLabel->setText(report.className);
    
    int percentage = report.confidence * 100;
    confidenceBar->setValue(percentage);

    QString percentageString = QString("%1%").arg(percentage);
    confidenceValueLabel->setText(percentageString);

    outputPixmap = workingPixmap.scaled(zoneSize.width(), zoneSize.height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    detectionContainer->setPixmap(outputPixmap);
    mainViewRouter->setCurrentIndex(1);
}

void MainWindow::restartButton_clicked()
{
    filePath = "";
    IdInput->clear();
    Frontview->setChecked(true);
    statusLabel->setText("Status: <span style = 'color: #FFB703; font-weight: bold; font-size: 14px;'> WAITING </span><img src=':/hourglass.png' height='12' width='12'>");
    previousId->setEnabled(true);
    clearButton->setEnabled(false);
    pathLabel->clear();

    analyseButton_state();

    imageStageRouter->setCurrentIndex(0);
    mainViewRouter->setCurrentIndex(0);
}

void MainWindow::advDetailButton_clicked()
{
    if (clinicalNotesTitle->isVisible()) {
        clinicalNotesTitle->setVisible(false);
        clinicalNotes->setVisible(false);
        advDetailTitle->setText("Advanced Options +");
    }
    else {
        clinicalNotesTitle->setVisible(true);
        clinicalNotes->setVisible(true);
        advDetailTitle->setText("Advanced Options -");
    }
}

void MainWindow::analyseButton_state()
{
    QString maskedInput = IdInput->text().trimmed().remove("-");
    bool hasPatientID = maskedInput.length() > 9;
    bool hasFileUploaded = !filePath.isEmpty();
    bool isViewSelected = (Frontview->isChecked() || Lateralview->isChecked());

    bool allConditionsMet = hasPatientID && hasFileUploaded && isViewSelected;
    analyseButton->setEnabled(allConditionsMet);
}

void MainWindow::lastPatient_ID()
{
    IdInput->setText(patientId);
}

void MainWindow::editDiagnosis_clicked()
{
    editDiagnosisDialog.setId(patientId);
    editDiagnosisDialog.setDiagnosis(tumourNameLabel->text().trimmed());
    editDiagnosisDialog.setSeverity(severityLabel->text().trimmed());
    editDiagnosisDialog.setView(scanView);
    editDiagnosisDialog.exec();

    IdInput->setText(editDiagnosisDialog.getNewId());
    scanView = editDiagnosisDialog.getNewView();
    if (scanView == "Front") {
        Frontview->setChecked(true);
    }
    else {
        Lateralview->setChecked(true);
    }
    analyseButton_clicked();

    severityLabel->setText(editDiagnosisDialog.getNewSeverity());
    tumourNameLabel->setText(editDiagnosisDialog.getNewDiagnosis());
}
void MainWindow::generateReport_pdf()
{
    QString appDir = QCoreApplication::applicationDirPath();
    QString appDataPath = appDir + "/appData";

    QDir dir(appDataPath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QString inputPath = appDataPath + "/input_image.png";
    inputPixmap.save(inputPath, "PNG");

    QString outputPath = appDataPath + "/output_image.png";
    outputPixmap.save(outputPath, "PNG");

    reportInfoDialog.setClinicalNotes(clinicalNotes->toPlainText().trimmed());
    if (reportInfoDialog.exec() == QDialog::Accepted) {
        clinicianName = reportInfoDialog.getClinicianName();
        clinicalNotes->setText(reportInfoDialog.getClinicalNotes());
    }
    else {
        return;
    }

    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    
    QPixmap scaledPixmap = outputPixmap.scaled(450, 450, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    scaledPixmap.save(&buffer, "PNG");
    QString base64Image = QString(byteArray.toBase64());

    QFile htmlFile(":/report_template.html");
    if (!htmlFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Error"), tr("Could not open template file."));
        return;
    }

    QTextStream in(&htmlFile);
    QString htmlContent = in.readAll();
    htmlFile.close();

    scanView = "";
    if (Frontview->isChecked()) {
        scanView = "Front";
    } else if (Lateralview->isChecked()) {
        scanView = "Lateral";
    }

    htmlContent.replace("IMAGE_SRC_PLACEHOLDER", "data:image/png;base64," + base64Image);
    htmlContent.replace("PLACEHOLDER_ID", IdInput->text().trimmed());
    htmlContent.replace("PLACEHOLDER_VIEW", scanView);
    htmlContent.replace("PLACEHOLDER_CLINICALNOTES", clinicalNotes->toPlainText().trimmed());
    htmlContent.replace("PLACEHOLDER_DIAGNOSIS", tumourNameLabel->text().trimmed());
    htmlContent.replace("PLACEHOLDER_CONFIDENCE", confidenceValueLabel->text().trimmed());
    htmlContent.replace("PLACEHOLDER_SEVERITY", severityLabel->text());
    htmlContent.replace("PLACEHOLDER_CLINICAN", clinicianName);


    QTextDocument document;
    document.setHtml(htmlContent);

    QPrinter printer(QPrinter::ScreenResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageSize(QPageSize(QPageSize::A4));

    QString outputname = appDataPath + QString("/report_%1.pdf").arg(IdInput->text().trimmed());
    printer.setOutputFileName(outputname);

    document.print(&printer);

    QMessageBox::information(this, 
        tr("Report Generated"), 
        tr("The PDF report has been successfully generated and saved to your <b>appData</b> folder.<br><br>"
           "It will now open automatically in your system's default viewer.")
    );
    QDesktopServices::openUrl(QUrl::fromLocalFile(outputname));
}