#include <iostream>
#include <string>
#include <stdexcept>
#include <exception>
#include <vector>
#include <sys/stat.h>

#include "io.hpp"
#include "../STRING_SUPPORT/string_support.hpp"
#include "../PARAMETERS/parameters.hpp"

using namespace std;

ArgumentParser::ArgumentParser(set<vector<string>> flags){

  maxShortFlagSize = 0;
  maxFullFlagSize = 0;
  maxDescriptionSize = 0;   
  for( auto flag : flags ){
    
    if(flag.size()>3){
      throw invalid_argument("Flags may only be passed with a max of three "
        "values: full flag name, abbreviated name, and flag description");
    }

    string full_flag = "";
    string short_flag = "";
    string desc = "";
 
    bool description=false;
    bool abbreviation = false;
    bool flag_name = false;
    for(auto item : flag){
      trim(item);
      string full_flag_name = firstN(item,2);
      string short_flag_name = firstN(item,1);
      if(full_flag_name.compare("--")==0){
        if(flag_name){
          string err = "A flag name has already been suppied:\n";
          err.append(full_flag);
          err.append("\nNow you are supplying a second one:\n");
          err.append(item);
          throw invalid_argument(err);
        }
        full_flag = item;
        flag_name = true;
        if(full_flag.size()>maxFullFlagSize){
          maxFullFlagSize = full_flag.size();
        }
      }else if(short_flag_name.compare("-")==0){
        if(abbreviation){
          string err = "A flag name has already been suppied:\n";
          err.append(short_flag);
          err.append("\nNow you are supplying a second one:\n");
          err.append(item);
          throw invalid_argument(err);
        }
        short_flag = item;
        abbreviation = true;
        if(short_flag.size()>maxShortFlagSize){
          maxShortFlagSize = short_flag.size();
        }
      }else{
        if(description){
          string err = "A description has already been suppied:\n";
          err.append(desc);
          err.append("\nNow you are supplying a second one:\n");
          err.append(item);
          throw invalid_argument(err);
        }
        desc = item;
        description = true;
        if(desc.size()>maxDescriptionSize){
          maxDescriptionSize = desc.size();
        }
      } 
    }
    if(!flag_name){
      throw invalid_argument("You have failed to provide the full flag name");
    }
    flags_[make_pair(full_flag,short_flag)] = desc;
  } 
}

void ArgumentParser::showUsage(void){
  cout << endl;
  cout << "Options:" << endl;
  cout << endl;

  for(auto item : flags_ ){
    string full_flag = item.first.first;
    size_t diff = maxFullFlagSize-full_flag.size();
    cout << full_flag;
    for(size_t i=0;i<=diff;++i){
      cout << " ";
    }
    string short_flag = item.first.second;
    diff = maxShortFlagSize-short_flag.size();
    cout << short_flag;
    for(size_t i=0;i<=diff;++i){
      cout << " ";
    }

    size_t space = maxFullFlagSize+maxShortFlagSize+2;
    size_t allowed = maxLineLength-space;
    string space_block = "";
    for(size_t sp=0;sp<space;sp++){
      space_block.append(" ");
    }

    string desc = item.second;
    vector<string> vec_string = splitSt(desc);
    size_t words_size = 0;
    for( auto word : vec_string ){
      words_size+=word.size()+1;
      if(words_size<allowed){
        cout << word << " ";
      }else{
        cout << endl;
        cout << space_block;
        cout << word << " ";
        words_size = 0;
      }
    }
    cout << endl;
  }
  cout << endl;
}

void setFlagType(pair<string,string> flag_type, map<string,string> rules){
  if(flag_type.second.compare("FILE")==0){
  
  }else if(flag_type.second.compare("DOUBLE")==0){

  }else if(flag_type.second.compare("INT")==0){

  }else if(flag_type.second.compare("STRING")==0){

  }else{
    throw runtime_error("Unrecognized flag type "+flag_type.second);
  }
}

void ArgumentParser::flagTypeCheck(){

}

void ArgumentParser::parseArg(int & index, char *argv[], const int argc){

}

void ArgumentParser::parse(char * argv[], int argc){

  if(argc <= 1){
	  cout << "Usage: " << argv[0] << " <options(s)> SOURCES"
    showUsage();
    throw runtime_error("Must provide arguments");
  }

  for( int index=1; index<argc;++index){
    parseArg(index,argv,argc);    
  }
}

