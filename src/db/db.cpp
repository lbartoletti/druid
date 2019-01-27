#include <iostream>

#include "db.h"

DruidDb::DruidDb(const QString &filename) : _table("recipes") {
  open(filename);
}

DruidDb::~DruidDb() { close(); }

bool DruidDb::open(const QString &filename) {
  if (_db) {
    close();
  }

  return sqlite3_open(filename.toStdString().c_str(), &_db);
}

bool DruidDb::init() const {
  const QString sql =
      QString("CREATE TABLE %1(title TEXT, notes TEXT);").arg(_table);
  exec(sql);
  return is_ready();
}

DruidRecipe DruidDb::recipe(const QString &title) const {
  DruidRecipe recipe;

  const QString sql = QString("SELECT title, notes FROM %1 where title = '%2'")
                          .arg(_table, title);

  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(_db, sql.toStdString().c_str(), -1, &stmt, NULL) !=
      SQLITE_OK) {
    std::cerr << "ERROR: " << sqlite3_errmsg(_db) << std::endl;
    sqlite3_finalize(stmt);
    return recipe;
  }

  int ret_code = 0;
  int column = 0;
  while ((ret_code = sqlite3_step(stmt)) == SQLITE_ROW) {
    // title
    if (column == 0) {
      const QString title((char *)sqlite3_column_text(stmt, 0));
      recipe.setTitle(title);
    }

    // notes
    if (column == 1) {
      const QString notes((char *)sqlite3_column_text(stmt, 0));
      recipe.setNotes(notes);
    }
  }

  if (ret_code != SQLITE_DONE) {
    std::cerr << "ERROR: " << sqlite3_errmsg(_db) << std::endl;
  }

  sqlite3_finalize(stmt);

  return recipe;
}

bool DruidDb::recipe_exists(const QString &title) const {
  bool exist = false;

  const QString sql =
      QString("SELECT title FROM %1 where title = '%2'").arg(_table, title);

  sqlite3_stmt *stmt;
  if (sqlite3_prepare_v2(_db, sql.toStdString().c_str(), -1, &stmt, NULL) !=
      SQLITE_OK) {
    std::cerr << "ERROR: " << sqlite3_errmsg(_db) << std::endl;
    sqlite3_finalize(stmt);
    return exist;
  }

  int ret_code = 0;
  int column = 0;
  std::cout << sql.toStdString() << std::endl;
  while ((ret_code = sqlite3_step(stmt)) == SQLITE_ROW) {
    // title
    if (column == 0) {
      exist = true;
    }
  }

  std::cout << exist << std::endl;

  if (ret_code != SQLITE_DONE) {
    std::cerr << "ERROR: " << sqlite3_errmsg(_db) << std::endl;
  }

  sqlite3_finalize(stmt);

  return exist;
}

bool DruidDb::recipe_add(const DruidRecipe &recipe) const {
  const QString sql = QString("INSERT INTO %1 VALUES('%2', '%3')")
                          .arg(_table, recipe.title(), recipe.notes());
  exec(sql);
}

bool DruidDb::recipe_update(const DruidRecipe &recipe) const {
  const QString sql = QString("UPDATE %1 SET notes='%2' WHERE title='%3'")
                          .arg(_table, recipe.notes(), recipe.title());
  exec(sql);
}

bool DruidDb::is_ready() const {
  bool ready = false;

  if (_db) {
    const QString sql = QString(
                            "SELECT name FROM sqlite_master WHERE "
                            "type='table' AND name='%1'")
                            .arg(_table);

    sqlite3_stmt *stmt;
    if (sqlite3_prepare_v2(_db, sql.toStdString().c_str(), -1, &stmt, NULL) !=
        SQLITE_OK) {
      std::cerr << "ERROR: " << sqlite3_errmsg(_db) << std::endl;
      sqlite3_finalize(stmt);
      return false;
    }

    int ret_code = 0;
    while ((ret_code = sqlite3_step(stmt)) == SQLITE_ROW) {
      const QString result((char *)sqlite3_column_text(stmt, 0));
      if (result.compare(_table) == 0) {
        ready = true;
      }
    }

    if (ret_code != SQLITE_DONE) {
      std::cerr << "ERROR: " << sqlite3_errmsg(_db) << std::endl;
    }

    sqlite3_finalize(stmt);
  }

  return ready;
}

bool DruidDb::close() {
  if (_db) {
    sqlite3_close(_db);
    _db = nullptr;
  }
}

bool DruidDb::exec(const QString &sql) const {
  return sqlite3_exec(_db, sql.toStdString().c_str(), 0, 0, 0);
}
