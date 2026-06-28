#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QMessageBox>

QT_BEGIN_NAMESPACE

class MainWindow;

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onAddItem();
    void onDeleteItem();
    void onClearAll();

private:
    void initDatabase();
    void loadTodos();
    void addTodoToDB(const QString &text);
    void deleteTodoFromDB(int id);
    void clearTodosFromDB();

    QListWidget *m_listWidget;
    QLineEdit *m_lineEdit;
    QPushButton *m_addBtn;
    QPushButton *m_deleteBtn;
    QPushButton *m_clearBtn;

    QSqlDatabase m_db;

};
#endif // MAINWINDOW_H
