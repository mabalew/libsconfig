#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>
#include <sqlite3.h>
#include "libsconfig.hpp"

using namespace std;
using namespace config;

Config::Config(string dbFile) {
  this->dbFile_ = dbFile;
  createDB();
}

void Config::createDB() {
  sqlite3 *db;
  char *error = 0;
  int rc;

  rc = sqlite3_open(this->dbFile_.c_str(), &db);
  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
  } else {
    fprintf(stderr, "Database opened successfully\n");
  }

  stringstream sql;
  sql << "CREATE TABLE config (application VARCHAR NOT NULL " <<
    "DEFAULT 'DEFAULT_APPLICATION', context VARCHAR NOT NULL DEFAULT " <<
    "'DEFAULT_CONTEXT', key VARCHAR, value VARCHAR NOT NULL DEFAULT ''," <<
    " temporary INTEGER NOT NULL DEFAULT 0, " <<
    "PRIMARY KEY(application, context, key));";
  rc = sqlite3_exec(db, sql.str().c_str(), 0, 0, &error);
  if (rc && rc != 1) {
    fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
  } else {
    fprintf(stderr, "Database created successfully\n");
  }
  sqlite3_close(db);
}

Config::~Config() {
  removeTemporaryEntries();
}

Config::configurationEntry Config::get(Config::configurationEntry *data) {
  string context, application;
  string context_col = "context = ", application_col = "application = ";
  configurationEntry result;

  sqlite3 *db;
  int rc;

  rc = sqlite3_open(this->dbFile_.c_str(), &db);
  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
  } else {
    fprintf(stderr, "Database opened successfully\n");
  }

  if (data->context.length() > 0) {
    context = "'" + data->context + "' AND ";
  } else {
    context = "'DEFAULT_CONTEXT' AND ";
  }
  if (data->application.length() > 0) {
    application = "'" + data->application + "' AND ";
  } else {
    application = "'DEFAULT_APPLICATION' AND ";
  }
  string sql = "SELECT application, context, value, temporary FROM config WHERE " +
    application_col + application +
    context_col + context +
    "key = '" + data->key + "';";

  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
    sqlite3_step(stmt);
    data->application = (char*)sqlite3_column_text(stmt, 0);
    data->context = (char*)sqlite3_column_text(stmt, 1);
    data->value = (char*)sqlite3_column_text(stmt, 2);
    data->temporary = (bool)sqlite3_column_int(stmt, 3);
  }

  sqlite3_finalize(stmt);
  sqlite3_close(db);

  return result;
}

vector<Config::configurationEntry> Config::getAll(string application, string context) {
  string _context, _application;
  string _context_col = "context = ", _application_col = "application = ";
  configurationEntry entry;
  vector<configurationEntry> result;

  sqlite3 *db;
  int rc;

  rc = sqlite3_open(this->dbFile_.c_str(), &db);
  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
  } else {
    fprintf(stderr, "Database opened successfully\n");
  }

  if (application.length() > 0) {
    _application = "'" + application + "' AND ";
  } else {
    _application = "'DEFAULT_APPLICATION' AND ";
  }

  if (context.length() > 0) {
    _context = "'" + context + "' ";
  } else {
    _context = "'DEFAULT_CONTEXT' ";
  }
  string sql = "SELECT application, context, value, temporary FROM config WHERE " +
    _application_col + _application +
    _context_col + _context;

  cout << sql << endl;

  sqlite3_stmt *stmt;

  if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0) == SQLITE_OK) {
    while (true) {
      int step;
      step = sqlite3_step(stmt);
      if (step == SQLITE_ROW) {
        entry.application = (char*)sqlite3_column_text(stmt, 0);
        entry.context = (char*)sqlite3_column_text(stmt, 1);
        entry.value = (char*)sqlite3_column_text(stmt, 2);
        entry.temporary = (bool)sqlite3_column_int(stmt, 3);
      } else if (step == SQLITE_DONE) {
        break;
      } else {
        fprintf(stderr, "Failed.\n");
        exit(1);
      }
      result.push_back(entry);
    }
  }

  sqlite3_finalize(stmt);
  sqlite3_close(db);

  return result;
}

