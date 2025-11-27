#ifndef CHARACTERSELECTDIALOG_H
#define CHARACTERSELECTDIALOG_H

#include <QDialog>
#include "../core/Constants.h"

class CharacterSelectDialog : public QDialog {
    Q_OBJECT

public:
    explicit CharacterSelectDialog(QWidget *parent = nullptr);
    CharacterType getSelectedCharacter() const;

private:
    CharacterType m_selectedType;
    void setupUi();
    // 创建一个角色卡片的辅助函数
    QWidget* createCharacterCard(const QString &name, const QString &desc, const QString &imagePath, CharacterType type);
};

#endif // CHARACTERSELECTDIALOG_H