#include <vector>
Int_t hist_color=57;
Bool_t data_print=kTRUE;
Bool_t title_print=kFALSE;
std::vector<TLegend*> legends;
std::vector<TH1D*> plots;
int legends_index=-1;
int plot_index=0;
int color_increm=6;


void superimpose(const char* file, const char* Hist_MC, const char* Hist_DATA){
  gStyle->SetOptStat(0);
  gStyle->SetLineScalePS(1);
   TFile *FILE = new TFile(file,"read");
   loopdir(FILE->GetDirectory("/Limits"),Hist_MC,Hist_DATA,"");
   legends.at(legends_index)->Draw();
}

char* RemoveDigits(char* input)
{
    char* dest = input;
    char* src = input;

    while(*src)
    {
        if (isdigit(*src)) { src++; continue; }
        *dest++ = *src++;
    }
    *dest = '\0';
    return input;
}

void loopdir (TDirectory *dir, const char* Hist_MC, const char* Hist_DATA,char * prev) {
   const char delim[2] = "_";
   TIter next (dir->GetListOfKeys());
   TKey *key;
   char *token;
    while ((key = (TKey*)next())) {
      if (strcmp(key->GetClassName(),"TH1D")==0) {
        strcpy (token,dir->GetName());
        //token = strtok(temp,delim);
        RemoveDigits(token);
        if(!data_print){
        if (strcmp(key->GetName(),Hist_DATA)==0) {
          plot_index+=1;
          plots.push_back((TH1D*)key->ReadObj());
          legends.at(legends_index)->AddEntry(plots.at(plot_index),"Data","l");
          plots.at(plot_index)->SetLineColor(1);
          plots.at(plot_index)->Draw("SAME");
          data_print=kTRUE;
          }
        }
        if (strcmp(key->GetName(),Hist_MC)==0) {
          if (strcmp(token,"nominal")==0) {
            plots.push_back((TH1D*)key->ReadObj());
            plots.at(0)->SetLineColor(51);
          }
          else{
          if(strcmp(token,prev)==0){
            plot_index+=1;
            plots.push_back((TH1D*)key->ReadObj());
            plots.at(plot_index)->SetLineColor(hist_color);
            plots.at(plot_index)->Draw("SAME hist");
            legends.at(legends_index)->AddEntry(plots.at(plot_index),dir->GetName(),"l");
            hist_color+=color_increm;
          }
          else{
            if(legends_index>-1){
            legends.at(legends_index)->Draw();}
            legends_index+=1;
            auto c1 = new TCanvas(token,token,800,400);
            data_print=kFALSE;
            TLegend * legend= new TLegend(0.65,0.65,0.9,0.9);
            legends.push_back(legend);
            hist_color=57;
            plot_index+=1;
            plots.push_back((TH1D*)key->ReadObj());
            plots.at(plot_index)->SetLineColor(hist_color);
            hist_color+=color_increm;
            plots.at(plot_index)->Draw("SAME hist");
            plots.at(plot_index)->SetTitle(token);
            legends.at(legends_index)->AddEntry(plots.at(plot_index),dir->GetName(),"l");
            plots.at(0)->Draw("SAME hist");
            legends.at(legends_index)->AddEntry(plots.at(0),"nominal","l");
          }
        }
        strcpy (prev,token);
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
