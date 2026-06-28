## TodoList sqlite version

## 功能介绍
- 这是一个使用 Qt 和 SQLite 开发的待办事项管理工具。
- 支持：添加、删除、清空待办事项。
- 数据持久化：所有待办事项保存在本地 `todos.db` 数据库中，关闭软件后数据不丢失。

## 如何运行
- 环境：需要 Qt 5.x 或 Qt 6.x 以及 MinGW 编译器。
- 步骤：用 Qt Creator 打开 `Todolist_sql_ver.pro` 文件，编译运行即可。

## 技术点
- 使用 `QListWidget` 展示列表。
- 使用 `QSqlDatabase` 和 `QSqlQuery` 操作 SQLite 数据库。
- 使用 `QVBoxLayout` 和 `QHBoxLayout` 进行界面布局。
