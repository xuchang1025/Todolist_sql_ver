#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // ---------- 1. 创建控件 ----------
    m_listWidget = new QListWidget(this);
    m_lineEdit = new QLineEdit(this);
    m_lineEdit->setPlaceholderText("输入待办事项...");

    m_addBtn = new QPushButton("添加", this);
    m_deleteBtn = new QPushButton("删除选中", this);
    m_clearBtn = new QPushButton("清空全部", this);

    // ---------- 2. 布局 ----------
    QHBoxLayout *inputLayout = new QHBoxLayout;
    inputLayout->addWidget(m_lineEdit);
    inputLayout->addWidget(m_addBtn);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addWidget(m_deleteBtn);
    btnLayout->addWidget(m_clearBtn);
    btnLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(m_listWidget);
    mainLayout->addLayout(inputLayout);
    mainLayout->addLayout(btnLayout);

    QWidget *central = new QWidget(this);
    central->setLayout(mainLayout);
    setCentralWidget(central);

    // ---------- 3. 信号槽 ----------
    connect(m_addBtn, &QPushButton::clicked, this, &MainWindow::onAddItem);
    connect(m_deleteBtn, &QPushButton::clicked, this, &MainWindow::onDeleteItem);
    connect(m_clearBtn, &QPushButton::clicked, this, &MainWindow::onClearAll);
    connect(m_lineEdit, &QLineEdit::returnPressed, this, &MainWindow::onAddItem);

    // ---------- 4. 数据库 ----------
    initDatabase();
    loadTodos();

    // ---------- 5. 窗口设置 ----------
    resize(400, 350);
    setWindowTitle("待办事项 - SQLite版");
}

MainWindow::~MainWindow() {}

// ========== 数据库初始化 ==========

void MainWindow::initDatabase()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("todos.db");

    if (!m_db.open()) {
        QMessageBox::critical(this, "数据库错误",
                              "无法打开数据库：" + m_db.lastError().text());
        return;
    }

    QSqlQuery query;
    QString createTableSQL = R"(
        CREATE TABLE IF NOT EXISTS todos (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            text TEXT
        )
    )";

    if (!query.exec(createTableSQL)) {
        QMessageBox::critical(this, "数据库错误",
                              "创建表失败：" + query.lastError().text());
    }
}

// ========== 加载所有待办 ==========

void MainWindow::loadTodos()
{
    QSqlQuery query;
    if (!query.exec("SELECT id, text FROM todos ORDER BY id")) {
        QMessageBox::warning(this, "数据库错误",
                             "加载数据失败：" + query.lastError().text());
        return;
    }

    m_listWidget->clear();

    while (query.next()) {
        int id = query.value(0).toInt();
        QString text = query.value(1).toString();

        QListWidgetItem *item = new QListWidgetItem(text);
        item->setData(Qt::UserRole, id);  // 把 id 存到 item 里
        m_listWidget->addItem(item);
    }
}

// ========== 添加待办 ==========

void MainWindow::onAddItem()
{
    QString text = m_lineEdit->text().trimmed();
    if (text.isEmpty()) {
        QMessageBox::warning(this, "提示", "请输入内容！");
        return;
    }

    addTodoToDB(text);

    m_lineEdit->clear();
    m_lineEdit->setFocus();

    loadTodos();  // 刷新列表
}

void MainWindow::addTodoToDB(const QString &text)
{
    QSqlQuery query;
    query.prepare("INSERT INTO todos (text) VALUES (?)");
    query.addBindValue(text);

    if (!query.exec()) {
        QMessageBox::warning(this, "数据库错误",
                             "添加失败：" + query.lastError().text());
    }
}

// ========== 删除待办 ==========

void MainWindow::onDeleteItem()
{
    QListWidgetItem *item = m_listWidget->currentItem();
    if (!item) {
        QMessageBox::warning(this, "提示", "请先选中一项！");
        return;
    }

    int id = item->data(Qt::UserRole).toInt();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认删除",
                                  "确定删除 \"" + item->text() + "\" 吗？",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        deleteTodoFromDB(id);
        loadTodos();  // 刷新列表
    }
}

void MainWindow::deleteTodoFromDB(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM todos WHERE id = ?");
    query.addBindValue(id);

    if (!query.exec()) {
        QMessageBox::warning(this, "数据库错误",
                             "删除失败：" + query.lastError().text());
    }
}

// ========== 清空全部 ==========

void MainWindow::onClearAll()
{
    if (m_listWidget->count() == 0) {
        QMessageBox::information(this, "提示", "列表已经是空的");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "确认清空",
                                  "确定删除所有待办事项吗？",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        clearTodosFromDB();
        loadTodos();  // 刷新列表
    }
}

void MainWindow::clearTodosFromDB()
{
    QSqlQuery query;
    if (!query.exec("DELETE FROM todos")) {
        QMessageBox::warning(this, "数据库错误",
                             "清空失败：" + query.lastError().text());
    }
}