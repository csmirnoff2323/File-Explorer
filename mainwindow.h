#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

#include <QListView>
#include <QPushButton>
#include <QMessageBox>
#include <QGridLayout>
#include <QStringListModel>
#include <QStringList>
#include <QTableView>
#include <QStandardItemModel>
#include <QVBoxLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void LoadFromDir(QTableView * qTableView,
                     std::string Path);

private:
    Ui::MainWindow *ui;

    QPushButton * button1;
    QListView * listview1;
    //QGridLayout * myLayout;
    QVBoxLayout * myLayout;
    QStringListModel * Model;
    QTableView *qTableView;
    QTableView *qTableView2;
    QStringList List;
    std::string currentPath;
    std::string currentPath2;

    QVBoxLayout * rightLayout;
    QGridLayout * mainLayout;

private slots:
    void DblClick();
    void DblClick2();
};

#endif // MAINWINDOW_H
