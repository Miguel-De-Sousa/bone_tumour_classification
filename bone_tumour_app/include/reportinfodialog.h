#ifndef REPORTINFODIALOG_H
#define REPORTINFODIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QLineEdit>

class QLineEdit;

class ReportInfoDialog : public QDialog {
    Q_OBJECT

public:
    explicit ReportInfoDialog(QWidget *parent = nullptr);
    ~ReportInfoDialog() override = default;

    QString getClinicianName() const;
    QString getClinicalNotes() const;
    void setClinicalNotes(const QString notes);

private:
    QLineEdit *clinicianNameEdit;
    QTextEdit *clinicalNotesEdit;
};

#endif 