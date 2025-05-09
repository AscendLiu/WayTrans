#include "waylandselectionlistener.h"
#include "widget.h"
#include <QApplication>


int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    qDebug()<<"qDebug";
    qInfo()<<"qInfo";
    qWarning()<<"qWarning";
    qCritical()<<"qCritical";
    Widget w;
    WaylandSelectionListener listener;
    if (!listener.initialize()) {
        qCritical() << "Failed to initialize Wayland listener";
        return 1;
    }else{
        qWarning()<<"init Success";
    }

    QObject::connect(&listener, &WaylandSelectionListener::selectionChanged,
                     &w, &Widget::setSelectedText);

    QObject::connect(&listener, &WaylandSelectionListener::selectionChanged,&w, &Widget::setCnt);
    w.show();

    return a.exec();
}
