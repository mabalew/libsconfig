libsconfig - lib(SQL)config
==========

Library to manage configuration of your application. Stores data in SQLite database. Fast and simple to use.
libsconfig supports temporary values as well. They are deleted after your program finish.

Simple example:

```C++
#include <iostream>
#include <stdio.h>
#include <string>
#include "libsconfig.hpp"

using namespace config;

int main(int argc, char **argv) {
  Config::configurationEntry entry;
  entry.application = "APP";
  entry.context = "CONTEXT";
  entry.key = "KEY_1";
  entry.value = "VALUE_1";
  entry.temporary = false;

  Config config = Config("config.db");

  // Add new configuration entry
  config.put(entry);

  // Clean value field
  entry.value = "";

  // and get it again from storage
  config.get(&entry);

  cout << "Value after get: " << entry.value << endl;

  // change value
  entry.value = "VALUE_2";
  config.update(entry);
  cout << "Value after update: " << entry.value << endl;

  // finally, remove the entry
  config.remove(entry);
}
```
