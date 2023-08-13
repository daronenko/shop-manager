#ifndef TABLEWIDGET_H
#define TABLEWIDGET_H

#include <QWidget>

namespace Ui {
class TableWidget;
}

class TableWidget : public QWidget {
    Q_OBJECT

public:
    explicit TableWidget(QWidget *parent = nullptr);
    ~TableWidget();

public:
    Ui::TableWidget *ui;
};

#endif // TABLEWIDGET_H
