/* 
 * File:   main.cpp
 * Author: sousasag
 *
 * Created on October 3, 2011, 1:12 PM
 * 
 * Check paths in all program, and in auxiliary template files
 * line 55 for model path. line 291 for template script
 * 
 */

#include <cstdlib>
#include <sys/types.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm> 
#include <sstream>
#include <iomanip>

using namespace std;

void getParametersArrays(string path, vector<double>* teff, vector<double>* logg, vector<double>* feh);
void getGridPoints(vector<double>* vec, double value, double* gpoints);
bool checkModel(string path, string* model, double teff, double logg, double feh);
bool findSubModel(int posModel, int* iteration, int* iteration2, string path, string* model, double teff, double logg, double feh, vector<double> teffv, vector<double> loggv, vector<double> fehv);
int getPosVector(vector<double> vec, double value);
void createInputFile(string path, double teff, double logg, double feh, string model[]);
void cleanModelFile(double teff, double logg, double feh, double micro);

/*
 * 
 */
int main(int argc, char** argv) {
    if (argc < 5) { // Check the value of argc. If not enough parameters have been passed, inform user and exit.
        std::cout << "Usage is exec Teff logg [fe/h] micro \n"; // Inform the user of how to use the program
        std::cin.get();
        exit(0);
    }
    
    double teff=atof(argv[1]);
    double logg=atof(argv[2]);
    double feh=atof(argv[3]);
    double micro=atof(argv[4]);
    
    int fehpos[8]={0,1,0,1,0,1,0,1};
    int loggpos[8]={0,0,1,1,0,0,1,1};
    int teffpos[8]={0,0,0,0,1,1,1,1};
    
    string path_models;
    string type_models;
    if (logg > 3.5) type_models="MARCS_st_ppl_t01_mod"; //directory for plane paralell
    else     type_models="MARCS_st_sph_t02_mod"; //directory for spheric models 
    path_models="PATHIN/marcs_models/"+type_models+"/";
    cout << "Model Path: " << path_models << endl;

    vector<double> teffvec;
    vector<double> loggvec;
    vector<double> fehvec;     
    getParametersArrays(path_models, &teffvec, &loggvec, &fehvec);
    double teff_points[2];
    getGridPoints(&teffvec, teff, teff_points);
    double logg_points[2];
    getGridPoints(&loggvec, logg, logg_points);
    double feh_points[2];
    getGridPoints(&fehvec, feh, feh_points);
    cout << teff_points[0] << " - " << teff << " - " << teff_points[1] << endl;
    cout << logg_points[0] << " - " << logg << " - " << logg_points[1] << endl;    
    cout << feh_points[0] << " - " << feh << " - " << feh_points[1] << endl;    

    string model[8];
    for (int i=0; i<8 ; i++){
        if (checkModel(path_models, &model[i], teff_points[teffpos[i]], logg_points[loggpos[i]], feh_points[fehpos[i]]))
            cout << "Modelo (" << i+1 << "): " << model[i] << endl;
        else {
            cout << " Modelo não existe " << endl;
            int iteration=0;
            int iteration2=0;
            if (findSubModel(i, &iteration, &iteration2, path_models, &model[i], teff_points[teffpos[i]], logg_points[loggpos[i]], feh_points[fehpos[i]], teffvec, loggvec, fehvec))
                cout << "Modelo Suplente (" << i+1 << "): " << model[i] << endl;
            else 
                cout << " Modelo Suplente não existe " << endl;
        }
    }   
    
    for (int i=0; i<8 ; i++){ 
        cout << "Modelo ( "<< i+1 << "): " << model[i] << endl;
    }
    
    createInputFile(type_models, teff,logg ,feh , model);
    
    cleanModelFile(teff, logg, feh, micro);
    
    
    return 0;
}

