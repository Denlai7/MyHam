#include "mainwindow.h"
#include <QPixmap>
#include <QEvent>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), running(false), totalElapsedSeconds(0), clickCount(0)
{
    elapsedTime = QTime(0, 0);

    timer = new QTimer(this);
    timer->setInterval(600);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);

    imageResetTimer = new QTimer(this);
    imageResetTimer->setSingleShot(true);

    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout();

    imageLabel = new QLabel(this);
    QPixmap pixmap(":/res/Hamster.png");
    imageLabel->setPixmap(pixmap);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setScaledContents(true);
    imageLabel->setMaximumWidth(400);
    imageLabel->setFixedHeight(300);

    hamImageLabel = new QLabel(this);
    QPixmap hamPixmap(":/res/Ham.png");
    hamImageLabel->setPixmap(hamPixmap);
    hamImageLabel->setScaledContents(true);
    hamImageLabel->setFixedSize(100, 100);
    hamImageLabel->hide();
    hamImageLabel->raise();

    hamAnimation = new QPropertyAnimation(hamImageLabel, "geometry", this);

    QHBoxLayout *topRightLayout = new QHBoxLayout();
    topRightLayout->setAlignment(Qt::AlignRight);

    bigHamLabel = new QLabel(this);
    QPixmap bigHamPixmap(":/res/BigHam.png");
    bigHamLabel->setPixmap(bigHamPixmap);
    bigHamLabel->setScaledContents(true);
    bigHamLabel->setFixedSize(100, 100);
    bigHamLabel->installEventFilter(this); // 🔹 Обробка натискань

    hamCountLabel = new QLabel("Ham: 0.0", this);
    hamCountLabel->setStyleSheet("font-size: 20px; color: #772;");
    hamCountLabel->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);

    topRightLayout->addWidget(bigHamLabel);
    topRightLayout->addWidget(hamCountLabel);

    timeLabel = new QLabel("00:00:00", this);
    timeLabel->setAlignment(Qt::AlignCenter);
    QFont font = timeLabel->font();
    font.setPointSize(32);
    timeLabel->setFont(font);

    startStopButton = new QPushButton("Пуск", this);
    resetButton = new QPushButton("Скинути", this);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(startStopButton);
    buttonLayout->addWidget(resetButton);

    historyList = new QListWidget(this);

    // DevEdit кнопка
    devButton = new QPushButton("DevEdit", this);
    devButton->hide();
    connect(devButton, &QPushButton::clicked, [=]() {
        devEdit->show();
    });

    mainLayout->addLayout(topRightLayout);
    mainLayout->addWidget(imageLabel);
    mainLayout->addWidget(timeLabel);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(devButton);
    mainLayout->addWidget(historyList);

    central->setLayout(mainLayout);
    setCentralWidget(central);
    setWindowTitle("HammyStudy");

    resize(420, 500);

    connect(startStopButton, &QPushButton::clicked, this, &MainWindow::onStartStopClicked);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::onResetClicked);

    connect(imageResetTimer, &QTimer::timeout, [=]() {
        QPixmap originalPixmap(":/res/Hamster.png");
        imageLabel->setPixmap(originalPixmap);
        hamImageLabel->hide();
    });

    devEdit = new DevEdit(this);
    connect(devEdit, &DevEdit::addSeconds, this, &MainWindow::onDevSecondsAdded);
}

MainWindow::~MainWindow() {}

void MainWindow::updateTime()
{
    elapsedTime = elapsedTime.addSecs(1);
    timeLabel->setText(elapsedTime.toString("hh:mm:ss"));
}

void MainWindow::onStartStopClicked()
{
    if (!running) {
        timer->start();
        running = true;
        startStopButton->setText("Зупинити");
        addHistoryEntry("Запущено: " + QTime::currentTime().toString("hh:mm:ss"));
    } else {
        timer->stop();
        running = false;
        startStopButton->setText("Відновити");
        addHistoryEntry("Зупинено: " + QTime::currentTime().toString("hh:mm:ss") +
                        " (Тривалість: " + elapsedTime.toString("hh:mm:ss") + ")");
    }
}

void MainWindow::onResetClicked()
{
    if (running) {
        timer->stop();
        running = false;
        startStopButton->setText("Пуск");
    }

    addHistoryEntry("Скинуто в: " + QTime::currentTime().toString("hh:mm:ss") +
                    " (Було: " + elapsedTime.toString("hh:mm:ss") + ")");

    int sessionSeconds = QTime(0, 0).secsTo(elapsedTime);
    totalElapsedSeconds += sessionSeconds;

    double totalHam = totalElapsedSeconds / 3600.0;
    hamCountLabel->setText(QString("Ham: %1").arg(QString::number(totalHam, 'f', 3)));

    QPixmap tempPixmap(":/res/Hamster2.png");
    imageLabel->setPixmap(tempPixmap);

    int centerX = imageLabel->x() + imageLabel->width() / 2 - 50;
    int centerY = imageLabel->y() + imageLabel->height() / 2 - 50;

    QRect startRect(10, height() - 110, 100, 100);
    QRect endRect(centerX, centerY, 40, 40);

    hamImageLabel->setGeometry(startRect);
    hamImageLabel->show();

    hamAnimation->setDuration(1000);
    hamAnimation->setStartValue(startRect);
    hamAnimation->setEndValue(endRect);
    hamAnimation->start();

    imageResetTimer->start(1000);

    elapsedTime = QTime(0, 0);
    timeLabel->setText("00:00:00");
}

void MainWindow::addHistoryEntry(const QString &text)
{
    history.append(text);
    historyList->addItem(text);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == bigHamLabel && event->type() == QEvent::MouseButtonPress) {
        clickCount++;
        if (clickCount >= 4) {
            devButton->show();
        }
        return true;
    }

    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::onDevSecondsAdded(int seconds)
{
    elapsedTime = elapsedTime.addSecs(seconds);
    timeLabel->setText(elapsedTime.toString("hh:mm:ss"));
    addHistoryEntry(QString("DevEdit: додано %1 сек.").arg(seconds));
}
