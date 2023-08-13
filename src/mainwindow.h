#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "table.h"
#include "editwindow.h"

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>


#include <vector>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    std::vector<Table*> tables;

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    size_t find_table(QTableWidget *table_widget);

    QTableWidget *get_qtable(size_t tab_index);
    void delete_table(size_t index);
    Column get_column();

protected:
    size_t get_unique_tab_number();

private slots:
    void on_action_new_triggered();

    void on_tab_widget_tabCloseRequested(int index);

    void on_action_open_triggered();

    void on_action_save_triggered();

    void on_action_save_as_triggered();

    void on_action_close_triggered();

    void on_action_close_all_triggered();

    void on_action_add_triggered();

    void on_action_edit_triggered();

    void on_action_remove_triggered();

    void on_action_filter_triggered();

    void on_action_show_chart_supply_rate_date_triggered();

private:
    Ui::MainWindow *ui;

    size_t unique_tab_number {};
};
#endif // MAINWINDOW_H
