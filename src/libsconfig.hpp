#ifndef LIBCONFIG_H
#define LIBCONFIG_H

using namespace std;

namespace config {
  /**
   * Represents configuration unit.
   */
  class Config {
    private:
      /**
       * SQLite storage file
       */
      string dbFile_;

      /**
       * Create database if it does not exists.
       */
      void createDB();
      void removeTemporaryEntries();
    public:
      /**
       * Constructor of the class
       * \param path to the SQLite database file. Database will be created if
       * does not exists.
       */
      explicit Config(string dbFile);
      ~Config();

      /**
       * Structure to hold configuration entry.
       */
      struct configurationEntry {
        string context;
        string application;
        string key;
        string value;
        bool temporary;
      };

      int put(configurationEntry entry);
      int remove(configurationEntry entry);
      int update(configurationEntry entry);
      configurationEntry get(configurationEntry *entry);

      string getDbFile();
  }; // class Config
} // namespace config 
#endif // LIBCONFIG_H

