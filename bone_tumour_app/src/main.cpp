#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QComboBox>
#include <QObject>
#include <QPushButton>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget mainWindow;
    mainWindow.setWindowTitle("Bone Tumour Classification App");
    mainWindow.resize(800, 600);

    QVBoxLayout *layout = new QVBoxLayout(&mainWindow);

    QLabel *titleLabel = new QLabel("Bone X-Ray Analysis Suite", &mainWindow);
    titleLabel->setAlignment(Qt::AlignCenter);

    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: white;");
    layout->addWidget(titleLabel);

    QWidget *firstPageWidget = new QWidget;
    QWidget *secondPageWidget = new QWidget;

    QStackedWidget *stackedWidget = new QStackedWidget;
    stackedWidget->addWidget(firstPageWidget);
    stackedWidget->addWidget(secondPageWidget);
    layout->addWidget(stackedWidget);

    QComboBox *pageComboBox = new QComboBox;
    pageComboBox->addItem(QObject::tr("Page 1"));
    pageComboBox->addItem(QObject::tr("Page 2"));
    pageComboBox->addItem(QObject::tr("Page 3"));
    QObject::connect(pageComboBox, &QComboBox::activated,
            stackedWidget, &QStackedWidget::setCurrentIndex);
    
    QPushButton *uploadButton = new QPushButton("Upload Image", firstPageWidget);
    uploadButton->setIcon(QIcon("/Users/miguel/GitHub/bone_tumour_classification/bone_tumour_app/assets/downloads.png"));
    uploadButton->setIconSize(QSize(16,16));

    mainWindow.show();

    return app.exec();
}