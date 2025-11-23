#ifndef PAUSEDIALOG_H
#define PAUSEDIALOG_H

#include <QDialog>

class PauseDialog : public QDialog {
    Q_OBJECT
public:
    explicit PauseDialog(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void resumeGame();
    void saveGame();
    void quitToTitle();
};

#endif // PAUSEDIALOG_H