int Config::update(Config::configurationEntry data) {
  string context, application;
  string context_col = "context = ", application_col = "application = ";
  configurationEntry result;

  sqlite3 *db;
  char *error = 0;
  int rc;

  rc = sqlite3_open(this->dbFile_.c_str(), &db);
  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
  } else {
    fprintf(stderr, "Database opened successfully\n");
  }

  if (data.context.length() > 0) {
    context = "'" + data.context + "' AND ";
  } else {
    context = "'DEFAULT_CONTEXT' AND ";
  }
  if (data.application.length() > 0) {
    application = "'" + data.application + "' AND ";
  } else {
    application = "'DEFAULT_APPLICATION' AND ";
  }
  string sql = "UPDATE config SET value='" + data.value + "' WHERE " +
    application_col + application +
    context_col + context +
    "key = '" + data.key + "';";

  rc = sqlite3_exec(db, sql.c_str(), 0, 0, &error);
  if (rc) {
    fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    return 2;
  } else {
    fprintf(stderr, "Data updated successfully\n");
  }
  sqlite3_close(db);

  return 0;
}


int Config::remove(configurationEntry data) {
  string context, application;
  string context_col = "context = ", application_col = "application = ";
  configurationEntry result;

  sqlite3 *db;
  char *error = 0;
  int rc;

  rc = sqlite3_open(this->dbFile_.c_str(), &db);
  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
  } else {
    fprintf(stderr, "Database opened successfully\n");
  }

  if (data.context.length() > 0) {
    context = "'" + data.context + "' AND ";
  } else {
    context = "'DEFAULT_CONTEXT' AND ";
  }
  if (data.application.length() > 0) {
    application = "'" + data.application + "' AND ";
  } else {
    application = "'DEFAULT_APPLICATION' AND ";
  }
  string sql = "DELETE FROM config WHERE " +
    application_col + application +
    context_col + context +
    "key = '" + data.key + "';";

  rc = sqlite3_exec(db, sql.c_str(), 0, 0, &error);
  if (rc) {
    fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    return 2;
  } else {
    fprintf(stderr, "Data deleted successfully\n");
  }
  sqlite3_close(db);

  return 0;
}

int Config::put(configurationEntry data) {
  int rc = 0;
  char *error = 0;
  string context, application, context_col, application_col;
  sqlite3 *db;
  rc = sqlite3_open(this->dbFile_.c_str(), &db);
  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return 1;
  } else {
    fprintf(stderr, "Database opened successfully\n");
  }

  if (data.context.length() > 0) {
    context = "'" + data.context + "', ";
    context_col = "context, ";
  }
  if (data.application.length() > 0) {
    application = "'" + data.application + "',";
    application_col = "application, ";
  }
  stringstream sql;
  sql << "INSERT INTO config(" <<
    (context_col.size() > 0 ? context_col : "") <<
    (application_col.size() > 0 ? application_col : "") <<
    " key, value, temporary) VALUES(" <<
    (context_col.size() > 0 ? context : "") <<
    (application_col.size() > 0 ? application: "") <<
    "'" << data.key << "', " <<
    "'" << data.value << "', " <<
    data.temporary << ");";
  cout << sql.str() << endl;
  rc = sqlite3_exec(db, sql.str().c_str(), 0, 0, &error);
  if (rc) {
    fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
    return 2;
  } else {
    fprintf(stderr, "Data inserted successfully\n");
  }
  sqlite3_close(db);
  return 0;
}

void Config::removeTemporaryEntries() {
  sqlite3 *db;
  char *error = 0;
  int rc;

  rc = sqlite3_open(this->dbFile_.c_str(), &db);
  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
  } else {
    fprintf(stderr, "Database opened successfully\n");
  }

  string sql = "DELETE FROM config WHERE temporary = 1";

  rc = sqlite3_exec(db, sql.c_str(), 0, 0, &error);
  if (rc) {
    fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
  } else {
    fprintf(stderr, "Data deleted successfully\n");
  }
  sqlite3_close(db);

}

string Config::getDbFile() {
  return this->dbFile_;
}
