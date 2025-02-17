#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <vector>
#include <iostream>

void trajektoria(int targetCaseNumber) {
    TFile *file = new TFile("MyOutput.root", "READ");
    if (!file || file->IsZombie()) {
        std::cerr << "Błąd otwierania pliku ROOT!" << std::endl;
        return;
    }

    // Pobierz ntuple
    TTree *tree = (TTree*)file->Get("Photons");
    if (!tree) {
        std::cerr << "Nie znaleziono ntupla w pliku!" << std::endl;
        return;
    }

    int caseNumber; 
    Double_t xPos, yPos, zPos;

    tree->SetBranchAddress("EventID", &caseNumber);
    tree->SetBranchAddress("xPos", &xPos);
    tree->SetBranchAddress("yPos", &yPos);
    tree->SetBranchAddress("zPos", &zPos);

    std::vector<Double_t> xPositions;
    std::vector<Double_t> zPositions;

    Long64_t nEntries = tree->GetEntries();
    for (Long64_t i = 0; i < nEntries; i++) {
        tree->GetEntry(i);
        
        if (caseNumber == targetCaseNumber) {
            xPositions.push_back(xPos);
            zPositions.push_back(zPos);
        }
    }

    // Tworzenie TGraph dla jednego fotonu
    TGraph *graph = new TGraph(xPositions.size(), &zPositions[0], &xPositions[0]);
    graph->SetLineColor(kBlue);  
    
    // Rysowanie trajektorii fotonu
    TCanvas *c2 = new TCanvas("c2", "Trajektoria fotonu", 800, 600);
    graph->SetTitle(Form("Trajektoria fotonu %d; Z [cm]; X [cm]", targetCaseNumber));
    graph->Draw("AL");

    c2->SaveAs(Form("photon_trajectory_%d.png", targetCaseNumber));

    delete c2;
    delete graph;
    file->Close();
    delete file;
}

