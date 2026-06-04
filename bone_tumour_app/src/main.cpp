#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

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

    mainWindow.show();

    return app.exec();
}