void show_usage(string name) {
	
	cerr << "Usage: " << name << " <options(s)> SOURCES"
						<< '\n'
						<< "Options:\n"
						<< "\t-h,      --help             \tShow this help message\n"
						<< "\t-l_1,    --log1  PATH/LOGFILE\tSpecify the Gaussian .log file\n"
						<< "\t                            \tfor monomer 1 this is important\n"
						<< "\t                            \tfor determining the HOMO and LUMO\n"
						<< "\t-l_2,    --log2  PATH/LOGFILE\tSpecify the Gaussian .log file\n"
						<< "\t                            \tfor monomer 2 this is important\n"
						<< "\t                            \tfor determining the HOMO and LUMO\n"
						<< "\t-l_P,    --logP  PATH/LOGFILE\tSpecify the Gaussian .log file\n"
						<< "\t                            \tfor the dimer pair, must contain\n"
						<< "\t                            \tthe overlap matrix for the dimer\n"
						<< "\t-p_1,    --pun1  PATH/PUNFILE\tSpecify the Guassian .pun file\n"
						<< "\t                            \tfor the first monomer\n"
						<< "\t-p_2,    --pun2  PATH/PUNFILE\tSpecify the Guassian .pun file\n"
						<< "\t                            \tfor the second monomer\n"
						<< "\t-p_P,    --punP  PATH/PUNFILE\tSpecify the Gaussian .pun file\n"
						<< "\t                            \tshould be an interger value\n"
						<< endl;
}

int file_exist(char * name){
  struct stat buffer;
  return (stat (name, &buffer) == 0);
}

