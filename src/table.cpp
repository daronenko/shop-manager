#include "table.h"


Item* Table::get_item(size_t index) {
    return items[index];
}

void Table::add(Item *item) {
    items.push_back(item);
    is_saved = false;
}

void Table::remove(Item *item) {
    items.erase(items.begin() + find(item));
    is_saved = false;
}

void Table::remove(unsigned index) {
    items.erase(items.begin() + index);
    is_saved = false;
}

void Table::update(Item *curr_item, Item *new_item) {
    curr_item->batch = new_item->batch;
    curr_item->name = new_item->name;
    curr_item->count = new_item->count;
    curr_item->batch_date = new_item->batch_date;
    curr_item->shelf_life_date = new_item->shelf_life_date;
    is_saved = false;
}

unsigned Table::find(Item *item) {
    for (unsigned i {}; i < items.size(); ++i) {
        if (*items[i] == *item)
            return i;
    }

    return items.size();
}

void Table::sync() {
    sync(items);
}

size_t Table::size() {
    return items.size();
}

void Table::sync(std::vector<Item*> source) {
    table->clear();
    QStringList headers;
    headers << "Номер партии" << "Наименование" << "Количество" << "Дата поставки" << "Срок годности";
    table->setColumnCount(headers.size());
    table->setHorizontalHeaderLabels(headers);
    table->verticalHeader()->setVisible(true);

    table->setRowCount(source.size());
    for (int row {}; row < table->rowCount(); ++row) {
        table->setVerticalHeaderItem(row, new QTableWidgetItem(QString::number(row + 1)));

        table->setItem(row, 0, new QTableWidgetItem(QString::number(source[row]->batch)));
        table->setItem(row, 1, new QTableWidgetItem(source[row]->name));
        table->setItem(row, 2, new QTableWidgetItem(QString::number(source[row]->count)));
        table->setItem(row, 3, new QTableWidgetItem(source[row]->batch_date.toString("dd.MM.yyyy")));
        table->setItem(row, 4, new QTableWidgetItem(source[row]->shelf_life_date.toString("dd.MM.yyyy")));
    }
}

void Table::save() {
    if (!file->open(QIODevice::WriteOnly)) {
        throw "Failed to get the file!";
    }

    QDataStream out(file);
    for (const auto &item: items)
        out << item->batch << item->name << item->count << item->batch_date << item->shelf_life_date;
    file->close();

    is_saved = true;
}

void Table::save(QString filename) {
    QFile *file {new QFile(filename)};
    if (!file->open(QIODevice::WriteOnly)) {
        throw "Failed to get the file!";
    }

    QDataStream out(file);
    for (const auto &item: items)
        out << item->batch << item->name << item->count << item->batch_date << item->shelf_life_date;
    file->close();

    is_saved = true;
}

void Table::load() {
    if (!file->open(QIODevice::ReadOnly)) {
        throw "Failed to get the file!";
    }

    items.clear();
    Item *item;
    QDataStream in(file);
    while (!in.atEnd()) {
        item = new Item;
        in >> item->batch;
        in >> item->name;
        in >> item->count;
        in >> item->batch_date;
        in >> item->shelf_life_date;
        items.push_back(item);
    }
    file->close();

    is_saved = true;
}

void print(const Item *item) {
    qDebug() << "Item("
             << "\n\tbatch =" << item->batch
             << "\n\tname =" << item->name
             << "\n\tcount =" << item->count
             << "\n\tbatch_date =" << item->batch_date.toString("dd.MM.yyyy")
             << "\n\tshelf_life_date =" << item->shelf_life_date.toString("dd.MM.yyyy")
             << "\n)";
}

void print(const std::vector<Item*> &items) {
    for (const auto &item: items)
        print(item);
}

void Table::print() {
    for (const auto &item: items)
        ::print(item);
}

Item* Table::operator[](int index) {
    return items[index];
}

bool Item::operator==(const Item &item) {
    return this->batch == item.batch
        && this->name == item.name
        && this->count == item.count
        && this->batch_date == item.batch_date
        && this->shelf_life_date == item.shelf_life_date;
}

std::vector<Item*> Table::filter(Column col, QString value) {
    std::vector<Item*> result;
    bool is_matching;

    for (const auto &item: items) {
        is_matching = false;
        switch (col) {
          case Column::BATCH:
            is_matching = QString::number(item->batch) ==  value;
            break;

          case Column::NAME:
            is_matching = item->name == value;
            break;

          case Column::COUNT:
            is_matching = QString::number(item->count) == value;
            break;

          case Column::BATCH_DATE:
            is_matching = item->batch_date.toString("dd.MM.yyyy") == value;
            break;

          case Column::SELF_LIFE_DATE:
            is_matching = item->shelf_life_date.toString("dd.MM.yyyy") == value;
            break;
        }

        if (is_matching || value == "")
            result.push_back(item);
    }

    return result;
}
