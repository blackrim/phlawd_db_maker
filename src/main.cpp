/*
 * Copyright (C) 2017 blackrim
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

/* 
 * File:   main.cpp
 * Author: blackrim
 *
 * Created on March 27, 2017, 8:58 PM
 */

#include <iostream>
#include <cstdio>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

#include "utils.h"
#include "SQLiteDBController.h"

int main(int argc, char* argv[]){
    if(argc != 3){
	cout << "PHLAWD DB MAKER 0.1" << endl;
	cout << "you need more arguments." << endl;
	cout << "usage: PHLAWD division dbloc" << endl;
    }else{

	string division(argv[1]);
        string dbname(argv[2]);

        cout << "setting up database" << endl;
        bool download = true;

        SQLiteDBController * c;
        c = new SQLiteDBController(dbname);
        bool ret = c->initiate();
        if (ret == false){
            cout << "database exists: exiting" << endl;
            exit(0);
        }
        if(division.size () > 1){
            c->load_seqs(division,download);
        }else{
            cout << "you need to add a division=<division,like pln> to "<< argv[2] <<endl;
            exit(0);
        }
        //need to add updating

        delete c;
	
    }
}