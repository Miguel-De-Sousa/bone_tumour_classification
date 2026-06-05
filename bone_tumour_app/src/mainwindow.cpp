#include "mainwindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    resize(800,600);

    QLabel *titleLabel = new QLabel("Bone X-Ray Analysis Suite", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: white;");

    uploadButton = new QPushButton("Upload X-Ray Image", this);
    uploadButton->setIcon(QIcon("/Users/miguel/GitHub/bone_tumour_classification/bone_tumour_app/assets/downloads.png"));
    uploadButton->setIconSize(QSize(16,16));

    comboBox = new QComboBox(this);
    stackedWidget = new QStackedWidget(this);
    imageDisplayLabel = new QLabel(this);
    imageDisplayLabel->setAlignment(Qt::AlignCenter);

    comboBox->addItem("Page 1: Upload");
    comboBox->addItem("Page 2: View Image");

    QWidget *page1 = new QWidget();
    QVBoxLayout *page1Layout = new QVBoxLayout(page1);
    page1Layout->addWidget(uploadButton); 

    QWidget *page2 = new QWidget();
    page2->setStyleSheet("background-color: black");
    QVBoxLayout *page2Layout = new QVBoxLayout(page2);
    page2Layout->addWidget(imageDisplayLabel); 

    stackedWidget->addWidget(page1);
    stackedWidget->addWidget(page2);

    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(comboBox);
    mainLayout->addWidget(stackedWidget);
    
    setCentralWidget(centralWidget);

    connect(comboBox, &QComboBox::currentIndexChanged,
            stackedWidget, &QStackedWidget::setCurrentIndex);
    
    connect(uploadButton, &QPushButton::clicked, this, &MainWindow::uploadButton_clicked);


}

MainWindow::~MainWindow(){}

void MainWindow::uploadButton_clicked()
{
    QString filePath = QFileDialog::getOpenFileName(
        this, tr("Open X-Ray Image"), "", tr("Images (*.png *.jpg *.jpeg)")
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

    // Switch to page 2 (index 1)
    comboBox->setCurrentIndex(1);
}