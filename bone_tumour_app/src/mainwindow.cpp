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
    resize(800,600);

    titleLabel = new QLabel("X-Ray Analysis Suite", this);
    titleLabel->setStyleSheet("font-size: 24px; font-weight: bold; color: white;");
    titleLabel->setAlignment(Qt::AlignCenter);

    uploadButton = new QPushButton("Upload X-Ray", this);
    uploadButton->setIcon(QIcon("/Users/miguel/GitHub/bone_tumour_classification/bone_tumour_app/assets/downloads.png"));
    uploadButton->setIconSize(QSize(16,16));
    uploadButton->setFixedSize(200, 45);
    uploadButton->setStyleSheet(R"(
        QPushButton{
            background-color: #0076FF;
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
    decisionLabel->setVisible(false);
    
    acceptButton = new QPushButton(this);
    acceptButton->setIcon(QIcon("/Users/miguel/GitHub/bone_tumour_classification/bone_tumour_app/assets/check.png"));
    acceptButton->setIconSize(QSize(16,16));
    acceptButton->setFixedSize(200, 45);
    acceptButton->setVisible(false);
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
    denyButton->setVisible(false);
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
    page1Layout->addWidget(uploadButton); 
    page1Layout->setAlignment(Qt::AlignCenter);

    QWidget *page2 = new QWidget();
    page2->setStyleSheet("background-color: black");
    QVBoxLayout *page2Layout = new QVBoxLayout(page2);
    page2Layout->addWidget(imageDisplayLabel); 
    page2Layout->setAlignment(Qt::AlignCenter);

    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(page1);
    stackedWidget->addWidget(page2);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    QHBoxLayout *navBarLayout =  new QHBoxLayout();
    QHBoxLayout *footerBarLayout = new QHBoxLayout();

    navBarLayout->addWidget(titleLabel);
    navBarLayout->addWidget(comboBox);

    footerBarLayout->addWidget(decisionLabel);
    footerBarLayout->addWidget(acceptButton);
    footerBarLayout->addWidget(denyButton);

    mainLayout->addLayout(navBarLayout);
    mainLayout->addWidget(stackedWidget);
    mainLayout->addLayout(footerBarLayout);
    
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

    acceptButton->setVisible(true);
    denyButton->setVisible(true);
    decisionLabel->setVisible(true);
    titleLabel->setVisible(false);
    comboBox->setCurrentIndex(1);
}
void MainWindow::denyButton_clicked()
{
    comboBox->setCurrentIndex(0);
    acceptButton->setVisible(false);
    denyButton->setVisible(false);
    decisionLabel->setVisible(false);
    titleLabel->setVisible(true);
}