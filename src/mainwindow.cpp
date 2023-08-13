#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tablewidget.h"
#include "ui_tablewidget.h"
#include "chartwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tab_widget->clear();
}

MainWindow::~MainWindow() {
    delete ui;
}

size_t MainWindow::find_table(QTableWidget *table_widget) {
    for (size_t i {}; i < tables.size(); ++i) {
        if (table_widget == tables[i]->table)
            return i;
    }

    return tables.size();
}

QTableWidget *MainWindow::get_qtable(size_t tab_index) {
    QTableWidget *table_widget {};
    QWidget* pWidget= ui->tab_widget->widget(tab_index);
    if (pWidget->metaObject()->className() == "QTableWidget") {
        table_widget = (QTableWidget*)pWidget;
    } else {
        QList<QTableWidget*> all_table_widgets = pWidget->findChildren<QTableWidget*>();
        if (all_table_widgets.count() != 1)
            throw;
        table_widget = all_table_widgets[0];
    }

    return table_widget;
}

void MainWindow::delete_table(size_t index) {
    delete tables[index];
    tables.erase(tables.begin() + index);
}

void MainWindow::on_action_new_triggered() {
    TableWidget *table_widget {new TableWidget()};
    ui->tab_widget->insertTab(ui->tab_widget->count(), table_widget, QIcon(QString("")), QString("New Tab %1").arg(get_unique_tab_number()));
    ui->tab_widget->setCurrentIndex(ui->tab_widget->count() - 1);

    Table *table {new Table(table_widget->ui->table)};
    tables.push_back(table);
    table->sync();
}


void MainWindow::on_tab_widget_tabCloseRequested(int index) {
    QTableWidget *qtable {get_qtable(index)};
    size_t table_index {find_table(qtable)};
    Table *table {tables[table_index]};

    if (table->is_saved) {
        ui->tab_widget->removeTab(index);
    } else {
        QMessageBox::StandardButton reply {QMessageBox::warning(
            this,
            "Несохраненные данные", QString("Вы хотите сохранить изменения, которые вы сделали в таблице \"%1\"?").arg(ui->tab_widget->tabText(index)),
            QMessageBox::Cancel | QMessageBox::No | QMessageBox::Yes
        )};

        switch (reply) {
            case (QMessageBox::Yes):
                if (table->file->fileName() == "")
                    table->file->setFileName(QFileDialog::getSaveFileName(this, "Сохранить как"));
                table->save();
                ui->tab_widget->removeTab(index);
                delete_table(table_index);
                break;

            case (QMessageBox::No):
                ui->tab_widget->removeTab(index);
                break;

            case (QMessageBox::Cancel):
                break;
        }
    }
}

void MainWindow::on_action_open_triggered() {
    QString filename {QFileDialog::getOpenFileName(this, "Открыть файл")};
    TableWidget *table_widget {new TableWidget()};
    try {
        tables.push_back(new Table(table_widget->ui->table, filename));
    } catch (const char *err) {
        qDebug() << err;
        delete table_widget;
        return;
    }

    ui->tab_widget->insertTab(ui->tab_widget->count(), table_widget, QIcon(QString("")), filename);
    ui->tab_widget->setCurrentIndex(ui->tab_widget->count() - 1);

    QTableWidget *qtable {get_qtable(ui->tab_widget->currentIndex())};
    Table *table {tables[find_table(qtable)]};
    table->sync();
}


void MainWindow::on_action_save_triggered() {
    if (ui->tab_widget->count()) {
        QTableWidget *qtable {get_qtable(ui->tab_widget->currentIndex())};
        Table *table {tables[find_table(qtable)]};

        if (table->file->fileName() == "")
            table->file->setFileName(QFileDialog::getSaveFileName(this, "Сохранить как"));

        try {
            table->save();
        } catch (const char *err) {
            qDebug() << err;
            return;
        }

        ui->tab_widget->setTabText(ui->tab_widget->currentIndex(), table->file->fileName());
    }
}


void MainWindow::on_action_save_as_triggered() {
    if (ui->tab_widget->count()) {
        QTableWidget *qtable {get_qtable(ui->tab_widget->currentIndex())};
        Table *table {tables[find_table(qtable)]};

        QString filename {QFileDialog::getSaveFileName(this, "Сохранить как")};
        try {
            table->save();
        } catch (const char *err) {
            qDebug() << err;
            return;
        }

        if (table->file->fileName() == "") {
            table->file->setFileName(filename);
            ui->tab_widget->setTabText(ui->tab_widget->currentIndex(), table->file->fileName());
        }
    }
}


