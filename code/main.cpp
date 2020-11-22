// $Id: main.cpp,v 1.12 2020-10-22 16:50:08-07 - - $

#include <cstdlib>
#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <regex>
#include <cassert>

using namespace std;

#include "listmap.h"
#include "xpair.h"
#include "util.h"

using str_str_map = listmap<string,string>;
using str_str_pair = str_str_map::value_type;
const string cin_name = "-";

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      int option = getopt (argc, argv, "@:");  
      if (option == EOF) break;
      switch (option) {
         case '@':
            debugflags::setflags (optarg);
            break;
         default:
            complain() << "-" << char (optopt) << ": invalid option"
                       << endl;
            break;
      }
   }
}

void print_prompt (int count) {
   cout << "-: " << count << ": ";
}

int main (int argc, char** argv) {
   sys_info::execname (argv[0]);
   int status = 0;
   scan_options (argc, argv);

   str_str_map list;
   regex comment_regex {R"(^\s*(#.*)?$)"};
   regex key_value_regex {R"(^\s*(.*?)\s*=\s*(.*?)\s*$)"};
   regex trimmed_regex {R"(^\s*([^=]+?)\s*$)"};
   int count = 1;

   bool isCin = false;
   if (argc < 2) isCin = true;

   for (int it = 1 - isCin; it < argc; ++it) {
      string filename = argv[it];
      ifstream infile (filename) ;
      if (filename == cin_name) isCin = true;
      // getline (filestr)
      for(;;) {
         string line;
         if (!isCin && infile.fail()) {
            status = 1;
            cerr << "keyvalue" << ": " << filename << ": "
                 << strerror (errno) << endl;
            break;
         }
         if (isCin) {
            getline(cin, line);
            if (cin.eof()) break;
         }
         else {
            getline(infile, line);
            if (infile.eof()) {
               infile.close();
               break;
            }
         }
         smatch result;
         if (regex_search (line, result, comment_regex)) {
            print_prompt(count); cout << result[1] << endl;
            continue;
         }
         if (regex_search (line, result, key_value_regex)) {
            if (result[2] == "") {
               if (result[1] == "") {
                  print_prompt(count); cout << "=" << endl;
                  for(auto itr = list.begin(); itr != list.end(); ++itr) {
                     list.print(itr);
                  }
               }
               else
               {
                  print_prompt(count);
                  cout << result[1] << " =" << endl;
                  auto iterator = list.find(result[1]);
                  if(iterator != list.end()) {
                     list.erase(iterator);
                  }
               }
            }
            else if (result[1] == "") {
               print_prompt(count); cout << "= " << result[2] << endl;
               for(auto itr = list.begin(); itr != list.end(); ++itr) {
                  if ((*itr).second == result[2]) {
                     list.print(itr);
                  }
               }
            }
            else {
               print_prompt(count);
               str_str_pair newpair {result[1], result[2]};
               auto position = list.insert({result[1], result[2]});
               list.print(position);
               list.print(position);
            }
         } else if (regex_search (line, result, trimmed_regex)) {
            list.print(list.find(result[1]));
         } else {
            assert (false and "This can not happen.");
         }

         count++;
      }
      isCin = false;
   }

   if (status == 0) cout << "EXIT_SUCCESS" << endl;
   else cout << "EXIT_FAILURE" << endl;
   return status;
}

