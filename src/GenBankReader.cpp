/*
 * PHLAWD: Phylogeny assembly with databases
 * Copyright (C) 2012  Stephen A. Smith
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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */

#include <string>
#include <iostream>
#include <fstream>
#include <regex.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>

#include "utils.h"
#include "GenBankReader.h"
#include "libsqlitewrapped.h"
#include <sqlite3.h>

int upper(int c){
  return std::toupper((unsigned char)c);
}


GenBankReader::GenBankReader(){

}


void GenBankReader::parse_file(string fl, string db_name){
    ifstream file(fl.c_str());
    string ln;
    bool descr = false;
    bool seq = false;
    bool title = false;
    bool titledone = false;
    string descrst;
    string titlest;
    string seqst;
    string locus;
    string acc;
    string ver;
    string taxid;
    sqlite3 *conn;
    int rc = sqlite3_open(db_name.c_str(), &conn);
    char *zErrMsg = 0;
    
    sqlite3_exec(conn, "BEGIN TRANSACTION", NULL, NULL, NULL);
    while(getline(file,ln)){
    vector<string> tokens;
    string del(" ");
    Tokenize(ln,tokens,del);
    for(int j=0;j<tokens.size();j++){
        TrimSpaces(tokens[j]);
    }
    if(tokens.size() >= 1){
        if(tokens[0] == "LOCUS"){
            locus = tokens[1];
            continue;
        }
        if(tokens[0] == "VERSION"){
            ver = tokens[1];
            continue;
        }
        if(tokens[0].find("/db_xref=\"taxon:")!= string::npos){
            vector<string> t2;
            string del2(":");
            Tokenize(tokens[0],t2,del2);
            taxid = t2[1].substr(0,t2[1].size()-1);
            continue;
        }
        if(tokens[0] == "DEFINITION"){
            descr = true;
            string ln2 = ln;
            TrimSpaces(ln2);
            descrst += ln2.substr(12,ln2.size()-12);
            continue;
        }
        //keep reading the description
        if(descr == true){
            if(tokens[0] == "ACCESSION"){
                acc = tokens[1];
                descr = false;
                continue;
            }else{
                string ln2 = ln;
                TrimSpaces(ln2);
                descrst += " "+ln2;
                continue;
            }
        }
        if(tokens[0] == "TITLE" && titledone == false){
            title = true;
            titledone = true;
            string ln2 = ln;
            TrimSpaces(ln2);
            titlest += ln2.substr(10,ln2.size()-10);
            continue;
        }
        if(title == true){
            if (tokens[0] == "JOURNAL"){
                title = false;
                continue;
            }else{
                string ln2 = ln;
                TrimSpaces(ln2);
                titlest += " "+ln2;
                continue;
            }
        }
        if(tokens[0] == "ORIGIN" && tokens.size() == 1){
            seq = true;
            continue;
        }
        //keep reading the sequence
        //sequence is always the last thing
        if(seq == true){
            if(tokens[0] == "//"){
                bool deposit = true;
                if(locus.size() == 0){
                    cout<<"locus" << endl;
                    deposit = false;
                }else if(taxid.size() ==0){
                    cout<<"taxid" << endl;
                    deposit = false;
                }else if(acc.size() == 0 || titlest.size() == 0 || ver.size() == 0){
                    deposit = false;
                    cout << "acc titlest ver" << endl;
                }else if(descrst.size() ==0){
                    cout<<"descr" << endl;
                    deposit = false;
                }else if(seqst.size() ==0){
                    cout<<"seqst" << endl;
                    deposit = false;
                }
                string sql = "insert into sequence (ncbi_id,locus,accession_id,version_id,description,title,seq) values (";
                sql += taxid+",'";
                sql += locus+"','";
                sql += acc+"','";
                sql += ver+"','";
                sql += descrst +"','";
                sql += titlest +"','";
                std::transform(seqst.begin(), seqst.end(), seqst.begin(), upper);
                sql += seqst+"');";
                size_t found = seqst.find_first_of("(),.[]@#$%!+=^&*\"'|-_/{}`~<>\\");
                if(found != string::npos){
                    cout << taxid << "," << locus << "," << descrst << endl;
                    cout << seqst << endl;
                    exit(0);
                }
                if(deposit == true)
                    rc = sqlite3_exec(conn, sql.c_str(), 0, 0, 0);
                seqst = "";
                descrst = "";
                locus = "";
                taxid = "";
                seq = false;
                descr = false;
                acc = "";
                ver = "";
                titlest = "";
                title = false;
                titledone = false;
            }else{
                for(int j=1;j<tokens.size();j++){
                    seqst+=tokens[j];
                }
                continue;
            }
        }
/*
//SHOULDN'T NEED THIS
        if(tokens[0] == "//"){
        cout << "next seq" << endl;
        break;
        }
*/
    }
    }
    sqlite3_exec(conn, "COMMIT TRANSACTION", NULL, NULL, NULL);
    sqlite3_close(conn);
}

GenBankReader::~GenBankReader(){

}
