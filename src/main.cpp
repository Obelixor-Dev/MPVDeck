#include <QApplication>
#include <QMainWindow>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include "viewmodels/SettingsViewModel.h"
#include <iostream>

// Custom message handler to redirect Qt logs to the console
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QByteArray localMsg = msg.toLocal8Bit();
    const char *file = context.file ? context.file : "unknown";
    const char *function = context.function ? context.function : "unknown";
    switch (type) {
        case QtDebugMsg:
            fprintf(stdout, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
            break;
        case QtInfoMsg:
            fprintf(stdout, "Info: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
            break;
        case QtWarningMsg:
            fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
            break;
        case QtCriticalMsg:
            fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
            break;
        case QtFatalMsg:
            fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
            abort();
    }
}

int main(int argc, char *argv[]) {
    qInstallMessageHandler(messageHandler);
    QApplication app(argc, argv);

    QMainWindow mainWindow;
    mainWindow.setWindowTitle("MPVDeck");

    // 1. Create ViewModel and load data
    SettingsViewModel viewModel;
    viewModel.loadSettings();

    // 2. Create a data model for the view
    auto *model = new QStandardItemModel(0, 2, &mainWindow);
    model->setHorizontalHeaderLabels({"Setting", "Value"});

    for (auto it = viewModel.settings().constBegin(); it != viewModel.settings().constEnd(); ++it) {
        QList<QStandardItem *> rowItems;
        rowItems.append(new QStandardItem(it.key()));
        rowItems.append(new QStandardItem(it.value()));
        model->appendRow(rowItems);
    }

    // 3. Create the View
    auto *tableView = new QTableView(&mainWindow);
    tableView->setModel(model);
    tableView->horizontalHeader()->setStretchLastSection(true);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers); // Read-only for now
    tableView->setSortingEnabled(true);
    tableView->sortByColumn(0, Qt::AscendingOrder);

    mainWindow.setCentralWidget(tableView);
    mainWindow.resize(800, 600);
    mainWindow.show();

    return app.exec();
}
