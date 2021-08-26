//
// Created by xtx on 2021/8/12.
//

#include <iostream>
#include <QPropertyAnimation>
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Player.h"

using namespace std;

extern Player *playerList[2];//玩家对象表

extern vector<QPushButton *> HandCardGroup;


void MainWindow::cardAllDown() {
    for (auto &i : HandCardGroup) {
        if (i->geometry().y() == 0) {
            auto *pScaleAnimation1 = new QPropertyAnimation(i, "geometry");
            pScaleAnimation1->setStartValue(QRect(i->geometry().x(), 0, width, height));//初状态
            pScaleAnimation1->setEndValue(QRect(i->geometry().x(), 50, width, height));//末状态
            animeGroup->addAnimation(pScaleAnimation1);
        }
    }

}

void MainWindow::cardChooseAnime(bool single, QPushButton *a, ...) {
    bool isWantTop=false;
    if(a->geometry().y()==50)isWantTop=true;
    //此时如果是50就是下方,0就是上方
    animeGroup = new QParallelAnimationGroup(this);
    if (single)cardAllDown();
//    va_list ap;
//    va_start(ap, a);
//    QPushButton* i;//遍历

//    while(true) {
//        i=va_arg( ap, QPushButton*);
//        cout<<i;
//        if(i==ui->testButton)break;
    cardUpDown(single, a);

//   }
    if (mainState == playing) {
        //TODO: 原来打算说先等上下动画绘制完再做分层(emmm其实本身也不太好),结果发现下来的时候connect不成功
//        cout<<123;
//        disconnect(animeGroup);
//        connect(animeGroup, &QPropertyAnimation::finished, this, [=] {
//            setHandLevel();//重绘分层
//            if(isWantTop) {
//                a->raise();
//            }
//        });
    }
    setHandLevel();//重绘分层
    if(isWantTop) {
        a->raise();
    }
    animeGroup->start();
}

void MainWindow::cardUpDown(bool single, QPushButton *a) {
    int y = a->geometry().y();

    bool isTop = y != 50;
    auto *pScaleAnimation1 = new QPropertyAnimation(a,
                                                    "geometry");//geometry大小位置，pos角度，透明度后面说，见https://blog.csdn.net/weixin_42347660/article/details/113314656
    pScaleAnimation1->setDuration(100);//时间
    if (isTop && !single) {

        pScaleAnimation1->setStartValue(QRect(a->geometry().x(), 0, width, height));//初状态
        pScaleAnimation1->setEndValue(QRect(a->geometry().x(), 50, width, height));//末状态
    } else {
        pScaleAnimation1->setStartValue(QRect(a->geometry().x(), 50, width, height));//初状态
        pScaleAnimation1->setEndValue(QRect(a->geometry().x(), 0, width, height));//末状态
    }
    animeGroup->addAnimation(pScaleAnimation1);
}

void MainWindow::askChoose(PlayerID PlayerID, int num, tipsType tipsType, cardSpecies cardSpecies) {
    QString str;
    switch (tipsType) {

        case giveUp:
            ui->tips->setText(str.sprintf("请弃置%d张牌", num));
            break;
        case OutHand:
            ui->tips->setText(str.sprintf("请出牌"));
            //timerRun(playingHandTimer);//TODO: 定时器,暂时注释
            break;
    }

    for (int i = 0; i < playerList[PlayerID]->playerHandHeap.size(); i++) {
        //确定该牌是否可以使用
        if (!playerList[PlayerID]->playerHandHeap[i].isUseInPlayingRound)
            HandCardGroup[i]->setEnabled(false);
        //绑定槽
        connect(HandCardGroup[i], &QPushButton::clicked, this, [=] {
            cardChooseAnime(true, HandCardGroup[i]);
        });
    }


}

void MainWindow::timerRun(timerType type, int sec) {
    timeRound = sec * 10;
    this->timerNowType = type;
    timerWork();
    ui->timeBar->show();
    connect(qtimer, SIGNAL(timeout()), this, SLOT(timeout()));
}

void MainWindow::timerWork() {
    qtimer->start(timeRound);
    ui->timeBar->setValue(100);
    timerTemp = 100;
}

void MainWindow::timeout() {
    timerTemp--;
    ui->timeBar->setValue(timerTemp);
    if (timerTemp == 0) {
        cout << "TimeOut!";
        disconnect(qtimer);
        qtimer->stop();
        //到这一步证明用户没有选择,超时处理代码
        switch (this->timerNowType) {
            case choosingHeroTimer: {
                vector<int> a = {1};
                chooseFinish(a);
                break;
            }
            case playingHandTimer:
                break;
        }


    }
}

void MainWindow::repaintHands(bool isSetLocated) {
    QString str;
    int base = 0;
    int num = playerList[0]->playerHandHeap.size();
    for (int i = 0; i < num; i++)HandCardGroup[i]->show();
    for (int i = num; i < HandCardGroup.size(); i++)HandCardGroup[i]->hide();
    if (num > 4)base = 800 / num;
    else base = 200;
    for (int i = 0; i < num; i++) {
        //绘制背景

        HandCardGroup[i]->setIcon(QIcon(str.sprintf(":/hands/%d.png", playerList[0]->playerHandHeap[i].Species)));
        //HandCardGroup[i]->setStyleSheet(str.sprintf("border-image: url(://hands/%d.png);",playerList[0]->playerHandHeap[i].Species));

        //刷新所有牌的位置在下
        HandCardGroup[i]->setGeometry(QRect(base * i, 50, 200, 290));

        HandCardGroup[i]->raise();

        HandCardGroup[i]->setToolTip(QString::fromStdString(playerList[0]->playerHandHeap[i].name));
        HandCardGroup[i]->setEnabled(true);//恢复正常按钮状态
    }
}

void MainWindow::setHandLevel() {
    for (auto &i : HandCardGroup) i->raise();
}
