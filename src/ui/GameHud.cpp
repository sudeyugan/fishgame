#include "GameHud.h"
#include <QPainter>
#include <QPainterPath>

GameHud::GameHud(QWidget *parent) : QWidget(parent), m_score(0), m_level(1) {
    // 否则鼠标点在 HUD 区域（比如左上角）时，鱼就不会动了
    setAttribute(Qt::WA_TransparentForMouseEvents);
    
    // 设置窗口本身透明，只显示我们画的内容
    setAttribute(Qt::WA_TranslucentBackground);

}

void GameHud::updateScore(int score) {
    m_score = score;
    update(); // 触发重绘 (调用 paintEvent)
}

void GameHud::updateLevel(int level) {
    m_level = level;
    update();
}

void GameHud::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); // 抗锯齿，文字更平滑

    // --- 1. 绘制半透明背景底板 ---
    int bgWidth = 220;
    int bgHeight = 90;
    int margin = 20;
    QRect bgRect(margin, margin, bgWidth, bgHeight);
    
    painter.setBrush(QColor(0, 0, 0, 150)); // 黑色，60%透明度
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(bgRect, 15, 15);

    // --- 2. 准备字体 ---
    QFont font("Arial Black", 16, QFont::Bold);
    // 如果系统没有 Arial Black，会回退到 Arial
    painter.setFont(font);

    // --- 3. 绘制文字函数 (带描边效果) ---
    // 这是一个 lambda 辅助函数，用来画带边框的字
    auto drawOutlinedText = [&](int x, int y, const QString &text, const QColor &fillColor) {
        QPainterPath path;
        // 在 (x, y) 位置创建文字路径
        path.addText(x, y, font, text);

        // 先画描边 (黑色，粗细 3)
        painter.strokePath(path, QPen(QColor(0, 20, 40), 4));
        painter.drawPath(path);

        // 再画填充 (指定的颜色)
        painter.fillPath(path, QBrush(fillColor));
    };

    // --- 4. 绘制 "LEVEL" ---
    // 亮青色，突显科技感/海洋感
    QString levelText = QString("LEVEL  %1").arg(m_level);
    drawOutlinedText(margin + 20, margin + 35, levelText, QColor(0, 255, 255));

    // --- 5. 绘制 "SCORE" ---
    // 金黄色，突显奖励感
    // 把字体稍微加大一点
    QFont scoreFont("Arial Black", 22, QFont::Bold);
    painter.setFont(scoreFont);
    
    QString scoreText = QString("SCORE: %1").arg(m_score);
    
    // 重新定义绘制逻辑因为字体变了 (或者直接手动画)
    QPainterPath scorePath;
    scorePath.addText(margin + 20, margin + 75, scoreFont, scoreText);
    
    painter.strokePath(scorePath, QPen(QColor(0, 20, 40), 5));
    painter.drawPath(scorePath);
    painter.fillPath(scorePath, QBrush(QColor(255, 215, 0))); // 金色
}