void MainWindow::on_action_close_triggered() {
    on_tab_widget_tabCloseRequested(ui->tab_widget->currentIndex());
}


void MainWindow::on_action_close_all_triggered() {
    for (int i {}; i < ui->tab_widget->count(); ++i) {
        ui->tab_widget->setCurrentIndex(0);
        on_tab_widget_tabCloseRequested(0);
    }
}

size_t MainWindow::get_unique_tab_number() {
    ++unique_tab_number;
    return unique_tab_number;
}

void MainWindow::on_action_add_triggered() {
    if (ui->tab_widget->count()) {
        QTableWidget *qtable {get_qtable(ui->tab_widget->currentIndex())};
        Table *table {tables[find_table(qtable)]};

        EditWindow *window {new EditWindow(EditWindow::WindowState::APPENDING, table)};
        window->setAttribute(Qt::WA_DeleteOnClose);
        window->show();
    }
}


void MainWindow::on_action_edit_triggered() {
    if (ui->tab_widget->count()) {
        QTableWidget *qtable {get_qtable(ui->tab_widget->currentIndex())};
        Table *table {tables[find_table(qtable)]};

        QItemSelectionModel *selectionModel = qtable->selectionModel();
        QModelIndexList selectedRows = selectionModel->selectedRows();
        std::sort(selectedRows.begin(), selectedRows.end());

        Item item;
        for (int i = selectedRows.count() - 1; i >= 0; --i) {
            item = Item(
                qtable->item(selectedRows.at(i).row(), 0)->text().toDouble(),
                qtable->item(selectedRows.at(i).row(), 1)->text(),
                qtable->item(selectedRows.at(i).row(), 2)->text().toDouble(),
                QDate::fromString(qtable->item(selectedRows.at(i).row(), 3)->text(), "dd.MM.yyyy"),
                QDate::fromString(qtable->item(selectedRows.at(i).row(), 4)->text(), "dd.MM.yyyy")
            );
            EditWindow *window = new EditWindow(EditWindow::WindowState::EDITING, table, table->get_item(table->find(&item)));
            window->setAttribute(Qt::WA_DeleteOnClose);
            window->show();
        }
    }
}

void MainWindow::on_action_remove_triggered() {
    if (ui->tab_widget->count()) {
        QTableWidget *qtable {get_qtable(ui->tab_widget->currentIndex())};
        Table *table {tables[find_table(qtable)]};

        QItemSelectionModel *selectionModel = qtable->selectionModel();
        QModelIndexList selectedRows = selectionModel->selectedRows();
        std::sort(selectedRows.begin(), selectedRows.end());

        Item item;
        for (int i = selectedRows.count() - 1; i >= 0; --i) {
            item = Item(
                qtable->item(selectedRows.at(i).row(), 0)->text().toDouble(),
                qtable->item(selectedRows.at(i).row(), 1)->text(),
                qtable->item(selectedRows.at(i).row(), 2)->text().toDouble(),
                QDate::fromString(qtable->item(selectedRows.at(i).row(), 3)->text(), "dd.MM.yyyy"),
                QDate::fromString(qtable->item(selectedRows.at(i).row(), 4)->text(), "dd.MM.yyyy")
            );
            table->remove(table->find(&item));
            qtable->removeRow(selectedRows.at(i).row());
        }

        table->sync();
    }
}

Column MainWindow::get_column() {
    switch (ui->column_box->currentIndex()) {
        case (int) Column::BATCH:
            return Column::BATCH;

        case (int) Column::NAME:
            return Column::NAME;

        case (int) Column::COUNT:
            return Column::COUNT;

        case (int) Column::BATCH_DATE:
            return Column::BATCH_DATE;

        case (int) Column::SELF_LIFE_DATE:
            return Column::SELF_LIFE_DATE;

        default:
            return Column::UNKNOWN;
    }
}

void MainWindow::on_action_filter_triggered() {
    if (tables.size()) {
        QTableWidget *qtable {get_qtable(ui->tab_widget->currentIndex())};
        Table *table {tables[find_table(qtable)]};

        std::vector<Item*> filtered_items {table->filter(get_column(), ui->search_line->text())};
        table->sync(filtered_items);
    }
}


void MainWindow::on_action_show_chart_supply_rate_date_triggered() {
    QTableWidget *qtable {get_qtable(ui->tab_widget->currentIndex())};
    Table *table {tables[find_table(qtable)]};

    ChartWindow *window = new ChartWindow(table);
    window->setAttribute(Qt::WA_DeleteOnClose);
    window->show();
}

