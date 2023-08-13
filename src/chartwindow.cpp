#include "chartwindow.h"
#include "ui_chartwindow.h"


ChartWindow::ChartWindow(Table *table, QWidget *parent):
    QWidget(parent),
    table(table),
    ui(new Ui::ChartWindow)
{
    ui->setupUi(this);

    chart = new QChart();
    chart_view = new QChartView(chart);
    ui->vertical_layout->addWidget(chart_view);
    axis = new QBarCategoryAxis();
    axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);

    setup_chart();
    load_goods();
    draw_chart();
}

ChartWindow::~ChartWindow() {
    delete ui;
}

void ChartWindow::load_goods() {
    QSet<QString> goods;
    for (size_t i {}; i < table->size(); ++i)
        goods.insert((*table)[i]->name);

    for (const auto &good: goods)
        ui->goods_box->addItem(good);
}

QMap<QDate, int>* ChartWindow::get_supply_count(QString good) {
    QMap<QDate, int> *supply_count {new QMap<QDate, int>};
    for (size_t i {}; i < table->size(); ++i) {
        if ((*table)[i]->name == good) {
            if (supply_count->contains((*table)[i]->batch_date))
                (*supply_count)[(*table)[i]->batch_date] += (*table)[i]->count;
            else
                supply_count->insert((*table)[i]->batch_date, (*table)[i]->count);
        }
    }

    return supply_count;
}

void ChartWindow::setup_chart() {
    chart->setTitle("Объем поставок / Дата");
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    chart_view->setRenderHint(QPainter::Antialiasing);
}

void ChartWindow::draw_chart() {
    chart_view->chart()->removeAllSeries();

    QMap<QDate, int> *supply_count {get_supply_count(ui->goods_box->currentText())};

    QBarSeries *series {new QBarSeries()};
    QStringList categories;
    QBarSet *bar_set {new QBarSet(ui->goods_box->currentText())};

    QMap<QDate, int>::iterator iter;
    for (iter = supply_count->begin(); iter != supply_count->end(); ++iter) {
        *bar_set << iter.value();
        categories << iter.key().toString("dd.MM.yyyy");
    }
    series->append(bar_set);
    chart->addSeries(series);

    axis->clear();
    axis->append(categories);
    chart->setAxisX(axis, series);

    QList<int> nums {supply_count->values()};
    axisY->setRange(0, *std::max_element(nums.begin(), nums.end()));
}


void ChartWindow::on_action_update_chart_triggered() {
    draw_chart();
}

