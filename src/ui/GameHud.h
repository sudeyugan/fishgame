#ifndef GAMEHUD_H
#define GAMEHUD_H

#include <QWidget>
#include <QLabel>

class GameHud : public QWidget {
    Q_OBJECT
public:
    GameHud(QWidget *parent = nullptr);

public slots:
    void updateScore(int score);
    void updateLevel(int level);

private:
    QLabel *m_scoreLabel;
    QLabel *m_levelLabel;
};

#endif