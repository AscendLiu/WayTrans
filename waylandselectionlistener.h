#ifndef WAYLANDSELECTIONLISTENER_H
#define WAYLANDSELECTIONLISTENER_H

#include <QObject>
#include <QSocketNotifier>
#include <unistd.h>
#include <fcntl.h>
#include <wayland-client.h>
#include <wayland-client-protocol.h>
#include <QFile>
#include <QDebug>
#include "qwayland-wlr-data-control-unstable-v1.h"

class WaylandSelectionListener : public QObject,public QtWayland::zwlr_data_control_device_v1
{
    Q_OBJECT
public:
    explicit WaylandSelectionListener(QObject *parent = nullptr);
    ~WaylandSelectionListener();

    bool initialize();
    void handleWaylandEvent();

    static const wl_registry_listener s_registryListener;

    class DataControlDevice : public QtWayland::zwlr_data_control_device_v1{
    public:
        explicit DataControlDevice(WaylandSelectionListener *parent,struct ::zwlr_data_control_device_v1 *object):
            m_parent(parent),
            QtWayland::zwlr_data_control_device_v1(object) {}

    protected:
        void zwlr_data_control_device_v1_selection(struct ::zwlr_data_control_offer_v1 *id) override{
            qWarning()<<__FUNCTION__;

        }
        void zwlr_data_control_device_v1_finished() override{
            qWarning()<<__FUNCTION__;
        }
        void zwlr_data_control_device_v1_primary_selection(struct ::zwlr_data_control_offer_v1 *id) override{
            qWarning()<<__FUNCTION__;
            // 创建临时数据源触发数据发送
            DataControlSource *source = new DataControlSource(m_parent);
            auto *src = m_parent->m_manager->create_data_source();
            source->init(src);
            source->offer("text/plain");

            zwlr_data_control_device_v1_set_selection(object(), src);
            wl_display_flush(m_parent->m_display);
        }
        void zwlr_data_control_device_v1_data_offer(struct ::zwlr_data_control_offer_v1 *id) override{
            if (!id) return;
            qWarning()<<__FUNCTION__;

        }
    private:
        WaylandSelectionListener *m_parent;
    };

    class DataControlSource : public QtWayland::zwlr_data_control_source_v1 {
    public:
        explicit DataControlSource(WaylandSelectionListener *parent) : m_parent(parent) {}
    protected:
        void zwlr_data_control_source_v1_send(const QString &mime_type, int32_t fd) override {
            if (mime_type != "text/plain") {
                qWarning()<<"Erro Type";
                return;
            }

            QFile file;
            if (file.open(fd, QIODevice::ReadOnly)) {
                QByteArray data = file.readAll();
                emit m_parent->textSelected(QString::fromUtf8(data));
            }
            close(fd);
        }
    private:
        WaylandSelectionListener *m_parent;
    };

signals:
    void textSelected(const QString &text);

private:
    wl_display *m_display = nullptr;
    wl_registry *m_registry = nullptr;
    wl_seat *m_seat = nullptr;
    QtWayland::zwlr_data_control_manager_v1 *m_manager = nullptr;
    DataControlDevice *m_device = nullptr;
    QSocketNotifier *m_notifier = nullptr;
};

#endif // WAYLANDSELECTIONLISTENER_H
