#ifndef EDITDIAGNOSISDIALOG_H
#define EDITDIAGNOSISDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QRadioButton>

class EditDiagnosisDialog : public QDialog {
    Q_OBJECT

public:
    explicit EditDiagnosisDialog(QWidget *parent = nullptr);
    ~EditDiagnosisDialog() override = default;

    QString getNewId() const;
    QString getNewView() const;
    QString getNewDiagnosis() const;
    QString getNewSeverity() const;

private:
    QLineEdit *IdInput;
    QRadioButton *Frontview;
    QRadioButton *Lateralview;
    QLineEdit *tumourNameInput;
    QLineEdit *severityInput;
};

#endif