#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>
#include <QStringList>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QPropertyAnimation>

#include "devedit.h" // 🔹 Підключаємо DevEdit

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *obj, QEvent *event) override; // 🔹 Подія

private slots:
    void updateTime();
    void onStartStopClicked();
    void onResetClicked();
    void onDevSecondsAdded(int seconds); // 🔹 Новий слот

private:
    QTimer *timer;
    QTimer *imageResetTimer;
    QTime elapsedTime;
    bool running;
    QStringList history;
    int totalElapsedSeconds;

    QLabel *timeLabel;
    QLabel *imageLabel;
    QLabel *hamImageLabel;
    QPropertyAnimation *hamAnimation;

    QLabel *bigHamLabel;
    QLabel *hamCountLabel;

    QPushButton *startStopButton;
    QPushButton *resetButton;
    QListWidget *historyList;

    // 🔹 Нові поля
    int clickCount;
    QPushButton *devButton;
    DevEdit *devEdit;

    void addHistoryEntry(const QString &text);
};

#endif // MAINWINDOW_H
 MAINWINDOW_H
