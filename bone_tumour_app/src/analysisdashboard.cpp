#include "analysisdashboard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QMenuBar>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(800,600);
    this->setFixedSize(800, 600);
    this->setWindowTitle("Analysis Suite");
    this->setStyleSheet("background-color: #FFFFFF;");

    QLabel *titleLabel = new QLabel("X-Ray Analysis Suite", this);
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

    QLabel *supportLabel = new QLabel("Supported formats: PNG, JPG, JPEG", this);
    supportLabel->setStyleSheet("color: #555555; ");

    infoButton = new QPushButton(" Model Information", this);
    infoButton->setIcon(QIcon("/Users/miguel/GitHub/bone_tumour_classification/bone_tumour_app/assets/info.png"));
    infoButton->setIconSize(QSize(16,16));
    infoButton->setFixedWidth(150);
    infoButton->setStyleSheet(R"(
    QPushButton {
        background-color: #FFFFFF;
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
    
    acceptButton = new QPushButton("Proceed: Accept File ", this);
    acceptButton->setIcon(QIcon("/Users/miguel/GitHub/bone_tumour_classification/bone_tumour_app/assets/angle-right.png"));
    acceptButton->setIconSize(QSize(16,16));
    acceptButton->setFixedSize(220, 45);
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

    denyButton = new QPushButton("Return: File Selection ", this);
    denyButton->setIcon(QIcon("/Users/miguel/GitHub/bone_tumour_classification/bone_tumour_app/assets/refresh.png"));
    denyButton->setIconSize(QSize(16,16));
    denyButton->setFixedSize(220, 45);
    denyButton->setLayoutDirection(Qt::RightToLeft);
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

    comboBox = new QComboBox(this);
    comboBox->addItem("Page 1");
    comboBox->addItem("Page 2");
    comboBox->addItem("Page 3");
    comboBox->setFixedSize(0, 0);

    imageDisplayLabel = new QLabel(this);
    imageDisplayLabel->setStyleSheet("background-color: #000000; border-radius: 8px;");
    imageDisplayLabel->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    imageDisplayLabel->setAlignment(Qt::AlignCenter);

    resultsLabel = new QLabel(this);

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
    QVBoxLayout *page2Layout = new QVBoxLayout(page2);
    QHBoxLayout *page2decisionLayout = new QHBoxLayout(page2);

    page2decisionLayout->addWidget(acceptButton);
    page2decisionLayout->addWidget(denyButton);
    
    page2Layout->addWidget(imageDisplayLabel);
    page2Layout->addLayout(page2decisionLayout);
    page2Layout->setAlignment(Qt::AlignCenter);

    QWidget *page3 = new QWidget();
    QVBoxLayout *page3Layout = new QVBoxLayout(page3);
    page3Layout->addWidget(resultsLabel);


    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(page1);
    stackedWidget->addWidget(page2);
    stackedWidget->addWidget(page3);


    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    mainLayout->addWidget(stackedWidget);
    
    setCentralWidget(centralWidget);

    connect(comboBox, &QComboBox::currentIndexChanged,
            stackedWidget, &QStackedWidget::setCurrentIndex);
    connect(acceptButton, &QPushButton::clicked, this, &MainWindow::acceptButton_clicked);
    connect(denyButton, &QPushButton::clicked, this, &MainWindow::denyButton_clicked);
    connect(uploadButton, &QPushButton::clicked, this, &MainWindow::uploadButton_clicked);

    if (!classifier.loadModel("bonetumour_model.onnx")){
        QMessageBox::critical(this, tr("Error"), tr("AI Model failed to load"));
    };
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

    int w = stackedWidget->width();
    int h = stackedWidget->height();
    imageDisplayLabel->setPixmap(medicalImage.scaled(w, h, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    ModelResult result = classifier.predict(filePath);
    resultsLabel->setText(result.displayString);
    if (result.isMalignant) {
        resultsLabel->setStyleSheet("QLabel { color: red; font-weight: bold; }");
    } else {
        resultsLabel->setStyleSheet("QLabel { color: green; font-weight: bold; }");
    }

    comboBox->setCurrentIndex(1);
}
void MainWindow::acceptButton_clicked()
{
    comboBox->setCurrentIndex(2);
}
void MainWindow::denyButton_clicked()
{
    comboBox->setCurrentIndex(0);
}