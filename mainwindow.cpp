#include <QPainter>
#include <time.h>
#include <QTimer>
#include <QString>
#include <QMessageBox>
#include <QApplication>
#include <QMap>
#include <QSlider>
#include <QPropertyAnimation>
#include <QLCDNumber>
#include <QFont>
//#include <QtGui>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    QGridLayout *gridLayout = new QGridLayout(centralWidget);
    flagbutton = new QPushButton(centralWidget);
    flagbutton->setText("flag: off");
    flagbutton->setGeometry(MARGIN+(COLS+1)*BLOCK_SIZE,MARGIN+1*BLOCK_SIZE,BLOCK_SIZE*5,BLOCK_SIZE*2);
    flagbutton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    //flagbutton->setColor(QPalette::Button, QColor(Qt::blue));
    connect(flagbutton, SIGNAL(clicked()), this, SLOT(Flag_change()));

    for (int row = 1; row <= ROWS; row++) {
        for (int col = 1; col <= COLS; col++) {
            QPushButton *button = new QPushButton(centralWidget);
            button->setGeometry(MARGIN*10+col*BLOCK_SIZE,MARGIN+row*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
            button->setFixedSize(BLOCK_SIZE, BLOCK_SIZE);
            button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            button->setProperty("row", row);
            button->setProperty("col", col);
            button->setText(" ");
            buttons[row][col] = button;
            gridLayout->addWidget(button, row, col);
            connect(buttons[row][col], SIGNAL(clicked()), this, SLOT(Mine()));
        }
    }

    setCentralWidget(centralWidget);
    setWindowTitle(tr("Minesweeper"));
    Init();
}

MainWindow::~MainWindow()
{

}

void MainWindow::updateOptionLabel(int value) {
    Mode option = modeMap.lowerBound(value).value();
    MapSize size_option = sizeMap.lowerBound(value).value();
    QString optionText;
    switch (option) {
        case Easy:
            switch(size_option) {
                case Easy:
                    break;
                case Medium:
                    break;
                case Large:
                    break;
            }
            break;
        case Medium:
            switch(size_option) {
                case Easy:
                    break;
                case Medium:
                    break;
                case Large:
                    break;
            }
            break;
        case Hard:
            switch(size_option) {
                case Easy:
                    break;
                case Medium:
                    break;
                case Large:
                    break;
            }
            break;
        default:
            break;
    }
    ui->optionLabel->setText(optionText);
}


void MainWindow::ChooseOptions()
{
    QMap<int, Mode> modeMap;
    QMap<int, MapSize> sizeMap;
    modeMap.insert(0, Easy);
    modeMap.insert(25, Medium);
    modeMap.insert(50, Hard);

    sizeMap.insert(0, Small);
    sizeMap.insert(25, Medium);
    sizeMap.insert(50, Large);

    slider->setMinimum(0);
    slider->setMaximum(50);
    connect(slider, &QSlider::valueChanged, this, &MainWindow::updateOptionLabel());
    //Init();
}

void MainWindow::Init()
{
    for(int i = 0; i < ROWS+2; i++)
    {
        for(int j = 0; j < COLS+2; j++)
        {
            map[i][j] = 0;
            searched_map[i][j] = 0;
            if(i==0||i==ROWS+1||j==0||j==COLS+1)
            {
                map[i][j] = -1; // bound
                searched_map[i][j] = 1;
            }
            else
                buttons[i][j]->setText(" ");
        }
    }

    for(int i = 0; i < NUM_MINES; i++)
    {
        int loc = rand()%(ROWS*COLS);
        int loc_x = loc/COLS;
        int loc_y = loc%ROWS;
        if (map[loc_x+1][loc_y+1]==9)
            i--;
        map[loc_y+1][loc_x+1] = 9; // the mine
    }

    for(int i = 1; i <= ROWS; i++)
    {
        for(int j = 1; j <= COLS; j++)
        {
            if (map[i][j]!=9)
            {
                if(map[i-1][j-1] == 9)
                    map[i][j]++;
                if(map[i-1][j] == 9)
                    map[i][j]++;
                if(map[i-1][j+1] == 9)
                    map[i][j]++;
                if(map[i][j-1] == 9)
                    map[i][j]++;
                if(map[i][j+1] == 9)
                    map[i][j]++;
                if(map[i+1][j-1] == 9)
                    map[i][j]++;
                if(map[i+1][j] == 9)
                    map[i][j]++;
                if(map[i+1][j+1] == 9)
                    map[i][j]++;
            }
        }
    }
}

