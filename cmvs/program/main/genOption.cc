#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

int main(int argc, char* argv[]) {
  if (argc < 2) {
    cerr << "Usage: " << argv[0]
         << " prefix level[=1] csize[=2] threshold[=0.7] wsize[=7]"
         << " minImageNum[=3] CPU[=8]" << endl
         << endl
         << "Parameters are for PMVS2 option files." << endl
         << "Please refer to the following URL for the definitions:" << endl
         << "http://grail.cs.washington.edu/software/pmvs/documentation.html" << endl;
    exit (1);
  }

  int level = 1;
  if (3 <= argc)
    level = atoi(argv[2]);

  int csize = 2;
  if (4 <= argc)
    csize = atoi(argv[3]);
  
  float threshold = 0.7;
  if (5 <= argc)
    threshold = atof(argv[4]);

  int wsize = 7;
  if (6 <= argc)
    wsize = atoi(argv[5]);
  
  int minImageNum = 3;
  if (7 <= argc)
    minImageNum = atoi(argv[6]);
  
  int CPU = 8;
  if (8 <= argc)
    CPU = atoi(argv[7]);
  
  const int setEdge = 0;
  const int useBound = 0;
  const int useVisData = 1;
  const int sequence = -1;
  
  ifstream ifstr;
  char ske[1024];
  sprintf(ske, "%sske.dat", argv[1]);
  ifstr.open(ske);
  
  string header;
  int inum, cnum;
  ifstr >> header >> inum >> cnum;

  ofstream script;
  char pmvsfile[1024];
  sprintf(pmvsfile, "%spmvs.sh", argv[1]);
  script.open(pmvsfile);

  for (int c = 0; c < cnum; ++c) {
    ofstream ofstr;
    char buffer[1024];
    sprintf(buffer, "%soption-%04d", argv[1], c);
    ofstr.open(buffer);

    sprintf(buffer, "pmvs2 pmvs/ option-%04d", c);
    script << buffer << endl;

    ofstr << "# generated by genOption. mode 1. cluster: " << c << endl
          << "level " << level << endl
          << "csize " << csize << endl
          << "threshold " << threshold << endl
          << "wsize " << wsize << endl
          << "minImageNum " << minImageNum << endl
          << "CPU " << CPU << endl
          << "setEdge " << setEdge << endl
          << "useBound " << useBound << endl
          << "useVisData " << useVisData << endl
          << "sequence " << sequence << endl
          << "maxAngle 10" << endl
          << "quad 2.0" << endl;
    
    int timagenum, oimagenum;
    ifstr >> timagenum >> oimagenum;
    
    vector<int> timages, oimages;
    timages.resize(timagenum);
    oimages.resize(oimagenum);
    for (int i = 0; i < timagenum; ++i)
      ifstr >> timages[i];
    for (int i = 0; i < oimagenum; ++i)
      ifstr >> oimages[i];
    
    ofstr << "timages " << timagenum << ' ';
    for (int i = 0; i < timagenum; ++i)
      ofstr << timages[i] << ' ';
    ofstr << endl;
    ofstr << "oimages " << oimagenum << ' ';
    for (int i = 0; i < oimagenum; ++i)
      ofstr << oimages[i] << ' ';
    ofstr << endl;
    ofstr.close();
  }
  ifstr.close();
  script << endl;
  script.close();
}