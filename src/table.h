#pragma once

#include <QDate>
#include <QFile>
#include <QFileDialog>
#include <QTableWidget>
#include <QHeaderView>

#include <vector>
#include <string>
#include <unordered_map>
#include <type_traits>


enum class Column {
    BATCH,
    NAME,
    COUNT,
    BATCH_DATE,
    SELF_LIFE_DATE,
    UNKNOWN
};


struct Item {
    unsigned batch;
    QString name;
    unsigned count;
    QDate batch_date;
    QDate shelf_life_date;

    Item() {}
    Item(unsigned batch, QString name, unsigned count, QDate batch_date, QDate shelf_life_date)
        : batch(batch), name(name), count(count), batch_date(batch_date), shelf_life_date(shelf_life_date) {}

    bool operator==(const Item &item);
};

Item *row_to_item();

void print(const Item *item);
void print(std::vector<Item*> items);


class Table {
  protected:
    std::vector<Item*> items;

  public:
    QFile *file;
    QTableWidget *table;

    bool is_frozen {false};
    bool is_saved {false};

    Table(QTableWidget *table): file(new QFile()), table(table) {}
    Table(QTableWidget *table, QString filename): file(new QFile()), table(table) {
        file->setFileName(filename);
        load();
    }

    Item* get_item(size_t index);
    void add(Item *item);
    void remove(Item *item);
    void remove(unsigned index);
    void update(Item *curr_item, Item *new_item);
    unsigned find(Item *item);

    void save();
    void save(QString filename);

    void load();

    void sync();
    void sync(std::vector<Item*> items);

    void print();
    size_t size();

    Item* operator[](int index);

    std::vector<Item*> filter(Column col, QString value);
};
