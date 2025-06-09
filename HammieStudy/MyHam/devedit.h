#ifndef DEVEDIT_H
#define DEVEDIT_H

#include <QDialog>

class DevEdit : public QDialog
{
    Q_OBJECT

public:
    explicit DevEdit(QWidget *parent = nullptr);

signals:
    void addSeconds(int seconds);

private slots: // 🔹 ЦЕ ВАЖЛИВО!
    void onAdd1Hour();
    void onAdd6Minutes();
};

#endif // DEVEDIT_H
