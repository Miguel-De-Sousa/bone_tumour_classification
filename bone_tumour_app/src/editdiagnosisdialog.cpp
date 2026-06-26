#include "editdiagnosisdialog.h"
#include <QFormLayout>
#include <QVBoxLayout>
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

    QLabel *titleLabel = new QLabel(tr("Edit Report Information"), this);
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

    Frontview = new QRadioButton(tr("Front"));
    Frontview->setObjectName("InputField");
    Frontview->setCursor(Qt::PointingHandCursor);
    Frontview->setChecked(true);
    
    Lateralview = new QRadioButton(tr("Lateral"));
    Lateralview->setObjectName("InputField");
    Lateralview->setCursor(Qt::PointingHandCursor);

    QLabel *tumourNameTitle = new QLabel(tr("Tumour Category"));
    tumourNameTitle->setObjectName("FieldLabel");
    tumourNameInput = new QLineEdit();
    tumourNameInput->setObjectName("InputField");

    QLabel *severityTitle = new QLabel(tr("Case Severity"));
    severityTitle->setObjectName("FieldLabel");
    severityInput = new QLineEdit();
    severityInput->setObjectName("InputField");
    

    formLayout->addRow(IdLabel, IdInput);
    formLayout->addRow(Frontview, Lateralview);
    formLayout->addRow(tumourNameTitle, tumourNameInput);
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