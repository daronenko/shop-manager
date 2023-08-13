#ifndef CHARTWINDOW_H
#define CHARTWINDOW_H

#include "table.h"

#include <QWidget>
#include <QSet>

#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QHorizontalStackedBarSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>


namespace Ui {
class ChartWindow;
}

class ChartWindow : public QWidget {
    Q_OBJECT

public:
    Table *table;

    QChartView *chart_view;
    QChart *chart;
    QBarCategoryAxis *axis;
    QValueAxis *axisY;

    explicit ChartWindow(Table *table, QWidget *parent = nullptr);
    ~ChartWindow();

    void load_goods();
    void setup_chart();
    void draw_chart();
    QMap<QDate, int>* get_supply_count(QString good);

private slots:
    void on_action_update_chart_triggered();

private:
    Ui::ChartWindow *ui;
};

#endif // CHARTWINDOW_H
