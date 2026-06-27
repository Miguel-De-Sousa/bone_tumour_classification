#include "reportinfodialog.h"
#include <QLineEdit>
#include <QTextEdit>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QPushButton>
#include <QFrame>

ReportInfoDialog::ReportInfoDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle(tr("Report Information"));
    setWindowModality(Qt::WindowModal);
    resize(460, 320);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(24, 24, 24, 24);

    QLabel *titleLabel = new QLabel(tr("<img src=':/report.png' height='20' width='20'> Generate PDF Report"), this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setObjectName("TitleLabel");
    mainLayout->addWidget(titleLabel);

    QFrame *separatorTitle = new QFrame();
    separatorTitle->setFrameShape(QFrame::HLine);
    separatorTitle->setFrameShadow(QFrame::Sunken);
    separatorTitle->setObjectName("SidebarDivider");
    mainLayout->addWidget(separatorTitle);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->setVerticalSpacing(14);
    formLayout->setHorizontalSpacing(16);

    clinicianNameEdit = new QLineEdit(this);
    clinicianNameEdit->setObjectName("InputField");
    clinicianNameEdit->setPlaceholderText(tr("Dr. John Smith"));

    clinicalNotesEdit = new QTextEdit(this);
    clinicalNotesEdit->setObjectName("ClinicalNotes");
    clinicalNotesEdit->setPlaceholderText(tr("Enter any diagnostic notes here..."));

    QLabel *clinicianLabel = new QLabel(tr("Clinician Name"), this);
    clinicianLabel->setObjectName("FieldLabel");
    formLayout->addRow(clinicianLabel, clinicianNameEdit);

    QLabel *notesLabel = new QLabel(tr("Clinical Notes"), this);
    notesLabel->setObjectName("FieldLabel");
    formLayout->addRow(notesLabel, clinicalNotesEdit);

    mainLayout->addLayout(formLayout);

    QFrame *separatorAction = new QFrame();
    separatorAction->setFrameShape(QFrame::HLine);
    separatorAction->setFrameShadow(QFrame::Sunken);
    separatorAction->setObjectName("SidebarDivider");
    mainLayout->addWidget(separatorAction);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);

    QPushButton *okButton = buttonBox->button(QDialogButtonBox::Ok);
    if (okButton) {
        okButton->setObjectName("AnalyseButton"); 
        okButton->setText(tr("Generate"));
    }
    
    QPushButton *cancelButton = buttonBox->button(QDialogButtonBox::Cancel);
    if (cancelButton) {
        cancelButton->setObjectName("ActionButton"); 
    }

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    mainLayout->addSpacing(8);
    mainLayout->addWidget(buttonBox);
}

QString ReportInfoDialog::getClinicianName() const {
    return clinicianNameEdit->text().trimmed();
}

QString ReportInfoDialog::getClinicalNotes() const {
    return clinicalNotesEdit->toPlainText().trimmed();
}