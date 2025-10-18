#include <QApplication>
#include <QMainWindow>
#include <QTableView>
#include <QStandardItemModel>
#include <QHeaderView>
#include <QPushButton>
#include <QVBoxLayout>
#include "viewmodels/SettingsViewModel.h"
#include "models/ConfigManager.h"
#include <iostream>
#include <QDebug>

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
    tableView->setSortingEnabled(true);
    tableView->sortByColumn(0, Qt::AscendingOrder);

    auto *saveButton = new QPushButton("Save", &mainWindow);

    QObject::connect(saveButton, &QPushButton::clicked, [model]() {
        qDebug() << "Save button clicked. Saving settings...";
        QMap<QString, QString> newSettings;
        for (int row = 0; row < model->rowCount(); ++row) {
            QString key = model->item(row, 0)->text();
            QString value = model->item(row, 1)->text();
            if (!key.isEmpty()) {
                newSettings[key] = value;
            }
        }

        ConfigManager configManager;
        configManager.saveConfigFile(newSettings);
        qDebug() << "Settings saved.";
    });

    auto *centralWidget = new QWidget(&mainWindow);
    auto *layout = new QVBoxLayout(centralWidget);
    layout->addWidget(tableView);
    layout->addWidget(saveButton);

    mainWindow.setCentralWidget(centralWidget);
    mainWindow.resize(800, 600);
    mainWindow.show();

    return app.exec();
}
