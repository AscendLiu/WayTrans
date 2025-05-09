#ifndef WAYLANDSELECTIONLISTENER_H
#define WAYLANDSELECTIONLISTENER_H

#include <QObject>
#include <QString>
#include <QSocketNotifier>
#include "qwayland-primary-selection-unstable-v1.h"
#include <unistd.h>
#include <wayland-client.h>
#include <wayland-client-protocol.h>
#include <QFile>
#include <QDebug>


class WaylandSelectionListener : public QObject
{
    Q_OBJECT
public:
    explicit WaylandSelectionListener(QObject *parent = nullptr): QObject(parent){}
    ~WaylandSelectionListener();

    bool initialize();

    class SelectDevice : public QtWayland::zwp_primary_selection_device_v1{
    public:
        explicit SelectDevice(struct ::zwp_primary_selection_device_v1 *object) :
            QtWayland::zwp_primary_selection_device_v1(object){

        }
    protected:
        void zwp_primary_selection_device_v1_data_offer(struct ::zwp_primary_selection_offer_v1 *offer) override{
            qWarning()<<"YYDS:"<<__FUNCTION__;
        }
        void zwp_primary_selection_device_v1_selection(struct ::zwp_primary_selection_offer_v1 *id) override{
            qWarning()<<"YYDS:"<<__FUNCTION__;
        }
    };

    class SelectOffer : public QtWayland::zwp_primary_selection_offer_v1{
    public:
        explicit SelectOffer(struct ::zwp_primary_selection_offer_v1 *object) :
            QtWayland::zwp_primary_selection_offer_v1(object){

        }

        QString receiveData(const QString &mimeType) {
            int fds[2];
            if (pipe(fds) == -1) return QString();

            receive(mimeType, fds[1]);
            close(fds[1]);

            QFile readPipe;
            if (!readPipe.open(fds[0], QIODevice::ReadOnly)) {
                close(fds[0]);
                return QString();
            }

            QString data = QString::fromUtf8(readPipe.readAll());
            readPipe.close();
            return data;
        }

    protected:
        void zwp_primary_selection_offer_v1_offer(const QString &mime_type) override{
            qWarning()<<"YYDS:"<<__FUNCTION__;
            m_offeredMimeTypes.append(mime_type);
        }
    private:
        QStringList m_offeredMimeTypes;
    };

    class SelectSource : public QtWayland::zwp_primary_selection_source_v1{
    public:
        explicit SelectSource(struct ::zwp_primary_selection_source_v1 *object) :
            QtWayland::zwp_primary_selection_source_v1(object){

        }
    protected:
        virtual void zwp_primary_selection_source_v1_send(const QString &mime_type, int32_t fd) override{
            qWarning()<<"YYDS:"<<__FUNCTION__;
        }
        virtual void zwp_primary_selection_source_v1_cancelled() override{
            qWarning()<<"YYDS:"<<__FUNCTION__;
        }

    };

signals:
    void selectionChanged(const QString &text);
    void eventChanged();

private slots:
    void handleWaylandEvent(int socket);

private:
    static const wl_registry_listener s_registryListener;

    wl_display *m_display = nullptr;
    wl_registry *m_registry = nullptr;
    wl_seat *m_seat = nullptr;
    QtWayland::zwp_primary_selection_device_manager_v1 *m_manager = nullptr;
    SelectDevice *m_device = nullptr;
    QSocketNotifier *m_notifier = nullptr;

};

#endif // WAYLANDSELECTIONLISTENER_H
