#include "widget.h"

#include <QLabel>
#include <QVBoxLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    // 创建UI组件
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    m_sourceTextEdit = new QTextEdit(this);
    m_sourceTextEdit->setReadOnly(true);

    m_translatedTextEdit = new QTextEdit(this);
    m_translatedTextEdit->setReadOnly(true);

    m_translateButton = new QPushButton("翻译", this);
    connect(m_translateButton,&QPushButton::clicked,[](){
        qDebug()<<"click";
    });
    layout->addWidget(new QLabel("选中的文本:", this));
    layout->addWidget(m_sourceTextEdit);
    layout->addWidget(new QLabel("翻译结果:", this));
    layout->addWidget(m_translatedTextEdit);
    layout->addWidget(m_translateButton);

    resize(400,300);
    // setCentralWidget(centralWidget);
}

Widget::~Widget() {}

void Widget::setSelectedText(const QString &text)
{
    m_sourceTextEdit->setPlainText(text);
}

void Widget::setCnt()
{
    m_sourceTextEdit->setPlainText(QString::number(cnt++));
}
