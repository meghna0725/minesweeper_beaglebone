#include <QPainter>
#include <time.h>
#include <QTimer>
#include <QString>
#include <QMessageBox>
#include <QApplication>
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
    connect(flagbutton, SIGNAL(clicked()), this, SLOT(Flag_change()));

    for (int row = 1; row <= ROWS; row++) {
        for (int col = 1; col <= COLS; col++) {
            QPushButton *button = new QPushButton(centralWidget);
            button->setGeometry(MARGIN+col*BLOCK_SIZE,MARGIN+row*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
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
/*
void Widget::FoodGenerate()
{
    //Generate Food ramdomly
    foodPoint.setX(rand()%NUM_COL);
    foodPoint.setY(rand()%NUM_ROW);
    //In case the food generated inside snake
    if(snake.contains(foodPoint))
        FoodGenerate();
}

void Widget::PoisonGenerate()
{
    if(isPoison)
    {
      poisonPoint.setX(rand()%NUM_COL);
      poisonPoint.setY(rand()%NUM_ROW);
      if(snake.contains(poisonPoint)|| (poisonPoint.x()==foodPoint.x() && poisonPoint.y() == foodPoint.y()))
          PoisonGenerate();
    }
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    
    //background picture
    painter.drawPixmap(MARGIN,MARGIN,NUM_COL*BLOCK_SIZE,NUM_ROW*BLOCK_SIZE,QPixmap("back.png"));
    //background with red border
    painter.setBrush(QColor(0,1,0,100));
    painter.setPen(Qt::red);
    painter.drawRect(MARGIN,MARGIN,NUM_COL*BLOCK_SIZE,NUM_ROW*BLOCK_SIZE);
    
    //blue snake with red border
    painter.setBrush(Qt::blue);
    painter.setPen(Qt::red);
    
    for(int i=1;i<snake.size();i++)
        painter.drawRect(MARGIN+snake[i].x()*BLOCK_SIZE,MARGIN+snake[i].y()*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
    
    //set the snake head as circle
    painter.drawEllipse(MARGIN+snake[0].x()*BLOCK_SIZE,MARGIN+snake[0].y()*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE); //snake head
    //painter.drawPixmap(MARGIN+snake[0].x()*BLOCK_SIZE,MARGIN+snake[0].y()*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE,QPixmap("head.png")); //snake head as picture but very slow not recommended
    for(int i=1;i<snake.size();i++)
        painter.drawRect(MARGIN+snake[i].x()*BLOCK_SIZE,MARGIN+snake[i].y()*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
        
    //red food
    //painter.setBrush(Qt::red);
    //painter.drawEllipse(MARGIN+foodPoint.x()*BLOCK_SIZE,MARGIN+foodPoint.y()*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
    painter.drawPixmap(MARGIN+foodPoint.x()*BLOCK_SIZE,MARGIN+foodPoint.y()*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE,QPixmap("food.png"));
    
    //yellow poison
    //painter.setBrush(Qt::yellow);
    //painter.drawEllipse(MARGIN+foodPoint.x()*BLOCK_SIZE,MARGIN+foodPoint.y()*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
    if(isPoison){
      painter.drawPixmap(MARGIN+poisonPoint.x()*BLOCK_SIZE,MARGIN+poisonPoint.y()*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE,QPixmap("poison.png"));
    }
    
    //black Arial 14 score information with grass background 
    painter.drawPixmap(0,NUM_ROW*BLOCK_SIZE,MARGIN*4+(NUM_COL+3)*BLOCK_SIZE,MARGIN*6+3*BLOCK_SIZE,QPixmap("grass.png"));
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial",14));
    painter.drawText(MARGIN*3+NUM_COL*BLOCK_SIZE,MARGIN+16*BLOCK_SIZE,"score: "+QString::number(score));
    painter.drawText(MARGIN*3+NUM_COL*BLOCK_SIZE,MARGIN+18*BLOCK_SIZE,"Best: "+QString::number(bestscore));
    //painter.drawText(MARGIN+2*BLOCK_SIZE,MARGIN*3+(NUM_ROW+1)*BLOCK_SIZE,"Press up,down,left,right to control the snake");
    painter.drawText(MARGIN+1*BLOCK_SIZE,MARGIN*3+(NUM_ROW+1)*BLOCK_SIZE,"Press w,s,a,d as up,down,left,right to control the snake");
    painter.drawText(MARGIN+1*BLOCK_SIZE,MARGIN*5+(NUM_ROW+2)*BLOCK_SIZE,"Press space or p to start or pause/continue the game; Press j to restart");     

}
*/
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
        qApp->quit();  
}