#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <QWidget>

class StartScreen : public QWidget {
    Q_OBJECT
public:
    StartScreen(QWidget *parent = nullptr);
signals:
    void startClicked();
    void helpClicked();
    void quitClicked();
};

#endif