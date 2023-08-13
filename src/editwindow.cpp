#include "editwindow.h"
#include "ui_editwindow.h"


EditWindow::EditWindow(WindowState State, Table *table, Item *item, QWidget *parent):
    QWidget(parent),
    State(State),
    item(item),
    table(table),
    ui(new Ui::EditWindow)
{
    ui->setupUi(this);
    if (State == WindowState::EDITING)
        load_item();
    else
        throw;
}

EditWindow::EditWindow(WindowState State, Table *table, QWidget *parent):
    QWidget(parent),
    State(State),
    table(table),
    ui(new Ui::EditWindow)
{
    ui->setupUi(this);
    if (State == WindowState::APPENDING)
        clear();
    else
        throw;
}

EditWindow::~EditWindow() {
    delete ui;
}

void EditWindow::clear() {
    ui->batch_edit->clear();
    ui->name_edit->clear();
    ui->count_edit->clear();
    ui->batch_date_edit->setDate(QDate::currentDate());
    ui->shelf_life_date_edit->setDate(QDate::currentDate());
}

void EditWindow::load_item() {
    ui->batch_edit->setText(QString::number(item->batch));
    ui->name_edit->setText(item->name);
    ui->count_edit->setText(QString::number(item->count));
    ui->batch_date_edit->setDate(item->batch_date);
    ui->shelf_life_date_edit->setDate(item->shelf_life_date);
}

void EditWindow::save_changes() {
    *item = *build_item();
}

Item* EditWindow::build_item() {
    return new Item(
        ui->batch_edit->text().toInt(),
        ui->name_edit->text(),
        ui->count_edit->text().toInt(),
        ui->batch_date_edit->date(),
        ui->shelf_life_date_edit->date()
    );
}

void EditWindow::append_item(Item *item) {
    table->add(item);
}

bool EditWindow::is_valid() {
    if (
        ui->batch_edit->text() == ""
        || ui->name_edit->text() == ""
        || ui->count_edit->text() == ""
    )
        return false;

    if (ui->batch_edit->text()[0] == '-' || ui->count_edit->text()[0] == '-')
        return false;

    bool ok;
    if (ui->batch_edit->text().toInt(&ok); !ok)
        return false;

    if (ui->name_edit->text()[0].isDigit())
        return false;

    if (ui->batch_date_edit->date() > ui->shelf_life_date_edit->date())
        return false;

    return true;
}

void EditWindow::on_apply_button_clicked() {
    if (is_valid()) {
        switch (State) {
            case (WindowState::APPENDING):
                append_item(build_item());
                break;

            case (WindowState::EDITING):
                save_changes();
                break;
        }
        table->sync();
        close();
    } else {
        QMessageBox::warning(
            this,
            "Недопустимые данные", QString("Вы ввели недопустимые данные!"),
            QMessageBox::Ok
        );
    }
}

