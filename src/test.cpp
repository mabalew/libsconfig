#include <iostream>
#include <stdio.h>
#include "libsconfig.hpp"
#include <string>

using namespace std;
using namespace config;

Config::configurationEntry test_data_with_context() {
  Config::configurationEntry result;
  result.context = "CONTEXT_1";
  result.key = "KEY_1";
  result.value = "VALUE_1";

  return result;
}

Config::configurationEntry test_data_with_application() {
  Config::configurationEntry result;
  result.application = "APP_1";
  result.key = "KEY_2";
  result.value = "VALUE_2";

  return result;
}

Config::configurationEntry test_data_without_context_and_application() {
  Config::configurationEntry result;
  result.key = "KEY_1";
  result.value = "VALUE_1";

  return result;
}

Config::configurationEntry test_data_temporary_entry() {
  Config::configurationEntry result;
  result.application = "APP_2";
  result.context = "CONTEXT_4";
  result.key = "TMP_KEY_1";
  result.value = "TMP_VALUE_1";
  result.temporary = true;

  return result;
}

Config::configurationEntry test_data_full() {
  Config::configurationEntry result;
  result.application = "APP_2";
  result.context = "CONTEXT_4";
  result.key = "KEY_3";
  result.value = "VALUE_3";

  return result;
}

void print(Config::configurationEntry *entry) {
  cout << "=======================" << endl;
  cout << "Application: " << entry->application << endl;
  cout << "Context: " << entry->context << endl;
  cout << "Key: " << entry->key << endl;
  cout << "Value: " << entry->value << endl;
  cout << "Is temporary: " << entry->temporary << endl << endl;
}

void test() {
  Config::configurationEntry data_1;
  Config::configurationEntry data_2;
  Config::configurationEntry data_3;
  Config::configurationEntry data_4;
  Config::configurationEntry data_5;

  data_1 = test_data_with_context();
  data_2 = test_data_with_application();
  data_3 = test_data_without_context_and_application();
  data_4 = test_data_full();
  data_5 = test_data_temporary_entry();

  Config config = Config("config.db");
  config.put(data_1);
  config.put(data_2);
  config.put(data_3);
  config.put(data_4);
  config.put(data_5);

  data_1.value = "";
  data_2.value = "";
  data_3.value = "";
  data_4.value = "";

  config.get(&data_1);
  config.get(&data_2);
  config.get(&data_3);
  config.get(&data_4);
  config.get(&data_5);

  print(&data_1);
  print(&data_2);
  print(&data_3);
  print(&data_4);
  print(&data_5);

  cout << "before update: " << endl;
  print(&data_2);
  data_2.value = "VAL_100";
  config.update(data_2);
  cout << "after update: " << endl;
  print(&data_2);

  config.remove(data_1);
  config.remove(data_2);
  config.remove(data_3);
  config.remove(data_4);

  config.get(&data_5);
  cout << "temporary entry still exists?" << endl;
  print(&data_5);
  cout << "check database and press any key" << endl;
  cin.get();
}

int main(int argc, char **argv) {
  test();
  return 0;
}


