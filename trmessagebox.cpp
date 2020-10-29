#include <QObject>

#include "trmessagebox.h"

TrMessageBox::TrMessageBox()
{
    setButtonText(TrMessageBox::Yes, QObject::tr("Yes"));
    setButtonText(TrMessageBox::No, QObject::tr("No"));
    setButtonText(TrMessageBox::Cancel, QObject::tr("Cancel"));
    setButtonText(TrMessageBox::Ok, QObject::tr("OK"));
}
