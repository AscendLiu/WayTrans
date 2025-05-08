#include "waylandselectionlistener.h"
#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Widget w;
    WaylandSelectionListener listener;
    if (!listener.initialize()) {
        qCritical() << "Failed to initialize Wayland listener";
        return 1;
    }
    QObject::connect(&listener, &WaylandSelectionListener::textSelected,
                     &w, &Widget::setSelectedText);

    QObject::connect(&listener, &WaylandSelectionListener::textSelected,[&](const QString &text){
        qWarning() << "Select:"<<text;
    });
    w.show();

    return a.exec();
}
