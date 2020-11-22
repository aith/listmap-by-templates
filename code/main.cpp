// $Id: main.cpp,v 1.12 2020-10-22 16:50:08-07 - - $

#include <cstdlib>
#include <exception>
#include <iostream>
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

void scan_options (int argc, char** argv) {
   opterr = 0;
   for (;;) {
      // getopt() takes a string of possible flags. ":" indicates
      //   that char must be followed by another char
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
   scan_options (argc, argv);

   str_str_map list;
   // // starting at the optind, while argp(ointer) is not at end
   // for (char** argp = &argv[optind]; argp != &argv[argc]; ++argp) {
   //    // create pair with the arg, and the point in the range?
   //    str_str_pair pair (*argp, to_string<int> (argp - argv)); 
   //    cout << "Before insert: " << pair << endl;
   //    test.insert (pair);
   // }
   regex comment_regex {R"(^\s*(#.*)?$)"};
   regex key_value_regex {R"(^\s*(.*?)\s*=\s*(.*?)\s*$)"};
   regex trimmed_regex {R"(^\s*([^=]+?)\s*$)"};
   int count = 1;

   for(;;) {
      string line;
      getline(cin, line);
      if (cin.eof()) break;
      smatch result;
      // create one listmap

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
               print_prompt(count); cout << result[1] << " =" << endl;
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
            auto position = list.insert(newpair);
            list.print(position);
            list.print(position);
         }
      } else if (regex_search (line, result, trimmed_regex)) {
         // Print individual keys
         list.print(list.find(result[1]));
      } else {
         assert (false and "This can not happen.");
      }

      count++;
   }

   cout << list.empty() << endl;
   for (str_str_map::iterator itor = list.begin();
        itor != list.end(); ++itor) {
      // do something for every node in str_str_map
      cout << "During iteration: " << *itor << endl; 
   }

   // str_str_map::iterator itor = list.begin();
   // list.erase (itor);

   cout << "EXIT_SUCCESS" << endl;
   return EXIT_SUCCESS;
}

