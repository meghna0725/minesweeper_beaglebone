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
    flagbutton->setGeometry(300,30,120,60);
    flagbutton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(flagbutton, SIGNAL(clicked()), this, SLOT(Flag_change()));

    ss = new QPushButton(this);
    ss->setText("Small");
    ss->setGeometry(300,100,70,50);
    connect(ss, SIGNAL(clicked()), this, SLOT(ssc()));
    ms = new QPushButton(this);
    ms->setText("Medium");
    ms->setGeometry(300,150,70,50);
    connect(ms, SIGNAL(clicked()), this, SLOT(msc()));
    ls = new QPushButton(this);
    ls->setText("Large");
    ls->setGeometry(300,200,70,50);
    connect(ls, SIGNAL(clicked()), this, SLOT(lsc()));
    em = new QPushButton(this);
    em->setText("Easy");
    em->setGeometry(380,100,70,50);
    connect(em, SIGNAL(clicked()), this, SLOT(emc()));
    mm = new QPushButton(this);
    mm->setText("Medium");
    mm->setGeometry(380,150,70,50);
    connect(mm, SIGNAL(clicked()), this, SLOT(mmc()));
    hm = new QPushButton(this);
    hm->setText("Hard");
    hm->setGeometry(380,200,70,50);
    connect(hm, SIGNAL(clicked()), this, SLOT(hmc()));
    /*
    QMap<int ,Mode> modeMap;
    QMap<int, MapSize> sizeMap;
    modeMap.insert(0,Easy);
    modeMap.insert(25,Medium);
    modeMap.insert(50,Hard);
    sizeMap.insert(0,Small);
    sizeMap.insert(0,Medium);
    sizeMap.insert(0,Large);
    slider->setMinimum(0);
    slider->setMaximum(50);
    connect(slider,&QSlider::valueChanged, this , &MainWindow::updateOptionalLabel());
    */

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

    for (int row = 1; row <= ROWS; row++) {
        for (int col = 1; col <= COLS; col++) {
            //buttons[row][col]->hide();
            //QPushButton *button = new QPushButton(this);
            buttons[row][col]->setGeometry(MARGIN+(col-1)*BLOCK_SIZE,MARGIN+(row-1)*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
            buttons[row][col]->setText(" ");
            buttons[row][col]->setFixedSize(BLOCK_SIZE, BLOCK_SIZE);
            buttons[row][col]->show();
             //buttons[row][col]->setGeometry(MARGIN+(col-1)*BLOCK_SIZE,MARGIN+(row-1)*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
        }
    }

    for (int row = ROWS+1; row <= BOUND; row++)
        for (int col = 1; col <= BOUND; col++)
            buttons[row][col]->hide();
    
    for (int col = COLS+1; col <= BOUND; col++)
        for (int row = 1; row <= BOUND; row++)
            buttons[row][col]->hide();

    for(int i = 0; i < ROWS+2; i++)
    {
        for(int j = 0; j < COLS+2; j++)
        {
            flag[i][j] = 0;
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
        if (flag[row][col]==1)
            return;
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
        if (searched_map[row][col]==0)
            buttons[row][col]->setText(tr("m"));
        if (flag[row][col] == 0)
            flag[row][col] = 1; // quick mine function
        else
        {
            flag[row][col] = 0;
            buttons[row][col]->setText(" ");
        }
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
/*
void MainWindow::ChooseOptions()
{

}
void MainWindow::updateOptionalLabel(int value){
    Mode option2 = modeMap.lowerBound(value).value();
    MapSize option1 = sizeMap.lowerBound(value).value();
    QString optionText;
    switch(option1){
        case Small:
        {
            ROWS = 5;
            COLS = 5;
            BLOCK_SIZE = 54;
            switch(option2){
                case Easy:
                {
                    NUM_MINES = 3;
                    break;
                }
                case Medium:
                {
                    NUM_MINES = 5;
                    break;
                }
                case Hard:
                {
                    NUM_MINES = 7;
                    break;
                }
            }
            break;
        }
        case Medium:
        {
            ROWS = 7;
            COLS = 7;
            BLOCK_SIZE = 38;
            switch(option2){
                case Easy:
                {
                    NUM_MINES = 5;
                    break;
                }
                case Medium:
                {
                    NUM_MINES = 8;
                    break;
                }
                case Hard:
                {
                    NUM_MINES = 12;
                    break;
                }
            }
            break;
        }
        case Large:
        {
            ROWS = 10;
            COLS = 10;
            BLOCK_SIZE = 27;
            switch(option2){
                case Easy:
                {
                    NUM_MINES = 10;
                    break;
                }
                case Medium:
                {
                    NUM_MINES = 18;
                    break;
                }
                case Hard:
                {
                    NUM_MINES = 25;
                    break;
                }
            }
            break;
        }
    }
}
*/
void MainWindow::ssc()
{
    size = 1;
    set();
}
void MainWindow::msc()
{
    size = 2;
    set();
}
void MainWindow::lsc()
{
    size = 3;
    set();
}
void MainWindow::emc()
{
    mode = 1;
    set();
}
void MainWindow::mmc()
{
    mode = 2;
    set();
}
void MainWindow::hmc()
{
    mode = 3;
    set();
}
void MainWindow::set()
{
    switch(size){
        case 1:
        {
            ROWS = 5;
            COLS = 5;
            BLOCK_SIZE = 54;
            switch(mode){
                case 1:
                {
                    NUM_MINES = 3;
                    break;
                }
                case 2:
                {
                    NUM_MINES = 5;
                    break;
                }
                case 3:
                {
                    NUM_MINES = 7;
                    break;
                }
            }
            break;
        }
        case 2:
        {
            ROWS = 7;
            COLS = 7;
            BLOCK_SIZE = 38;
            switch(mode){
                case 1:
                {
                    NUM_MINES = 5;
                    break;
                }
                case 2:
                {
                    NUM_MINES = 8;
                    break;
                }
                case 3:
                {
                    NUM_MINES = 12;
                    break;
                }
            }
            break;
        }
        case 3:
        {
            ROWS = 10;
            COLS = 10;
            BLOCK_SIZE = 27;
            switch(mode){
                case 1:
                {
                    NUM_MINES = 10;
                    break;
                }
                case 2:
                {
                    NUM_MINES = 18;
                    break;
                }
                case 3:
                {
                    NUM_MINES = 25;
                    break;
                }
            }
            break;
        }
    }
    Init();
}