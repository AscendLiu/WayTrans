#ifndef WIDGET_H
#define WIDGET_H

#include <QPushButton>
#include <QTextEdit>
#include <QWidget>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

public slots:
    void setSelectedText(const QString &text);

private:
    QTextEdit *m_sourceTextEdit;
    QTextEdit *m_translatedTextEdit;
    QPushButton *m_translateButton;

};
#endif // WIDGET_H