void getParametersArrays(string path, vector<double>* teff, vector<double>* logg, vector<double>* feh){
    string tmpstr;
    string comand="ls -1 "+path+ "*.mod > tmp.file";
    system(comand.c_str());

    vector<string> model_names;    
    ifstream myReadFile;
    
    myReadFile.open("tmp.file");
    string output;
    if (myReadFile.is_open()) {
        while (!myReadFile.eof()) {
                myReadFile >> output;
//                cout << output << endl;
                model_names.push_back(output);
        }
        myReadFile.close();
    }
    system("rm tmp.file");
    model_names.erase(model_names.begin()+model_names.size()-1);
    model_names.erase(model_names.begin()+model_names.size()-1);
    int n_models=model_names.size();
    vector<double> teffvec;
    vector<double> loggvec;
    vector<double> fehvec;
    
    for (int i=0; i<n_models; i++){
        tmpstr=model_names[i];
//        cout << tmpstr.substr(path.length()+1,4) << " : " << tmpstr.substr(path.length()+7,4) << " : " << tmpstr.substr(path.length()+25,5) << endl; 
        teffvec.push_back(atof(tmpstr.substr(path.length()+1,4).c_str()));
        loggvec.push_back(atof(tmpstr.substr(path.length()+7,4).c_str()));
        fehvec.push_back(atof(tmpstr.substr(path.length()+25,5).c_str()));
    }
    vector<double>::iterator it;
    sort(teffvec.begin(), teffvec.end());
    it =unique(teffvec.begin(), teffvec.end());
    teffvec.resize( it - teffvec.begin() );
    sort(loggvec.begin(), loggvec.end());
    it =unique(loggvec.begin(), loggvec.end());
    loggvec.resize( it - loggvec.begin() );
    sort(fehvec.begin(), fehvec.end());
    it =unique(fehvec.begin(), fehvec.end());
    fehvec.resize( it - fehvec.begin() );

//    cout << " TEFF: " << endl;
//    for (it=teffvec.begin(); it!=teffvec.end(); ++it)
//            cout << " " << *it << endl;
//    cout << " LOGG: " << endl;
//    for (it=loggvec.begin(); it!=loggvec.end(); ++it)
//        cout << " " << *it << endl;
//    cout << " [Fe/H]: " << endl;
//    for (it=fehvec.begin(); it!=fehvec.end(); ++it)
//        cout << " " << *it << endl;
    *teff=teffvec;
    *logg=loggvec;
    *feh=fehvec;
    
}

void getGridPoints(vector<double>* vecpointer, double value, double* gpoints){
    vector<double> vec;
    vec=*vecpointer;
    gpoints[0]=vec[0];
    gpoints[1]=vec[vec.size()-1];
    int i=0; 
    while (i<vec.size()){
        if (vec[i] >= value){
            gpoints[1]=vec[i];
            i=vec.size();
        } else {
            gpoints[0]=vec[i];
        }
        i++; 
    }
    
}


int getPosVector(vector<double> vec, double value){
    int i=0; 
    while (i<vec.size()){
        if (vec[i] == value)
            return i;
        i++; 
    }

    return -1;
}



bool checkModel(string path, string* model, double teff, double logg, double feh){
    
//    cout << "CHECKING MODEL: " << teff << " - " << logg << " - " << feh << endl;
    
    string teffstr,loggstr,fehstr;
    ostringstream ss;
    ss << (int) teff;
    teffstr = ss.str();
    ss.str("");
    ss.setf( std::ios_base::fixed);
    ss << setw(3) << setprecision(1) << logg;
    loggstr= ss.str();
    ss.str("");
    ss.setf( std::ios_base::fixed);
    ss << setw(4)<< setprecision(2) << feh;
    fehstr= ss.str();
    ss.str("");   
//    cout << "/" << teffstr << "/" << loggstr << "/" << fehstr << "/" << endl;
    string signal="+";
    if (feh < 0 ) signal="";
    string searchstr="*"+teffstr+"*g+"+loggstr+"*t0[1-2]*z"+signal+fehstr+"*.mod";
    string comand="ls -1 "+path+searchstr + "> check.file";
    system(comand.c_str());
    
    vector<string> file_names; 
    ifstream myReadFile;
    myReadFile.open("check.file");
    string output;
    if (myReadFile.is_open()) {
        while (!myReadFile.eof()) {
                myReadFile >> output;
                file_names.push_back(output);
        }
        myReadFile.close();
    }  
    system("rm check.file");
    if (file_names.size() < 2 ) return false;    
    *model=file_names[0].substr(path.length(),file_names[0].length()-path.length());
    
    
return true;
}

