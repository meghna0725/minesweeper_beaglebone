#ifndef SNAKE_H
#define SNAKE_H

#include <QWidget>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QtGui>
#include <QPushButton>
#include <QGridLayout>
#include <QMainWindow>
#include <QMap>
#include <QSlider>

/*
enum Mode{
    Easy,
    Medium,
    Hard
};

enum MapSize {
    Small,
    Medium,
    Large
};
*/


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    int ROWS = 10;
    int COLS = 10;
    int NUM_MINES = 10;
    int BLOCK_SIZE= 27;
    int MARGIN=5; 
    const int BOUND = 10;

    bool flag_state = 0;
    int size = 3;
    int mode = 1;

    QPushButton *buttons[12][12];
    QPushButton *flagbutton;
    QPushButton *ss;//small-size
    QPushButton *ms;
    QPushButton *ls;
    QPushButton *em;//easy-mode
    QPushButton *mm;
    QPushButton *hm;

    QGridLayout *gridLayout;
    int map[12][12];
    int searched_map[12][12];
    int flag[12][12];

/*
    QMap<int, Mode> modeMap;
    QMap<int, MapSize> sizeMap;
    QSlider *slider;
    QLabel *OptionalLabel;
    */

    void Init();
    void Search(int row,int col);
    void Lose();
    void Win();
    void set();

private slots:
    void Mine();
    void Flag_change();
    void ssc();
    void msc();
    void lsc();
    void emc();
    void mmc();
    void hmc();
};

#endif // WIDGET_H
