#include <vector>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <string>
Int_t hist_color=57;
Bool_t data_print=kTRUE;
Bool_t title_print=kFALSE;
std::vector<TLegend*> legends;
std::vector<TH1D*> plots;
std::vector<std::string> variables;
std::vector<Int_t> n_entries;
std::vector<TCanvas*> canvases;
int legends_index=-1;
int plot_index=0;
int color_increm=6;
Double_t max_hist=0;
int first_hist=0;


void superimpose(const char* file, const char* Hist_MC, const char* Hist_DATA, const char* print_folder){
  gStyle->SetOptStat(0);
  gStyle->SetLineScalePS(1);
   TFile *FILE = new TFile(file,"read");
   loopdir(FILE->GetDirectory("/Limits"),Hist_MC,Hist_DATA,"");
   legends.at(legends_index)->Draw();
   PrintAllCanvases(Hist_MC,print_folder);
}


void PrintAllCanvases(char* Hist_MC,const char* print_folder){

  for(int i = 0; i < canvases.size(); i++) {
    string file_name(print_folder);
    file_name+=Hist_MC;
    file_name+="_";
    file_name+=variables.at(i);
    file_name+="_";
    file_name+="100k";
    file_name+=".pdf";
    cout << file_name << endl;
    (canvases.at(i))->Print(file_name.c_str(),"pdf");
  }

}


char* RemoveDigits(char* input)
{
    char* dest = input;
    char* src = input;

    while(*src)
    {
        if (isdigit(*src)) { src++; continue; }
        if (*src=='-'|| *src=='.') { src++; continue; }
        if (*src=='_') { *dest++ = ' '; src++; continue; }
        *dest++ = *src++;
    }
    *dest = '\0';
    return input;
}

string format_name(string name){
  string charToRemove(".-1234567890");
        for(int i = 0; i< charToRemove.length();i++)
        {
            int position = name.find(charToRemove.at(i),0);
            if (position > 0)
            {
              name.erase(position);
            }
        }
    if(name.at(name.size() - 1)=='_'){
    name.erase(name.size() - 1);}
    return name;
}

void loopdir (TDirectory *dir, const char* Hist_MC, const char* Hist_DATA,char * prev) {
   const char delim[2] = "_";
   TIter next (dir->GetListOfKeys());
   TKey *key;
  // char token[50];
    while ((key = (TKey*)next())) {
      if (strcmp(key->GetClassName(),"TH1D")==0) {
        //strcpy (token,dir->GetName());
        string token(dir->GetName());
        token=format_name(token);
        if(!data_print){
        if (strcmp(key->GetName(),Hist_DATA)==0) {
          plot_index+=1;
          plots.push_back((TH1D*)key->ReadObj());
          legends.at(legends_index)->AddEntry(plots.at(plot_index),"Data","l");
          plots.at(plot_index)->SetLineColor(1);
          plots.at(plot_index)->Draw("SAME");
          if(plots.at(plot_index)->GetBinContent(plots.at(plot_index)->GetMaximumBin())>max_hist){
            max_hist=plots.at(plot_index)->GetBinContent(plots.at(plot_index)->GetMaximumBin());
            plots.at(first_hist)->SetMaximum(1.15*max_hist);
          }
          data_print=kTRUE;
          }
        }
        if (strcmp(key->GetName(),Hist_MC)==0) {
          if (strcmp(token.c_str(),"nominal")==0) {
            plots.push_back((TH1D*)key->ReadObj());
            plots.at(0)->SetLineColor(51);
          }
          else{
          if(strcmp(token.c_str(),prev)==0){
            plot_index+=1;
            plots.push_back((TH1D*)key->ReadObj());
            plots.at(plot_index)->SetLineColor(hist_color);
            if(plots.at(plot_index)->GetBinContent(plots.at(plot_index)->GetMaximumBin())>max_hist){
              max_hist=plots.at(plot_index)->GetBinContent(plots.at(plot_index)->GetMaximumBin());
              plots.at(first_hist)->SetMaximum(1.05*max_hist);
            }
            plots.at(plot_index)->Draw("SAME hist");
            legends.at(legends_index)->AddEntry(plots.at(plot_index),dir->GetName(),"l");
            hist_color+=color_increm;
          }
          else{
            if(legends_index>-1){
            legends.at(legends_index)->Draw();}
            legends_index+=1;
            plot_index+=1;
            auto c1 = new TCanvas(token.c_str(),token.c_str(),800,400);
            canvases.push_back(c1);
            data_print=kFALSE;
            TLegend * legend= new TLegend(0.65,0.65,0.9,0.9);
            legends.push_back(legend);
            hist_color=57;
            first_hist=plot_index;
            plots.push_back((TH1D*)key->ReadObj());
            variables.push_back(token);
            n_entries.push_back(plots.at(0)->GetEntries());
            plots.at(plot_index)->SetLineColor(hist_color);
            plots.at(plot_index)->SetMinimum(0);
            max_hist=plots.at(plot_index)->GetBinContent(plots.at(plot_index)->GetMaximumBin());
            hist_color+=color_increm;
            plots.at(plot_index)->Draw("SAME hist");
            cout << token.c_str() << endl;
            plots.at(plot_index)->SetTitle(token.c_str());
            legends.at(legends_index)->AddEntry(plots.at(plot_index),dir->GetName(),"l");
            plots.at(0)->Draw("SAME hist");
            legends.at(legends_index)->AddEntry(plots.at(0),"nominal","l");
          }
        }
        strcpy (prev,token.c_str());
      }
      }
      if (!strcmp(key->GetClassName(),"TDirectoryFile")) {
         dir->cd(key->GetName());
         TDirectory *subdir = gDirectory;
         loopdir(subdir, Hist_MC,Hist_DATA,prev);
         dir->cd();
    }
  }
}
