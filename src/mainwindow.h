#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <set>
#include <QMainWindow>
#include <mutex>
#include <deque>
#include <string>
#include <QTimer>
#include <QListWidgetItem>
#include <QLabel>
#include <QShortcut>
#include <Python.h>

#include "glcanvas.hpp"
#include "stage.hpp"
#include "symbol_completer.h"
#include "bufferrequestmessage.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void plot_buffer(PyObject* buffer_metadata);
    ~MainWindow();

    void show();

    void draw();

    void resize_callback(int w, int h);

    void scroll_callback(float delta);

    void reset_ac_min_labels();
    void reset_ac_max_labels();

    void mouse_drag_event(int mouse_x, int mouse_y);
    void mouse_move_event(int mouse_x, int mouse_y);

    void set_plot_callback(int(*plot_cbk)(const char*));

    // Window change events - only called after the event is finished
    void resizeEvent(QResizeEvent*);
    void moveEvent(QMoveEvent*);

public Q_SLOTS:
    void show_context_menu(const QPoint &pos);

    void loop();
    void buffer_selected(QListWidgetItem * item);

    void ac_red_min_update();
    void ac_red_max_update();
    void ac_green_min_update();
    void ac_green_max_update();
    void ac_blue_min_update();
    void ac_blue_max_update();
    void ac_alpha_min_update();
    void ac_alpha_max_update();

    void ac_min_reset();
    void ac_max_reset();

    void ac_toggle();

    void recenter_buffer();

    void link_views_toggle();

    void remove_selected_buffer();

    void update_available_variables(PyObject* available_set);

    void on_symbol_selected();

    void on_symbol_completed(QString str);

    void export_buffer();

    void rotate_90_cw();

    void rotate_90_ccw();

private Q_SLOTS:
    void persist_settings_impl();

private:
    QTimer settings_persist_timer_;
    QTimer update_timer_;
    double render_framerate_;
    bool request_render_update_;

    Stage* currently_selected_stage_;
    std::map<std::string, std::shared_ptr<uint8_t>> held_buffers_;
    std::set<std::string> previous_session_buffers_;
    std::mutex mtx_;
    std::deque<BufferRequestMessage> pending_updates_;

    std::shared_ptr<QShortcut> symbol_list_focus_shortcut_;
    std::shared_ptr<SymbolCompleter> symbol_completer_;
    bool completer_updated_;
    QStringList available_vars_;

    Ui::MainWindow *ui_;
    bool ac_enabled_;
    bool link_views_enabled_;
    std::map<std::string, std::shared_ptr<Stage>> stages_;
    QLabel *status_bar;
    std::shared_ptr<QShortcut> buffer_removal_shortcut_;

    QListWidgetItem* generateListItem(BufferRequestMessage&);
    void set_ac_min_value(int idx, float value);
    void set_ac_max_value(int idx, float value);

    int(*plot_callback_)(const char*);

    void update_statusbar();

    std::string get_type_label(Buffer::BufferType type, int channels);
    void load_settings();
    void persist_settings();

    void set_currently_selected_stage(Stage* stage);
};

#endif // MAINWINDOW_H
