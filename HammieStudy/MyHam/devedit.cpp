#include "devedit.h"
#include <QPushButton>
#include <QVBoxLayout>

DevEdit::DevEdit(QWidget *parent)
    : QDialog(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QPushButton *add1HourButton = new QPushButton("+1 година", this);
    QPushButton *add6MinButton = new QPushButton("+6 хв", this);

    layout->addWidget(add1HourButton);
    layout->addWidget(add6MinButton);

    connect(add1HourButton, &QPushButton::clicked, this, &DevEdit::onAdd1Hour);
    connect(add6MinButton, &QPushButton::clicked, this, &DevEdit::onAdd6Minutes);

    setWindowTitle("DevEdit");
}

void DevEdit::onAdd1Hour()
{
    emit addSeconds(3600);
}

void DevEdit::onAdd6Minutes()
{
    emit addSeconds(360);
}
