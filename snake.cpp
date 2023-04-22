#include <QPainter>
#include <time.h>
#include <QTimer>
#include <QString>
#include <QMessageBox>
#include <QApplication>
//#include <QtGui>
#include "snake.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    /*
    saveAction = new QAction(tr("&Save"), this);
    exitAction = new QAction(tr("E&xit"), this);

    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

    //fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu = new QMenu("&File",menuBar());
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
    */
    
    //Init window size; leave space for scores:x size = background + score information in paintEvent function
    resize(MARGIN*4+(NUM_COL+3)*BLOCK_SIZE,MARGIN*6+(NUM_ROW+3)*BLOCK_SIZE);
    /*
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/back.png").scaled(this->size())));
    this->setPalette(palette);
    */
    Init();
}

Widget::~Widget()
{

}

void Widget::Init()
{

    //QGridLayout *map = new QGridLayout(this);

    for(int i = 0; i < map_size; i++)
    {
        for(int j = 0; j < map_size; j++)
        {
            //QPushButton *button = new QPushButton(this);
            map[i][j] = new QPushButton(this);
            //map->addWidget(button, i, j);
            map[i][j]->setText("1");
            map[i][j]->setGeometry(MARGIN+i*BLOCK_SIZE,MARGIN+j*BLOCK_SIZE,BLOCK_SIZE,BLOCK_SIZE);
            connect(map[i][j], SIGNAL(clicked()), this, SLOT(mine(i,j)));
        }
    }

    for(int i = 0; i < map_size+2; i++)
    {
        for(int j = 0; j < map_size+2; j++)
        {
            actual_map[i][j] = 0;
        }
    }

    for(int i = 0; i < mine_num; i++)
    {
        int loc = rand()%(map_size*map_size);
        int loc_x = loc/map_size;
        int loc_y = loc%map_size;
        if (actual_map[loc_x+1][loc_y+1]==1)
        actual_map[loc_x+1][loc_y+1] = 1;

    }
/*
    //Init snake; length = 5
    for(int j=4;j>=0;j--)
        snake.push_back(QPoint(j,0));  //adding to the tail of QList<QPoint> snake
    dir=RIGHT;//Init direction as right
    //Init Food
    srand(time(0));
    FoodGenerate();
    
    isPoison = false; //no poison at first
    PoisonGenerate();
    
    //Init game score
    score=0;
    bestscore=0;
 
    //isPause=false;  //start at first
    isPause = true; //pause at first
    
   
    gameTimer=new QTimer(this);
    connect(gameTimer,SIGNAL(timeout()),this,SLOT(Update()));
    
    //gameTimer->start(TIME_INTERVAL); //start at first
    gameTimer->stop();
    
    quitbutton = new QPushButton(this);
    quitbutton->setText("quit");
    quitbutton->setGeometry(MARGIN*3+NUM_COL*BLOCK_SIZE,MARGIN+1*BLOCK_SIZE,BLOCK_SIZE*2,BLOCK_SIZE*2);
    connect(quitbutton, SIGNAL(clicked()), this, SLOT(quit()));
*/
    /*
    start = new QPushButton(this);
    start->setText("Go/Pause");
    //start = new QPushButton(tr("start"));
    //start->setStyleSheet("background-color: rgb(170, 0, 255);");
    start->setGeometry(MARGIN*3+NUM_COL*BLOCK_SIZE,MARGIN+4*BLOCK_SIZE,BLOCK_SIZE*2,BLOCK_SIZE*2);
    //start->setFlat(true);
    connect(start, SIGNAL(clicked()), this, SLOT(Start()));
    */
/*
    pause = new QPushButton(this);
    pause->setText("Go/Stop");
    pause->setGeometry(MARGIN*3+NUM_COL*BLOCK_SIZE,MARGIN+4*BLOCK_SIZE,BLOCK_SIZE*2,BLOCK_SIZE*2);
    connect(pause, SIGNAL(clicked()), this, SLOT(Pause()));
    
    restart = new QPushButton(this);
    restart->setText("Restart");
    restart->setGeometry(MARGIN*3+NUM_COL*BLOCK_SIZE,MARGIN+7*BLOCK_SIZE,BLOCK_SIZE*2,BLOCK_SIZE*2);
    connect(restart, SIGNAL(clicked()), this, SLOT(Restart()));
    
    poisonButton = new QPushButton(this);
    poisonButton->setText("Poison");
    poisonButton->setGeometry(MARGIN*3+NUM_COL*BLOCK_SIZE,MARGIN+10*BLOCK_SIZE,BLOCK_SIZE*2,BLOCK_SIZE*2);
    connect(poisonButton, SIGNAL(clicked()), this, SLOT(ActivePoison()));
    
    helpbutton = new QPushButton(this);
    helpbutton->setText("Help");
    helpbutton->setGeometry(MARGIN*3+NUM_COL*BLOCK_SIZE,MARGIN+13*BLOCK_SIZE,BLOCK_SIZE*2,BLOCK_SIZE*2);
    connect(helpbutton, SIGNAL(clicked()), this, SLOT(help()));
*/
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

void Widget::keyPressEvent(QKeyEvent *event)
{
    //key press (up,down,left,right) to control the snake
    switch(event->key())
    {
    
    case Qt::Key_Up: 
        if(dir!=DOWN) //snake can not go back
            dir=UP;
        break;
    case Qt::Key_Down:
        if(dir!=UP)
            dir=DOWN;
        break;
    case Qt::Key_Left:
        if(dir!=RIGHT)
            dir=LEFT;
        break;
    case Qt::Key_Right:
        if(dir!=LEFT)
            dir=RIGHT;
        break;
    
    case Qt::Key_P:
        PauseGame();
        break;    
    case Qt::Key_Space:
        PauseGame();
        break;
    case Qt::Key_W: 
        if(dir!=DOWN) //snake can not go back
            dir=UP;
        break;
    case Qt::Key_S:
        if(dir!=UP)
            dir=DOWN;
        break;
    case Qt::Key_A:
        if(dir!=RIGHT)
            dir=LEFT;
        break;
    case Qt::Key_D:
        if(dir!=LEFT)
            dir=RIGHT;
        break;
    case Qt::Key_J: //restart
        snake.clear();
        for(int j=4;j>=0;j--)
          snake.push_back(QPoint(j,0));
        dir=RIGHT;
        srand(time(0));
        FoodGenerate();
        PoisonGenerate();
        score=0;
        gameTimer->start(TIME_INTERVAL);
        break;
    default:
        break;
    }
}

bool Widget::isGameOver()
{
    int x=snake.front().x();
    int y=snake.front().y();
    //against the wall
    if(x<0||x>NUM_COL-1||y<0||y>NUM_ROW-1)
        return true;
    //against the snake itself
    for(int i=3;i<snake.size();i++)
        if(snake[i]==snake.front())
            return true;
    if(snake.contains(poisonPoint) && isPoison)
      return true;       
    return false;
}

void Widget::Update()
{
    //snake move by deleting the tail and adding the head
    //add the head
    switch(dir)
    {
    case UP:
        snake.push_front(QPoint(snake.front().x(),snake.front().y()-1));
        break;
    case DOWN:
        snake.push_front(QPoint(snake.front().x(),snake.front().y()+1));
        break;
    case LEFT:
        snake.push_front(QPoint(snake.front().x()-1,snake.front().y()));
        break;
    case RIGHT:
        snake.push_front(QPoint(snake.front().x()+1,snake.front().y()));
        break;
    default:
        break;
    }
    //snake eat the food
    if(snake.contains(foodPoint))
    {
        score+=1; 
        
        if(bestscore < score)
          bestscore = score;
          
        FoodGenerate(); 
        PoisonGenerate();
    }
    else
        snake.pop_back(); //delete the tail
   
    if(isGameOver())
    {
        GameOver();
        //return;
    }
    update(); 
}

void Widget::GameOver()
{
    gameTimer->stop();
    //QMessageBox::information(this,"failed","game over!");
    QMessageBox messageBox;
    messageBox.setWindowTitle(tr("GAME OVER"));
    messageBox.setText(tr("Do you want to restart?"));
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    if (messageBox.exec() == QMessageBox::Yes)
    {
        snake.clear(); //clear QList<QPoint> snake to restart
        for(int j=4;j>=0;j--)
          snake.push_back(QPoint(j,0));
        dir=RIGHT;
        srand(time(0));
        FoodGenerate();
        PoisonGenerate();
        score=0;
        gameTimer->start(TIME_INTERVAL);
    }
    if (messageBox.exec() == QMessageBox::No)
        qApp->quit();  
}

void Widget::PauseGame()
{
    if(!isPause)
    {
        isPause=!isPause;
        gameTimer->stop();
        QMessageBox::information(this,"Stopped","Press p to continue",QMessageBox::NoButton);
    }
    else
    {
        isPause=!isPause;
        gameTimer->start(TIME_INTERVAL);
    }
}

void Widget::quit()
{
    isPause=true;
    gameTimer->stop();
    QMessageBox messageBox1;
    messageBox1.setWindowTitle(tr("Quit"));
    messageBox1.setText(tr("Do you really want to quit?"));
    messageBox1.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    messageBox1.setDefaultButton(QMessageBox::No);
    if (messageBox1.exec() == QMessageBox::Yes)
        qApp->quit();
}


void Widget::Start()
{  
        isPause=false;
        gameTimer->start(TIME_INTERVAL);
}

void Widget::Pause()
{  
        //isPause=true;
        //gameTimer->stop();
    if(!isPause)
    {
        isPause=!isPause;
        gameTimer->stop();
    }
    else
    {
        isPause=!isPause;
        gameTimer->start(TIME_INTERVAL);
    }
}

void Widget::Restart()
{  
        snake.clear(); //clear QList<QPoint> snake to restart
        for(int j=4;j>=0;j--)
          snake.push_back(QPoint(j,0));
        dir=RIGHT;
        srand(time(0));
        FoodGenerate();
        PoisonGenerate();
        score=0;
        gameTimer->start(TIME_INTERVAL);
}


void Widget::save()
{

}


void Widget::help()
{
    QMessageBox::information(this,"Help","Press w,s,a,d as up,down,left,right to control the snake.\n Eat red food to get score.\n Ate yellow poison, game over",QMessageBox::NoButton);
}

void Widget::ActivePoison()
{
      isPoison=!isPoison;
      PoisonGenerate();
}
*/
void Widget::mine(int a, int b)
{
    map[a][b]->setText("2");
    return;
}