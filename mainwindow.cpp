#include <QPainter>
#include <time.h>
#include <QTimer>
#include <QString>
#include <QMessageBox>
#include <QApplication>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    QGridLayout *gridLayout = new QGridLayout(this);
    flagbutton = new QPushButton(this);
    flagbutton->setText("flag: off");
    flagbutton->setGeometry(MARGIN+(COLS+1)*BLOCK_SIZE,MARGIN+1*BLOCK_SIZE,BLOCK_SIZE*5,BLOCK_SIZE*2);
    flagbutton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(flagbutton, SIGNAL(clicked()), this, SLOT(Flag_change()));

    for (int row = 1; row <= ROWS; row++) {
        for (int col = 1; col <= COLS; col++) {
            QPushButton *button = new QPushButton(this);
            button->setGeometry(10*MARGIN+col*BLOCK_SIZE,MARGIN+row*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
            button->setFixedSize(BLOCK_SIZE, BLOCK_SIZE);
            button->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            button->setProperty("row", row);
            button->setProperty("col", col);
            button->setText(" ");
            buttons[row][col] = button;/*
            buttons[row][col]->setGeometry(10*MARGIN+(col-1)*BLOCK_SIZE,MARGIN+(row-1)*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
            //buttons[row][col]->setGeometry(10*MARGIN+col*BLOCK_SIZE,MARGIN+row*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
            buttons[row][col]->setFixedSize(BLOCK_SIZE, BLOCK_SIZE);
            buttons[row][col]->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
            buttons[row][col]->setProperty("row", row);
            buttons[row][col]->setProperty("col", col);
            buttons[row][col]->setText(" ");*/
            buttons[row][col]->setGeometry(MARGIN+(col-1)*BLOCK_SIZE,MARGIN+(row-1)*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
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

void MainWindow::Mine()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    int row = button->property("row").toInt();
    int col = button->property("col").toInt();

    if (!flag_state) // mine
    {
        if (map[row][col] == 9)
        {
            Lose();
            return;
        }
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
            if (searched_map[row][col]==1)
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

void MainWindow::Win()
{
    QMessageBox messageBox;
    messageBox.setWindowTitle(tr("You win!"));
    messageBox.setText(tr("You win! Do you want to restart?"));
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
    messageBox.setText(tr("You lose! Do you want to restart?"));
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    if (messageBox.exec() == QMessageBox::Yes)
    {
        Init();
    }
    if (messageBox.exec() == QMessageBox::No)
        qApp->quit();  
}