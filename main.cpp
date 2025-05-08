#include "waylandselectionlistener.h"
#include "widget.h"
#include <QApplication>


int main(int argc, char *argv[]) {

    QApplication a(argc, argv);
    qDebug()<<"init 21";
    qInfo()<<"init 21";
    qWarning()<<"init 21";
    qCritical()<<"init 21";
    Widget w;
    WaylandSelectionListener listener;
    if (!listener.initialize()) {
        qCritical() << "Failed to initialize Wayland listener";
        return 1;
    }else{
        qDebug()<<"init Success";
    }

    QObject::connect(&listener, &WaylandSelectionListener::selectionChanged,
                     &w, &Widget::setSelectedText);

    QObject::connect(&listener, &WaylandSelectionListener::selectionChanged,&w, &Widget::setCnt);
    w.show();

    return a.exec();
}
