//
// Created by xtx on 2021/8/17.
//

#ifndef QTCMAKE_PLAYER_H
#define QTCMAKE_PLAYER_H


#include "mainwindow.h"

using namespace std;

class Player : MainWindow {

public:
    // 回合事件
    Player(int i);//主构造函数
    void startOfRoundEvent() {};//回合开始阶段
    void judgmentStageEvent() { doJudgmentStage(this); };//判定阶段
    void drawStageEvent() { doDrawStage(this); };//摸牌阶段
    void playStageEvent() { doPlayStage(this); };//出牌阶段
    void foldPhaseEvent() { doFoldPhase(this); };//弃牌阶段
    void endOfRoundEvent() {};//回合结束阶段

    //回合中事件
    void getHandEvent(int num);

    void bloodChangeEvent(int num, sourceOfDamage sourceOfDamage = None, Player* player = nullptr);

    void getPlayerHandEvent(Player* player,int num,bool type = false);

    void giveUpHand(Player player, int num, bool type = false);

    //人物参数
    int bloodTop;
    int bloodNow = bloodTop;
    int HandTop = bloodNow;
    vector<Hands> playerHandHeap;
private:
    void doJudgmentStage(Player* player);

    void doDrawStage(Player* player);

    void doPlayStage(Player* player);

    void doFoldPhase(Player* player);

};


#endif //QTCMAKE_PLAYER_H