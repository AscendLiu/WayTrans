#include "waylandselectionlistener.h"
#include <QSocketNotifier>
#include <QFile>

const struct wl_registry_listener WaylandSelectionListener::s_registryListener = {
    [](void *data, wl_registry *registry, uint32_t name,
        const char *interface, uint32_t version) {
        auto self = static_cast<WaylandSelectionListener*>(data);

        if (strcmp(interface, "wl_seat") == 0) {
            self->m_seat = static_cast<wl_seat*>(
                wl_registry_bind(registry, name, &wl_seat_interface, 1));
        }
        else if (strcmp(interface, "zwlr_data_control_manager_v1") == 0) {
            self->m_manager = new QtWayland::zwlr_data_control_manager_v1(
                registry, name, version);
        }
    },
    [](void *data, wl_registry *registry, uint32_t name) {
        Q_UNUSED(data);
        Q_UNUSED(registry);
        Q_UNUSED(name);
    }
};

WaylandSelectionListener::WaylandSelectionListener(QObject *parent)
    : QObject(parent){}

WaylandSelectionListener::~WaylandSelectionListener()
{
    delete m_notifier;
    m_notifier = nullptr;

    if (m_device) {
        delete m_device;
        m_device = nullptr;
    }

    if (m_manager) {
        delete m_manager;
        m_manager = nullptr;
    }

    if (m_seat) {
        wl_seat_destroy(m_seat);
        m_seat = nullptr;
    }

    if (m_registry) {
        wl_registry_destroy(m_registry);
        m_registry = nullptr;
    }

    if (m_display) {
        wl_display_disconnect(m_display);
        m_display = nullptr;
    }

}

bool WaylandSelectionListener::initialize()
{
    m_display = wl_display_connect(nullptr);
    if (!m_display) {
        qWarning() << "Failed to connect to Wayland display";
        return false;
    }

    m_registry = wl_display_get_registry(m_display);
    wl_registry_add_listener(m_registry, &s_registryListener, this);
    wl_display_roundtrip(m_display);

    if (!m_manager || !m_seat) {
        qWarning() << "Missing required Wayland interfaces";
        return false;
    }

    // 设置数据设备监听
    m_device = new DataControlDevice(this,m_manager->get_data_device(m_seat));

    // 设置事件监听
    int fd = wl_display_get_fd(m_display);
    m_notifier = new QSocketNotifier(fd, QSocketNotifier::Read, this);
    connect(m_notifier, &QSocketNotifier::activated,  this, &WaylandSelectionListener::handleWaylandEvent);
    wl_display_flush(m_display);  // 确保发送所有待处理请求
    wl_display_roundtrip(m_display);  // 阻塞直到收到所有响应, 完成跟服务器的同步

    return true;
}

void WaylandSelectionListener::handleWaylandEvent()
{
    qWarning()<<__FUNCTION__;
    // 1. 处理所有待处理的Wayland事件
    wl_display_dispatch_pending(m_display);

    // 2. 准备读取新事件
    while (wl_display_prepare_read(m_display) != 0) {
        wl_display_dispatch_pending(m_display);
    }

    wl_display_read_events(m_display);

    // 3. 分发处理新事件
    int ret = wl_display_dispatch(m_display);
    if (ret == -1) {
        qWarning() << "Wayland connection error";
        // cleanupWayland();
        return;
    }

    // 4. 确保所有请求已发送
    wl_display_flush(m_display);
}





