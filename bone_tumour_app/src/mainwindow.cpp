#include "mainwindow.h"
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

    uploadButton = new QPushButton(" Upload", this);
    uploadButton->setIcon(QIcon("/Users/miguel/GitHub/bone_tumour_classification/bone_tumour_app/assets/downloads.png"));
    uploadButton->setIconSize(QSize(16,16));
    uploadButton->setFixedSize(200, 45);
    uploadButton->setStyleSheet(R"(
        QPushButton{
            background-color: #2B76E3;
            color: #FFFFFF;
            font-family: "Segoe UI", Arial;    
            font-size: 14px;
            font-weight: bold;
            border: none;                       
            border-radius: 8px;                 
            padding: 10px 24px;
        }
        QPushButton:hover{
            background-color: #0056B3;
        }
        QPushButton:pressed {
            background-color: #003D80;          
        }
    )");

    decisionLabel = new QLabel("Analyse X-Ray Scan:", this);
    decisionLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: white;");
    
    acceptButton = new QPushButton(this);
    acceptButton->setIcon(QIcon("/Users/miguel/GitHub/bone_tumour_classification/bone_tumour_app/assets/check.png"));
    acceptButton->setIconSize(QSize(16,16));
    acceptButton->setFixedSize(200, 45);
    acceptButton->setStyleSheet(R"(
    QPushButton {
        background-color: #28A745;
        color: #FFFFFF;
        font-family: 'Segoe UI', Arial;
        font-size: 14px;
        font-weight: bold;
        border: 1px solid transparent;
        border-radius: 6px;
    }
    QPushButton:hover {
        background-color: #218838;
    }
    QPushButton:pressed {
        background-color: #1E7E34;
    }
    )");

    denyButton = new QPushButton(this);
    denyButton->setIcon(QIcon("/Users/miguel/GitHub/bone_tumour_classification/bone_tumour_app/assets/cross.png"));
    denyButton->setIconSize(QSize(16,16));
    denyButton->setFixedSize(200, 45);
    denyButton->setStyleSheet(R"(
    QPushButton {
        background-color: #DC3545;
        color: #FFFFFF;
        font-family: 'Segoe UI', Arial;
        font-size: 14px;
        font-weight: bold;
        border: 1px solid transparent;
        border-radius: 6px;
    }
    QPushButton:hover {
        background-color: #C82333;
    }
    QPushButton:pressed {
        background-color: #BD2130;
    }
    )");

    comboBox = new QComboBox(this);
    comboBox->addItem("Page 1: Upload Image");
    comboBox->addItem("Page 2: View Image");
    comboBox->setFixedSize(0, 0);

    imageDisplayLabel = new QLabel(this);

    QWidget *page1 = new QWidget();
    QVBoxLayout *page1Layout = new QVBoxLayout(page1);
    page1Layout->addWidget(titleLabel);
    page1Layout->addWidget(comboBox);
    page1Layout->addWidget(dragdropLabel);
    page1Layout->addWidget(supportLabel);
    page1Layout->addWidget(uploadButton, 0, Qt::AlignRight); 
    page1Layout->setSpacing(20);
    page1Layout->setAlignment(Qt::AlignCenter);

    QWidget *page2 = new QWidget();
    QVBoxLayout *page2Layout = new QVBoxLayout(page2);
    page2Layout->addWidget(imageDisplayLabel); 
    page2Layout->setAlignment(Qt::AlignCenter);
    page2Layout->addWidget(decisionLabel);
    page2Layout->addWidget(acceptButton);
    page2Layout->addWidget(denyButton);

    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(page1);
    stackedWidget->addWidget(page2);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    mainLayout->addWidget(stackedWidget);
    
    setCentralWidget(centralWidget);

    connect(comboBox, &QComboBox::currentIndexChanged,
            stackedWidget, &QStackedWidget::setCurrentIndex);
    connect(denyButton, &QPushButton::clicked, this, &MainWindow::denyButton_clicked);
    connect(uploadButton, &QPushButton::clicked, this, &MainWindow::uploadButton_clicked);

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

    comboBox->setCurrentIndex(1);
}
void MainWindow::denyButton_clicked()
{
    comboBox->setCurrentIndex(0);
}