bool findSubModel(int posModel, int* iteration, int* iteration2, string path, string* model, double teff, double logg, double feh, vector<double> teffv, vector<double> loggv, vector<double> fehv){
    
    //cout << "CHECKING SUB MODE FOR: " << teff << " - " << logg << " - " << feh << endl;
    // 0 low, 1 up    
    int  fehnext[8]={1,0,1,0,1,0,1};
    int loggnext[8]={0,1,1,0,0,1,1};
    int teffnext[8]={0,0,0,1,1,1,1};
    
    
    int  fehpos[8]={-1, 1,-1, 1,-1, 1,-1, 1};
    int loggpos[8]={-1,-1, 1, 1,-1,-1, 1, 1};
    int teffpos[8]={-1,-1,-1,-1, 1, 1, 1, 1};
    
    if (*iteration < 7) {
        
       int iteff=getPosVector(teffv, teff);
       int ilogg=getPosVector(loggv, logg);
       int ifeh=getPosVector(fehv, feh);
       
       iteff+=teffpos[posModel]*teffnext[*iteration];
       ilogg+=loggpos[posModel]*loggnext[*iteration]; 
       ifeh += fehpos[posModel]* fehnext[*iteration]; 
//       cout << *iteration << "-" << iteff << "-" << ilogg << "-" << ifeh  << endl;
       (*iteration)++;
       if (iteff >=0 and ilogg >=0 and ifeh>=0 and iteff < teffv.size() and ilogg < loggv.size() and ifeh < fehv.size()) {
//           cout << teffv[iteff] <<"," << loggv[ilogg] << "," << fehv[ifeh]<< endl;
           if (checkModel(path, model, teffv[iteff], loggv[ilogg], fehv[ifeh]))
                   return true;
       }
       return findSubModel(posModel, iteration, iteration2, path, model, teff, logg, feh, teffv, loggv, fehv);
        
        
    } else {
       if (*iteration2 < 7) { 
       int iteff=getPosVector(teffv, teff);
       int ilogg=getPosVector(loggv, logg);
       int ifeh=getPosVector(fehv, feh);
       
       iteff+=teffpos[posModel]*teffnext[*iteration2];
       ilogg+=loggpos[posModel]*loggnext[*iteration2]; 
       ifeh += fehpos[posModel]* fehnext[*iteration2]; 
//       cout << "ITERATION2: "<< *iteration2 << "-" << iteff << "-" << ilogg << "-" << ifeh  << endl;
       (*iteration2)++;
        if (iteff >=0 and ilogg >=0 and ifeh>=0 and iteff < teffv.size() and ilogg < loggv.size() and ifeh < fehv.size()) {
//            cout << *iteration2<< " : " << teffv[iteff] <<"," << loggv[ilogg] << "," << fehv[ifeh]<< endl;
            *iteration=0;
            return findSubModel(posModel, iteration, iteration2, path, model, teffv[iteff], loggv[ilogg], fehv[ifeh], teffv, loggv, fehv);
        }
       
       } else
           return false;
    }
    
    
}