void MainWindow::GameTimer(QWidget *parent = nullptr)
{
    QLCDNumber *lcdNumber = new QLCDNumber();
    lcdNumber->setDigitCount(5);

    QTimer *timer = new QTimer(parent);
    int elapsedTime = 0;

    startButton = new QPushButton("Start");
    connect(startButton, &QPushButton::clicked, [timer, &elapsedTime, lcdNumber, startButton]() {
        startButton->setEnabled(false);
        timer->start(1000);
        connect(timer, &QTimer::timeout, [lcdNumber, &elapsedTime]() {
            elapsedTime++;
            lcdNumber->display(elapsedTime);
        });
    });

    QVBoxLayout *layout = new QVBoxLayout(parent);
    layout->addWidget(lcdNumber);
    layout->addWidget(startButton);
}

void MainWindow::Mine()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    int row = button->property("row").toInt();
    int col = button->property("col").toInt();

    if (!flag_state) // mine
    {
        if (map[row][col] == 9)
            Lose();
        if (searched_map[row][col] == 0)
            Search(row,col);
        /* quick mine function
        else
        {

        } 
        */
    }
    else // mark the flag
    {
        buttons[row][col]->setText(tr("m"));
        // flag[row][col] = 1; // quick mine function
    }
}

void MainWindow::Search(int row, int col)
{
    if (searched_map[row][col] == 1)
        return;
    searched_map[row][col] = 1;
    if (map[row][col] == 0)
    {
        buttons[row][col]->setText(QString::number(map[row][col]));
        Search(row-1,col-1);
        Search(row-1,col);
        Search(row-1,col+1);
        Search(row,col-1);
        Search(row,col+1);
        Search(row+1,col-1);
        Search(row+1,col);
        Search(row+1,col+1);
    }
    else if (map[row][col] == -1) // bound
        return;
    else if (map[row][col] >= 1 && map[row][col] <=8) // number of mines
        buttons[row][col]->setText(QString::number(map[row][col]));
    int sum_search = 0; // mark the amount of searched areas
    for (int row = 1; row <= ROWS; row++)
    {
        for (int col = 1; col <= COLS; col++)
        {
            sum_search++;
        }
    }
    if (sum_search == ROWS*COLS-NUM_MINES)
        Win();
}

void MainWindow::Flag_change()
{
    flag_state = !flag_state;
    if (flag_state)
        flagbutton->setText("flag: on");
    else
        flagbutton->setText("flag: off");
}

void MainWindow::Time_Tracker()
{
    
}

void MainWindow::Win()
{
    QMessageBox messageBox;
    messageBox.setWindowTitle(tr("You win!"));
    messageBox.setText(tr("Do you want to restart?"));
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    if (messageBox.exec() == QMessageBox::Yes)
    {
        Init();
    }
    if (messageBox.exec() == QMessageBox::No)
        qApp->quit();  
}

void MainWindow::Lose()
{
    QMessageBox messageBox;
    messageBox.setWindowTitle(tr("You lose!"));
    messageBox.setText(tr("Do you want to restart?"));
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    if (messageBox.exec() == QMessageBox::Yes)
    {
        Init();
    }
    if (messageBox.exec() == QMessageBox::No)
        QuitMessage();
        qApp->quit();  
}

void MainWindow::QuitMessage()
{
    QMessageBox messageBox;
    messageBox.setText(tr("Thank you for playing!"));
}

