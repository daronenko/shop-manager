#include "tablewidget.h"
#include "ui_tablewidget.h"

TableWidget::TableWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TableWidget) {
    ui->setupUi(this);
    ui->table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

TableWidget::~TableWidget() {
    delete ui;
}
