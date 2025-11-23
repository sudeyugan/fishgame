#ifndef GAMEHUD_H
#define GAMEHUD_H

#include <QWidget>

class GameHud : public QWidget {
    Q_OBJECT
public:
    explicit GameHud(QWidget *parent = nullptr);

public slots:
    void updateScore(int score);
    void updateLevel(int level);
    void updateTarget(int target);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_score;
    int m_level;
    int m_targetScore;
};

#endif // GAMEHUD_H