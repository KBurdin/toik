#include <QtWidgets>
#include "sstsdialog.h"

SSTSDialog::SSTSDialog()
{
    testButton2 = new QPushButton(tr("Signals"));
    testLayout2 = new QHBoxLayout;
    testLayout2->addWidget(testButton2);
    this->setLayout(testLayout2);

}
