#ifndef SNAKE_H
#define SNAKE_H

#include <QWidget>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QtGui>
#include <QPushButton>
#include <QGridLayout>
#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    const int ROWS = 10;
    const int COLS = 10;
    const int NUM_MINES = 10;
    const int BLOCK_SIZE=30;
    const int MARGIN=5; 

    bool flag_state = 0;

    QPushButton *buttons[12][12];
    QPushButton *flagbutton;
    QGridLayout *gridLayout;
    int map[12][12];
    int searched_map[12][12];

    void Init();
    void Search(int row,int col);
    void Lose();
    void Win();

private slots:
    void Mine();
    void Flag_change();
};

#endif // WIDGET_H
