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

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_score;
    int m_level;
};

#endif // GAMEHUD_H