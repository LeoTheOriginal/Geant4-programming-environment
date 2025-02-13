// analyzePhotons.C
//
// Macro to analyze an ntuple containing optical photon data.
// It automatically adjusts the histogram ranges to fit the data,
// then creates and draws histograms for the energy (in MeV) and time (in ns)
// of optical photons reaching the end of the fiber.

void analyzePhotons() {
  // Open the ROOT file containing the ntuple
  TFile* file = TFile::Open("MyOutput.root");
  if (!file || file->IsZombie()) {
    std::cout << "Error: Could not open file MyOutput.root" << std::endl;
    return;
  }

  // Get the TTree "Photons" from the file
  TTree* tree = (TTree*)file->Get("Photons");
  if (!tree) {
    std::cout << "Error: TTree 'Photons' not found in the file." << std::endl;
    return;
  }

  // Set up variables to read data from the tree branches
  Double_t energy = 0;
  Double_t time = 0;
  tree->SetBranchAddress("Energy", &energy);
  tree->SetBranchAddress("Time", &time);

  // Automatically determine the range for the energy histogram
  Double_t minEnergy = tree->GetMinimum("Energy");
  Double_t maxEnergy = tree->GetMaximum("Energy");
  if (minEnergy == maxEnergy) {
    // If all data points have the same energy, add a default margin
    minEnergy -= 1;
    maxEnergy += 1;
  } else {
    // Add a 10% margin to both sides
    Double_t marginE = 0.1 * (maxEnergy - minEnergy);
    minEnergy -= marginE;
    maxEnergy += marginE;
  }

  // Automatically determine the range for the time histogram
  Double_t minTime = tree->GetMinimum("Time");
  Double_t maxTime = tree->GetMaximum("Time");
  if (minTime == maxTime) {
    minTime -= 1;
    maxTime += 1;
  } else {
    Double_t marginT = 0.1 * (maxTime - minTime);
    minTime -= marginT;
    maxTime += marginT;
  }

  // Create a histogram for the energy of optical photons
  TH1D* hEnergy = new TH1D("hEnergy", "Optical Photons Energy Distribution", 100, minEnergy, maxEnergy);
  hEnergy->GetXaxis()->SetTitle("Energy (MeV)");
  hEnergy->GetYaxis()->SetTitle("Number of Photons");

  // Create a histogram for the time of optical photons
  TH1D* hTime = new TH1D("hTime", "Optical Photons Time Distribution", 100, minTime, maxTime);
  hTime->GetXaxis()->SetTitle("Time (ns)");
  hTime->GetYaxis()->SetTitle("Number of Photons");

  // Fill the histograms with data from the tree
  Long64_t nEntries = tree->GetEntries();
  for (Long64_t i = 0; i < nEntries; ++i) {
    tree->GetEntry(i);
    hEnergy->Fill(energy);
    hTime->Fill(time);
  }

  // Create a canvas and divide it into two pads for drawing both histograms
  TCanvas* c = new TCanvas("c", "Optical Photons Histograms", 1200, 600);
  c->Divide(2, 1);

  // Draw the energy histogram in the first pad
  c->cd(1);
  hEnergy->Draw();

  // Draw the time histogram in the second pad
  c->cd(2);
  hTime->Draw();

  // Save the canvas as a PNG image
  c->SaveAs("PhotonHistograms.png");
}
