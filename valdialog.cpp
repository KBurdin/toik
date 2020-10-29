#include <QMessageBox>
#include <QLayout>
#include <QDialog>

#include "valdialog.h"
#include "trmessagebox.h"

ValDialog::ValDialog(int saveConf, QWidget *parent): QDialog(parent),
    mainVBoxLayout(new QVBoxLayout),
    mainHBoxLayout(new QHBoxLayout),
    mainGroupBox(new QGroupBox),
    okPushButton(new QPushButton(tr("OK"))),
    cancelPushButton(new QPushButton(tr("Cancel"))),
    saveConfirmation(saveConf)

{

    mainVBoxLayout->addWidget(mainGroupBox);
    mainHBoxLayout->addStretch();
    mainHBoxLayout->addWidget(okPushButton);
    mainHBoxLayout->addWidget(cancelPushButton);
    mainHBoxLayout->addStretch();
    mainVBoxLayout->addLayout(mainHBoxLayout);
    setLayout(mainVBoxLayout);

    connect(okPushButton, &QPushButton::clicked, this, &ValDialog::accept);
    connect(cancelPushButton, &QPushButton::clicked, this, &ValDialog::reject);
}

void ValDialog::setMainLayout(QLayout *layout)
{
    if (layout != nullptr) {
        mainGroupBox->setLayout(layout);
    }
}

void ValDialog::setValidatorFields(QVector<QLineEdit *> lineEdits)
{
    edits = lineEdits;
}

bool ValDialog::isEdited()
{
    return editedFlag;
}

void ValDialog::setEdited()
{
    editedFlag = true;
}

void ValDialog::setGroupBoxTitle(const QString &title)
{
    mainGroupBox->setTitle(title);
}

void ValDialog::hideCancelButton()
{
    cancelPushButton->setHidden(true);
}

void ValDialog::showCancelButton()
{
    cancelPushButton->setVisible(true);
}

void ValDialog::accept()
{
    bool valid = true;
    QString nonValidStr;

    foreach (QLineEdit* e, edits) {
        if ( (e != nullptr) && (!e->hasAcceptableInput()) ) {
            valid = false;
            nonValidStr += "\n\"" + e->text() + "\"";
        }
    }

    if (isEdited()) {
        if (valid) {

            if (saveConfirmation == ConfirmSave) {
                if (TrMessageBox::warning(this, tr("TOiK"), tr("Do you want to save this changes?"), QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
                    QDialog::accept();

                } else {
                    QDialog::reject();
                }

            } else if (saveConfirmation == NoConfirmSave) {
                QDialog::accept();

            } else {
                QDialog::accept();
            }

        } else {
            TrMessageBox::warning(this, tr("Attention"), tr("Invalid format, check please: ") + nonValidStr );
        }

    } else {
        QDialog::reject();
    }
}

int ValDialog::exec()
{
    editedFlag = false;
    return QDialog::exec();
}
