void analyze_photons() {
  // Otwieramy plik ROOT
  TFile *file = new TFile("MyOutput.root");

  // Pobieramy ntuple
  TNtuple *ntuple = (TNtuple*)file->Get("Photons");

  // Tworzymy histogram energii
  ntuple->Draw("Energy");
  TH1F *histEnergy = (TH1F*)gPad->GetPrimitive("htemp");
  histEnergy->SetTitle("Rozkład energii fotonów optycznych");
  histEnergy->GetXaxis()->SetTitle("Energia (MeV)");
  histEnergy->GetYaxis()->SetTitle("Liczba fotonów");

  // Tworzymy histogram czasu dotarcia
  ntuple->Draw("Time>>histTime"); // Zmienione na "Time>>histTime"
  TH1F *histTime = (TH1F*)gPad->GetPrimitive("histTime"); // Pobieramy histogram o nazwie "histTime"
  histTime->SetTitle("Rozkład czasu dotarcia fotonów optycznych");
  histTime->GetXaxis()->SetTitle("Czas (ns)");
  histTime->GetYaxis()->SetTitle("Liczba fotonów");

  // Tworzymy canvas i wyświetlamy histogramy
  TCanvas *canvas = new TCanvas("canvas", "Wykresy", 800, 600);
  canvas->Divide(2, 1);

  canvas->cd(1);
  histEnergy->Draw();

  canvas->cd(2);
  histTime->Draw();

  // Zapisujemy canvas do pliku PDF
  canvas->SaveAs("wykresy_fotonow_optycznych.pdf");
}