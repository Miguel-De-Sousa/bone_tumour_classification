#include "editdiagnosisdialog.h"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QFrame>

EditDiagnosisDialog::EditDiagnosisDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle(tr("Edit Diagnosis"));
    setWindowModality(Qt::WindowModal);
    resize(460, 320);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(24, 24, 24, 24);

    QLabel *titleLabel = new QLabel(tr("<img src=':/edit.png' height='20' width='20'> Edit Report Information"), this);
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

    QLabel *IdLabel = new QLabel(tr("Patient ID"), this);
    IdLabel->setObjectName("FieldLabel");

    IdInput = new QLineEdit();
    IdInput->setObjectName("InputField");
    IdInput->setPlaceholderText("000-000-0000");
    IdInput->setInputMask("000-000-0000;-");
    formLayout->addRow(IdLabel, IdInput);

    QLabel *viewLabel = new QLabel(tr("Scan View"), this);
    viewLabel->setObjectName("FieldLabel");

    QHBoxLayout *viewLayout = new QHBoxLayout();
    Frontview = new QRadioButton(tr("Front"));
    Frontview->setObjectName("InputField");
    Frontview->setCursor(Qt::PointingHandCursor);
    Frontview->setChecked(true);
    
    Lateralview = new QRadioButton(tr("Lateral"));
    Lateralview->setObjectName("InputField");
    Lateralview->setCursor(Qt::PointingHandCursor);

    viewLayout->addWidget(Frontview);
    viewLayout->addWidget(Lateralview);
    viewLayout->addStretch(); 
    formLayout->addRow(viewLabel, viewLayout);

    QLabel *tumourNameTitle = new QLabel(tr("Tumour Category"), this);
    tumourNameTitle->setObjectName("FieldLabel");
    tumourNameInput = new QLineEdit();
    tumourNameInput->setObjectName("InputField");
    formLayout->addRow(tumourNameTitle, tumourNameInput);

    QLabel *severityTitle = new QLabel(tr("Case Severity"), this);
    severityTitle->setObjectName("FieldLabel");
    severityInput = new QLineEdit();
    severityInput->setObjectName("InputField");
    formLayout->addRow(severityTitle, severityInput);
    
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
        okButton->setText(tr("Save"));
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

QString EditDiagnosisDialog::getNewId() const {
    return IdInput->text().trimmed();
}

QString EditDiagnosisDialog::getNewView() const {
    if (Frontview->isChecked()) {
        return QStringLiteral("Front");
    } else if (Lateralview->isChecked()) {
        return QStringLiteral("Lateral");
    }
    return QString();
}

QString EditDiagnosisDialog::getNewDiagnosis() const {
    return tumourNameInput->text().trimmed();
}

QString EditDiagnosisDialog::getNewSeverity() const {
    return severityInput->text().trimmed();
}

void EditDiagnosisDialog::setId(const QString &id) {
    IdInput->setText(id);
}

void EditDiagnosisDialog::setDiagnosis(const QString &className) {
    tumourNameInput->setText(className);
}

void EditDiagnosisDialog::setSeverity(const QString &severity) {
    severityInput->setText(severity);
}

void EditDiagnosisDialog::setView(const QString &scanView) {
    if (scanView == "Front") {
        Frontview->setChecked(true);
    }
    else {
        Lateralview->setChecked(true);
    }
}