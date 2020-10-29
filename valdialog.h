#ifndef VALDIALOG_H
#define VALDIALOG_H

#include <QDialog>
#include <QVector>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QDebug>

class ValDialog: public QDialog
{
Q_OBJECT

public:

    enum {
        ConfirmSave = 1,
        NoConfirmSave = 2
    };

    ValDialog(int saveConf = ConfirmSave, QWidget *parent = nullptr);

    void setMainLayout(QLayout *layout);
    void setValidatorFields(QVector <QLineEdit*> lineEdits);
    bool isEdited();
    void setEdited();
    void setGroupBoxTitle(const QString& title);
    void hideCancelButton();
    void showCancelButton();


protected:
    QVector <QLineEdit*> edits;
    QVBoxLayout *mainVBoxLayout;
    QHBoxLayout *mainHBoxLayout;
    QGroupBox *mainGroupBox;
    QPushButton *okPushButton;
    QPushButton *cancelPushButton;

private:
    int saveConfirmation;

    bool editedFlag = false;

public slots:    
    void accept();
    int exec();

};

#endif // VALDIALOG_H