Parameters check_arguments(char * argv[], int argc  ){
	
  Parameters par;
  string log;
	
	if(argc <= 1) {
		show_usage(argv[0]);
		throw runtime_error("Must provide arguments");
	}

	int logP_flag;
	int log1_flag;
	int log2_flag;
	int err_exit_flag;

	logP_flag = 0;
	log1_flag = 0;
	log2_flag = 0;
	err_exit_flag = 0;

	string temp;
	string HOMO1log;
	string HOMO2log;
	string P_log;
	string arg;
	string ext;

	for( int i=1;i<argc;i++){
		arg=argv[i];
		if((arg=="-h")||(arg=="--help")){
			show_usage(argv[0]);
      exit(0);
		}else if ((arg=="-l_2")||(arg=="--log2")){
			if(i+1<argc){
				if(check_string_input(argv[i+1])==-1){
					temp = argv[i+1];
					ext = lastN(temp,4);
					if(ext==".log"){
						if(file_exist(const_cast<char*>((temp).c_str()))==0){
							cerr << "ERROR The file "+(temp)+" does not exist!" << endl;
							err_exit_flag = -1;
						}else{
							log1_flag = 1;
							HOMO1log = temp;
              par.setLog1(temp);
						}
						i++;
					}else{
						cerr << "WARNING --log2 option has no file specified!" << endl;
					}
				}else{
					cerr << "ERROR --log2 option requires *.log file!" << endl;
					err_exit_flag = -1;
				}
			}else{
				cerr << "ERROR --log2 option requires one argument!" << endl;
				err_exit_flag = -1;
			}
		}else if ((arg=="-l_1")||(arg=="--log1")){
			if(i+1<argc){
				if(check_string_input(argv[i+1])==-1){
					temp = argv[i+1];
					ext = lastN(temp,4);
					if(ext==".log"){
						if(file_exist(const_cast<char*>((temp).c_str()))==0){
							cerr << "ERROR The file "+(temp)+" does not existi!" << endl;
							err_exit_flag = -1;
						}else{
							log2_flag = 1;
							HOMO2log = temp;
              par.setLog2(temp);
						}
						i++;
					}else{
						cerr << "WARNING --log1 option has no file specified!" << endl;
					}
				}else{
					cerr << "ERROR --log1 option requires *.log file!" << endl;
					err_exit_flag = -1;
				}
			}else{
				cerr << "ERROR --log1 option requires one argument!" << endl;
				err_exit_flag = -1;
			}
		}else if ((arg=="-l_P")||(arg=="--logP")){
			if(i+1<argc){
				if(check_string_input(argv[i+1])==-1){
					log = argv[i+1];
					ext = lastN(log,4);
					if(ext==".log"){
						if(file_exist(const_cast<char*>((log).c_str()))==0){
							cerr << "ERROR The file "+(log)+" does not exist!" << endl;
							err_exit_flag = -1;
						}else{
							logP_flag = 1;
              par.setLogP(argv[i+1]);
						}
						i++;
					}else{
						cerr << "WARNING --logP option has no file specified!" << endl;
					}
				}else{
					cerr << "ERROR --logP option requires *.log file!" << endl;
					err_exit_flag = -1;
				}
			}else{
				cerr << "ERROR --logP option requires one argument!" << endl;
				err_exit_flag = -1;
			}
		}else if ((arg=="-p_2")||(arg=="--pun2")){
			if(i+1<argc){
				if(check_string_input(argv[i+1])==-1){
					string pun2 = argv[i+1];
					ext = lastN(pun2,4);
					if(ext==".pun"){
						if(file_exist(const_cast<char*>((pun2).c_str()))==0){
							cerr << "ERROR The file "+(pun2)+" does not exist!" << endl;
							err_exit_flag = -1;
						}
            par.setPun2(pun2);
						//Check if log file exist with same name as pun2 file
						temp = cut_end(pun2,4);
						temp = temp + ".log";
						if(file_exist(const_cast<char*>((temp).c_str()))!=0){
							if(log1_flag==0){
								log1_flag = 1;
								HOMO1log = temp;
                par.setLog1(temp);
							}
						}
						i++;
					}else{
						cerr << "WARNING --pun2 option has no file specified!" << endl;
					}
				}else{
					cerr << "ERROR --pun2 option requires *.pun file!" << endl;
					err_exit_flag = -1;
				}
			}else{
				cerr << "ERROR --pun2 option requires one argument!" << endl;
				err_exit_flag = -1;
			}
		}else if ((arg=="-p_1")||(arg=="--pun1")){
			if(i+1<argc){
				if(check_string_input(argv[i+1])==-1){
					string pun1 = argv[i+1];
					ext = lastN(pun1,4);
					if(ext==".pun"){
						if(file_exist(const_cast<char*>((pun1).c_str()))==0){
							cerr << "ERROR The file "+(pun1)+" does not exist!" << endl;
							err_exit_flag = -1;
						}
						//Check if log file exist with same name as pun1 file
            par.setPun1(pun1);
						temp = cut_end(pun1,4);
						temp = temp + ".log";
						if(file_exist(const_cast<char*>((temp).c_str()))!=0){
							if(log2_flag==0){
								log2_flag = 1;
								HOMO2log = temp;
                par.setLog2(temp);
							}
						}
						i++;
					}else{
						cerr << "WARNING --pun1 option has no file specified!" << endl;
					}
				}else{
					cerr << "ERROR --pun1 option requires *.pun file!" << endl;
					err_exit_flag = -1;
				}
			}else{
				cerr << "ERROR --pun1 option requires one argument!" << endl;
				err_exit_flag = -1;
			}
	
		}else if ((arg=="-p_P")||(arg=="--punP")){
			if(i+1<argc){
				if(check_string_input(argv[i+1])==-1){
					string punP = argv[i+1];
					ext = lastN(punP,4);
					if(ext==".pun"){
						if(file_exist(const_cast<char*>((punP).c_str()))==0){
							cerr << "ERROR: The file "+(punP)+" does not exist!" << endl;
							err_exit_flag = -1;
						}
						//Check if log file exist with same name as pun1 file
						temp = cut_end(punP,4);
						temp = trimmed(temp + ".log");
            par.setPunP(punP);
						if(file_exist(const_cast<char*>(temp.c_str()))!=0){
							logP_flag = 2;
							P_log = temp;
						}
						i++;
					}else{
						cerr << "WARNING --punP option has no file specified!" << endl;
					}
				}else{
					cerr << "ERROR --punP option requires *.pun file!" << endl;
					err_exit_flag = -1;
				}
			}else{
				cerr << "ERROR --punP option requires one argument!" << endl;
				err_exit_flag = -1;
			}
		}else {

			cerr << "WARNING the following is not an option " << arg << "\n";
		}
	}

	if(logP_flag ==2 && err_exit_flag!=-1){
    par.setLogP(P_log);
	}

	if(argc<7){
		cerr << "ERROR Must have a *.pun file for each monomer and the dimer,\n";
		cerr << "and a *.log file for the dimer containing the overlap matrix,\n";
		cerr << "to execute. By default the program will check to see if the\n";
		cerr << "log file has the same name as the pun files" << endl;
		show_usage(argv[0]);
		err_exit_flag = -1;
	}
	
	if(err_exit_flag==-1){
		throw invalid_argument("problem with executable arguments");
	}
  cerr << "Return"<< endl;
	return par;
}

int check_string_input(string str){

	if(str=="--log1" || str=="-l_1"){
		return 0;
	}else if(str=="--log2" || str=="-l_2"){
		return 0;
	}else if(str=="--logP" || str=="-l_P"){
		return 0;
	}else if(str=="--help" || str=="-h"){
		return 0;
	}else if(str=="--pun1" || str=="-p_1"){
		return 0;
	}else if(str=="--pun2" || str=="-p_2"){
		return 0;
	}else if(str=="--punP" || str=="-p_P"){
		return 0;
	}else{
		return -1;
	}
}