void createInputFile(string type_models, double teff, double logg, double feh, string model[]){

    string templatefile="PATHIN/interpol_template.com";
    string inputfile="interpol_moog.com";
    string comand= "cp "+ templatefile+ " tmp ";
    system(comand.c_str());
    

    for (int i=0; i<8; i++) {
        ostringstream ss;
        ss << (int) i+1;
        string istr = ss.str();
        comand="sed s/\"set model"+istr+" =\"/\"set model"+istr+" = "+model[i]+"\"/g tmp > "+ inputfile;
        system(comand.c_str());
        string comand= "cp "+ inputfile+ " tmp ";
        system(comand.c_str());
    }
    
    comand="sed s/models_spheric/"+ type_models+"/g tmp > "+ inputfile;    
    cout << comand << endl;

    system(comand.c_str());
    comand= "cp "+ inputfile+ " tmp ";
    system(comand.c_str());
    
    ostringstream ss;
    ss << teff;
    string tmpstr=ss.str();
    comand="sed s/\"foreach Tref ()\"/\"foreach Tref ("+tmpstr+")\"/g tmp > "+ inputfile;
    system(comand.c_str());
    comand= "cp "+ inputfile+ " tmp ";
    system(comand.c_str());
    
    ss.str("");
    ss << feh;
    tmpstr=ss.str();
    comand="sed s/\"foreach zref ()\"/\"foreach zref ("+tmpstr+")\"/g tmp > "+ inputfile;
    system(comand.c_str());
    comand= "cp "+ inputfile+ " tmp ";
    system(comand.c_str());
    
    ss.str("");
    ss << logg;
    tmpstr=ss.str();
    comand="sed s/\"foreach loggref ()\"/\"foreach loggref ("+tmpstr+")\"/g tmp > "+ inputfile;
    system(comand.c_str());
    comand= "cp "+ inputfile+ " tmp ";
    system(comand.c_str());
    comand="chmod a+x "+inputfile;
    system(comand.c_str());
    system("rm tmp");
    
}

void cleanModelFile(double teff, double logg, double feh, double micro){
    
    string comand="./interpol_moog.com";
    system(comand.c_str());
    ofstream myfile;
    myfile.open ("out_marcs.atm");
    myfile << "KURUCZ" << endl;
    myfile << "          Teff= " << teff << "          log g= " << logg << endl;
    myfile << "NTAU        56" << endl;
    myfile.close();
    
    comand="cat model2.moog | head -58 | sed -e '1d' -e '2d' >> out_marcs.atm";
    system(comand.c_str());
    myfile.open ("out_marcs.atm", fstream::app);
    ostringstream ss;
    ss.setf( std::ios_base::fixed);
    ss << setw(5) << setprecision(3) << micro;
    string tmpstr= ss.str();
    myfile << "    "<< tmpstr<< "e+05" << endl;
    ss.str("");
    ss << setw(4) << setprecision(2) << feh;
    tmpstr= ss.str();
    myfile << "NATOMS     1  "<< tmpstr<< endl;
    ss.str("");
    ss << setw(4) << setprecision(2) << (feh+7.47);
    tmpstr= ss.str();
    myfile << "      26.0   "<< tmpstr<< endl;
    myfile << "NMOL      19" << endl;
    myfile << "      606.0    106.0    607.0    608.0    107.0    108.0    112.0    707.0"<< endl;
    myfile << "      708.0    808.0     12.1  60808.0  10108.0    101.0      6.1      7.1"<< endl;
    myfile << "        8.1    822.0     22.1"<< endl;
    myfile.close();
    system("rm modele.sm");
    system("rm model1.interpol  model2.moog");
//	WRITE(12,'(6x,a4,3x,f4.2)')  '26.0', logefe
//	WRITE(12,'(a12)') 'NMOL      19'
//	WRITE(12,'(a74)') '      606.0    106.0    607.0    608.0    107.0    108.0    112.0    707.0'
//	WRITE(12,'(a74)') '      708.0    808.0     12.1  60808.0  10108.0    101.0      6.1      7.1'
//	WRITE(12,'(a29)') '        8.1    822.0     22.1'
 
}
