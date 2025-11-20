#ifndef LEVELINFODIALOG_H
#define LEVELINFODIALOG_H

#include <QDialog>

class LevelInfoDialog : public QDialog {
    Q_OBJECT
public:
    LevelInfoDialog(int level, const QString& description, QWidget *parent = nullptr);
};

#endif