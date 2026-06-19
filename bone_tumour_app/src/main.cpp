#include "analysisdashboard.h"
#include <QApplication>
#include <QFile>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv); 

    QFile file(":/stylesheet.qss");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream ts(&file);
        app.setStyleSheet(ts.readAll());
        file.close();
    }
    MainWindow window;
    window.show();                   
    return app.exec();            
}