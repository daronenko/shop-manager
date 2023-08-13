#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include "table.h"

#include <QWidget>
#include <QMessageBox>


namespace Ui {
class EditWindow;
}

class EditWindow : public QWidget {
    Q_OBJECT

public:
    enum class WindowState {
        APPENDING,
        EDITING
    } State;

    Item *item {};
    Table *table {};

    explicit EditWindow(WindowState State, Table *table, Item *item, QWidget *parent = nullptr);
    explicit EditWindow(WindowState State, Table *table, QWidget *parent = nullptr);
    ~EditWindow();

    void clear();
    void load_item();
    void save_changes();

    Item* build_item();
    void append_item(Item *item);
    bool is_valid();

private slots:
    void on_apply_button_clicked();

private:
    Ui::EditWindow *ui;
};

#endif // EDITWINDOW_H
